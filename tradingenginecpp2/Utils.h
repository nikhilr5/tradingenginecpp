#pragma once
#include <ctime>

double CalculateEma(double current_close, double previous_ema, double period);

double CalculateEmaDerrivative();

double getMinutesBetweenEpochTimes(long long timestamp1, long long timestamp2);