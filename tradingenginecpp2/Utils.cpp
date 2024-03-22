#include "Utils.h"
#include "Analyzer.h"
#include <iostream>
#include <chrono>



double CalculateEma(double current_close, double previous_ema, double period) {
	double smoothing_factor = 2 / (period + 1);
	return (current_close - previous_ema) * smoothing_factor + previous_ema;
}

double CalculateEmaDerrivative() {
	double time_diff = getMinutesBetweenEpochTimes(Analyzer::PreviousEmaTime, Analyzer::EmaTime);
	double ema_diff = Analyzer::EmaPrice - Analyzer::PreviousEmaPrice;

	return ema_diff / time_diff;
}

double getMinutesBetweenEpochTimes(long long timestamp1, long long timestamp2) {
	// Calculate the difference in seconds
	long long diffInSeconds = timestamp2 - timestamp1;

	// Convert seconds to minutes as a double
	double diffInMinutes = static_cast<double>(diffInSeconds) / 60.0;

	return diffInMinutes;
}