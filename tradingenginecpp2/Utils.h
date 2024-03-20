#pragma once
#include <ctime>

double CalculateEma(double current_close, double previous_ema, double period);

double CalculateEmaDerrivative(double current_ema, double previous_ema, std::time_t current_time, std::time_t previous_time);