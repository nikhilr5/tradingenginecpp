#include "Utils.h"
#include "Analyzer.h"
#include <iostream>
#include <chrono>
#include <openssl/hmac.h>
#include <sstream>
#include "tradingenginecpp2.h"
#include <ctime>
#include <iomanip>




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


std::string GeneratePostSignature(const nlohmann::json& parameters, std::string timestamp, std::string recvWindow) {
	std::string paramJson = parameters.dump();
	std::string rawData = timestamp + TradingEngine::ApiKey + recvWindow + paramJson;
	return ComputeSignature(rawData);
}

std::string GetTimestamp() {
	return std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - 8000);
}

std::string ComputeSignature(const std::string& data) {
	unsigned char* digest = HMAC(EVP_sha256(), TradingEngine::ApiSecret.c_str(), static_cast<int>(TradingEngine::ApiSecret.length()),
		(unsigned char*)data.c_str(), static_cast<int>(data.size()), NULL, NULL);

	std::ostringstream result;
	for (size_t i = 0; i < 32; i++) {
		result << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
	}

	return result.str();
}

std::string GetCurrentTimeStr() {
	std::time_t currentTime = std::time(nullptr);

    // Convert time to string
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y%m%d_%H%M%S");
    return ss.str();
}