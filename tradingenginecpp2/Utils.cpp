#include "Utils.h"


double CalculateEma(double current_close, double previous_ema, double period) {
	double smoothing_factor = 2 / (period + 1);
	return (current_close - previous_ema) * smoothing_factor + previous_ema;
}

double CalculateEmaDerrivative(double current_ema, double previous_ema, std::time_t current_time, std::time_t previous_time) {
	double time_diff = difftime(current_time, previous_time);
	double ema_diff = current_ema - previous_ema;

	return ema_diff / time_diff;
}