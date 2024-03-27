#include "Analyzer.h"
#include "Orderbook.h"
#include "Utils.h"
#include <iostream>
#include "tradingenginecpp2.h"

namespace Analyzer {
	double Level;
	std::atomic<bool> TradePlaced = false;
	std::atomic<bool> CrossedLevel = false;
	std::atomic<double> MostRecentEmaPriceMinima = 0;
	std::atomic<double> EmaPrice = 0;
	std::atomic<double> PreviousEmaPrice = 0;
	std::atomic<long long> EmaTime;
	std::atomic<long long> PreviousEmaTime;
	std::atomic<double> PreviousDerrivative;
	std::atomic<double> Derrivative;
	std::atomic<int> EmaCount = 0;

	bool DoIndicatorsPass() {

		if (TradePlaced) //no trade placed yet
			return false;

		if (!CrossedLevel) //has to have crossed level
			return false;

		if (Orderbook::MarketPrice > Level && MostRecentEmaPriceMinima < Level) //crossed back above support level
			return true;

		return false;
	}


	void UpdateEmaInfo(double close, long long currentTime) {

		EmaCount += 1;
		PreviousEmaPrice.store(EmaPrice.load());
		EmaPrice = CalculateEma(close, PreviousEmaPrice, EmaCount < TradingEngine::EmaPeriod ? EmaCount.load() : TradingEngine::EmaPeriod);

		PreviousEmaTime.store(EmaTime.load());
		EmaTime = currentTime;

		if (PreviousEmaPrice == 0)
			return;

		PreviousDerrivative.store(Derrivative.load());
		Derrivative = CalculateEmaDerrivative();

		CheckForLocalMinima();

		auto localTime = GetLocalTime(currentTime);
		std::ostringstream oss;
		oss << "EmaPrice=" << EmaPrice << " derrivative=" << Derrivative << " MarketPrice=" << Orderbook::MarketPrice << " Timestamp=" << localTime;
		std::cout << oss.str() << std::endl;
		*(TradingEngine::OutputFile) << oss.str() << std::endl;
	}

	void CheckForLocalMinima() {
		if (CrossedLevel) {
			if (PreviousDerrivative < 0 && Derrivative > 0)
			{
				std::cout << "Hit local min for EMA line at timestamp=" << EmaTime << ", minima=" << EmaPrice << std::endl;
				MostRecentEmaPriceMinima.store(EmaPrice.load());
			}
		}
	}

	void UpdateIndicators() {
		if (TradePlaced)
			return;

		if (!CrossedLevel && Orderbook::MarketPrice < Level)
		{
			std::cout << "Crossed below support level. MarketPrice=" << Orderbook::MarketPrice << " , Level=" << Level << std::endl;
			CrossedLevel = true;
		}
		else if (CrossedLevel && Orderbook::MarketPrice > Level)
		{
			std::cout << "Support level reclaim without all constraints being met. Mp=" << Orderbook::MarketPrice << ", lastMinima=" << MostRecentEmaPriceMinima << ", Level= " << Level
				<< ", CrossedLevel=" << CrossedLevel << ", time= get time";
			CrossedLevel = false;
			MostRecentEmaPriceMinima = -1;
		}
	}
}