#include "Utils.h"
#include "Analyzer.h"
#include <iostream>
#include <chrono>
#include <openssl/hmac.h>
#include <sstream>
#include "tradingenginecpp2.h"
#include <ctime>
#include <iomanip>
#include <thread>




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

std::string GetLocalTime(long long ts) {
	auto ts2 = ts / 1000.0;
	std::chrono::time_point<std::chrono::system_clock> time_point =
        std::chrono::system_clock::from_time_t(ts2);

	std::time_t time = std::chrono::system_clock::to_time_t(time_point);
    std::tm* local_time = std::localtime(&time);

	char timeStr[100];
	strftime(timeStr, sizeof(timeStr), "%m-%d-%Y %H:%M:%S", local_time);
	return timeStr;
}

int GetEngineParameters(char*  argv[]) {
	std::string outputFileDirectory = "";
    try {
        Analyzer::Level = std::stod(argv[1]);
        TradingEngine::TakeProfitPercent = std::stod(argv[2]);
        TradingEngine::StopLossPercent = std::stod(argv[3]);
        TradingEngine::TradeAmount = std::stod(argv[4]);
        TradingEngine::Leverage = std::stod(argv[5]);
        TradingEngine::LimitPriceDifferenceFromMarketPrice = std::stod(argv[6]);
        TradingEngine::Symbol = argv[7];
        TradingEngine::EmaPeriod = std::stod(argv[8]);
        outputFileDirectory = argv[9];
        TradingEngine::AttemptsForLevel = std::stoi(argv[10]);

        TradingEngine::ApiKey = std::getenv("BYBIT_API_KEY");
        TradingEngine::ApiSecret = std::getenv("BYBIT_API_SECRET");

		std::string currentTimeStr = GetCurrentTimeStr();
		TradingEngine::OutputFile = new std::ofstream(outputFileDirectory + "/engine_" + currentTimeStr+".txt");
		if (!TradingEngine::OutputFile->is_open()) {
			std::cerr << "Error: Unable to open file!" << std::endl;
			delete TradingEngine::OutputFile; // Clean up the allocated memory
			return 1;
		}

		return 0;
    }
    catch (const std::exception& e){
        std::cout << "Exception caught when trying to grab all parameters needed for executable. Exception=" << e.what() << std::endl;
        return -1;
    }
}

void FlushToFile() {
	while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(30)); //flush file every 30 seconds
        TradingEngine::OutputFile->flush();
    }
}

void Log(std::string log) {
		std::cout << log << std::endl;
		*(TradingEngine::OutputFile) << log << std::endl;
}