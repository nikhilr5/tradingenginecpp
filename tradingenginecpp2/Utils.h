#pragma once
#include <ctime>
#include <nlohmann/json.hpp>

double CalculateEma(double current_close, double previous_ema, double period);

double CalculateEmaDerrivative();

double getMinutesBetweenEpochTimes(long long timestamp1, long long timestamp2);

std::string ComputeSignature(const std::string& data);

std::string GetTimestamp();

std::string GeneratePostSignature(const nlohmann::json& parameters, std::string timestamp, std::string recvWindow);

std::string GetCurrentTimeStr();

std::string GetLocalTime(long long ts);

int GetEngineParameters(char* argv[]);

void FlushToFile();