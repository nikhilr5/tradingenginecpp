#pragma once
#include <atomic>
#include <ctime>
#include <mutex>

namespace Analyzer {
	//global fields
	extern double Level;
	extern std::atomic<bool> TradePlaced;
	extern std::atomic<bool> CrossedLevel;
	extern std::atomic<double> MostRecentEmaPriceMinima;
	extern std::atomic<double> EmaPrice;
	extern std::atomic<double> PreviousEmaPrice;
	extern std::atomic<long long> EmaTime;
	extern std::atomic<long long> PreviousEmaTime;
	extern std::atomic<double> Derrivative;
	extern std::atomic<double> PreviousDerrivative;
	extern std::mutex IndicatorLock;

	bool DoIndicatorsPass();

	void UpdateEmaInfo(double close, long long currentTime);


	void CheckForLocalMinima();

	void UpdateIndicators();
}