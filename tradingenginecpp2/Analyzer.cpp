#include "Analyzer.h"
#include "Orderbook.h"
#include "Utils.h"
#include <iostream>
#include "tradingenginecpp2.h"
#include "Trade.h"

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
	std::mutex IndicatorLock;
	double TotalPnl = 0;

	/*
	Called in after orderbook data is updates (every 200 ms) in WebsocketConnection.cpp
	Checks if all parameters passes and uses locks to avoid race conditions
	*/
	bool DoConditionsPass() {
		std::lock_guard<std::mutex> guard(IndicatorLock); //scoped lock
		std::lock_guard<std::mutex> guard2(TradingEngine::TradeLock);

		if (TradePlaced || TradingEngine::AttemptsForLevel <= 0) //no trade placed yet
			return false;

		if (!CrossedLevel) //has to have crossed level
			return false;

		if (Orderbook::MarketPrice > Level && MostRecentEmaPriceMinima < Level) //crossed back above support level
			return true;

		return false;
	}

	/*
	Only gets called when kline data is updated 
	recalculates the ema line and calls function to check for locally minima
	*/
	void UpdateEmaInfo(double close, long long currentTime) {

		EmaCount += 1;
		PreviousEmaPrice.store(EmaPrice.load()); // previousemaprice = emaprice;
		EmaPrice = CalculateEma(close, PreviousEmaPrice, EmaCount < TradingEngine::EmaPeriod ? EmaCount.load() : TradingEngine::EmaPeriod); // pull previous data instead?

		PreviousEmaTime.store(EmaTime.load());
		EmaTime = currentTime;

		if (PreviousEmaPrice == 0)
			return;

		PreviousDerrivative.store(Derrivative.load()); //previous_grad = current_grad
		Derrivative = CalculateEmaDerrivative();

		CheckForLocalMinima();

		if (EmaCount % 60 == 0){
			auto localTime = GetLocalTime(currentTime);
			std::ostringstream oss;
			oss << "EmaPrice=" << EmaPrice << " derrivative=" << Derrivative << " MarketPrice=" << Orderbook::MarketPrice << " Timestamp=" << localTime;
			Log(oss.str());
		}
	}

	/*
	Only checks for local minima if the below support level
	Checks by seeing if previous derrivate is less than 0 and current is great then we know we have a minima
	*/
	void CheckForLocalMinima() {
		std::lock_guard<std::mutex> guard(IndicatorLock); //scoped lock
		if (CrossedLevel) {
			if (PreviousDerrivative < 0 && Derrivative > 0)
			{
				std::string logString =  "Local minima detected for EMA line at timestamp=" + std::to_string(EmaTime) + ", minima=" + std::to_string(EmaPrice);
				Log(logString);
				MostRecentEmaPriceMinima.store(EmaPrice.load()); //MostRecentEmaPriceMinima = EmaPrice;
			}
		}
	}

	/*
	Called every time orderbook data is updated (200ms)
	Use locks to avoid race condition
	checks market price relative to parameters
	if already below and reclaimed but not all constrained pass then not updating indicators
	*/
	void UpdateConditions() {
		std::lock_guard<std::mutex> guard(IndicatorLock); //scoped lock

		auto now = std::chrono::system_clock::now();
    	long long epoch_time = std::chrono::system_clock::to_time_t(now);
		auto localTime = GetLocalTime(epoch_time);

		if (TradePlaced) //were long but still want to update CrossLevel if marketprice below level?
			return;

		if (!CrossedLevel && Orderbook::MarketPrice < Level)
		{
			Log("Market Price crossed below support level. MarketPrice=" + std::to_string(Orderbook::MarketPrice) + " , Level=" + std::to_string(Level) + ", Time="+ localTime);
			CrossedLevel = true;
		}

		if (DoConditionsPass()) // check if pass before resetting 
			PlaceTrade("Buy");


		else if (CrossedLevel && Orderbook::MarketPrice > Level)
		{
			std::string logString = "Support level reclaim without all constraints being met. Mp=" + std::to_string(Orderbook::MarketPrice) + ", lastMinima=" + 
				std::to_string(MostRecentEmaPriceMinima) +", Level= "+ std::to_string(Level) + ", CrossedLevel=" + std::to_string(CrossedLevel) + ", time" + localTime;
			Log(logString);
			CrossedLevel = false;
			MostRecentEmaPriceMinima = -1;
		}
	}
}