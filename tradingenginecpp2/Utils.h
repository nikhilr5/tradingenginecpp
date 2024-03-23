#pragma once
#include <ctime>
#include <nlohmann/json.hpp>

double CalculateEma(double current_close, double previous_ema, double period);

double CalculateEmaDerrivative();

double getMinutesBetweenEpochTimes(long long timestamp1, long long timestamp2);

std::string ComputeSignature(const std::string& data, std::string apiSecret);

std::string GetTimestamp();

std::string GeneratePostSignature(const nlohmann::json& parameters, std::string apiKey, std::string apiSecret, std::string timestamp);