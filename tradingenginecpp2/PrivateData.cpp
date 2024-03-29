#include <iostream>
#include "PrivateData.h"
#include <nlohmann/json.hpp>
#include <string>
#include "Utils.h"
#include "Analyzer.h"
#include "tradingenginecpp2.h"
#include <mutex>

using json = nlohmann::json;


PrivateData::PrivateData(){}

// Member function definition
void PrivateData::HandleUpdate(std::string update) {
	try {
		json snapshot = json::parse(update);
        if (snapshot["op"].is_string()){
            std::string op = snapshot["op"];
            if (op == "pong")
                return;
        }
        if (snapshot["topic"].is_string())
        {
            std::string topic = snapshot["topic"];
            if (topic == "position")
                HandlePositionUpdate(snapshot);
        }
		std::cout << "Private Data: " << snapshot << std::endl;
	}
	catch (const std::exception& e) {
		// Catch block to handle the exception and print out the exception information
		std::cerr << "Exception caught in private ws: " << e.what() << std::endl;
	}
}

void PrivateData::HandlePositionUpdate(json snapshot) {
    std::lock_guard<std::mutex> guard(TradingEngine::TradeLock);
    try {
        json data = snapshot["data"][0];
        if (data["side"] == "Buy")
        {
            Analyzer::TradePlaced = true; //trade has been placed
            TradingEngine::AttemptsForLevel -= 1; //one less attempt at level
            *(TradingEngine::OutputFile) << "Trade placed and set TradePlace=True and decreased by 1 AttemptsForLevel=" << TradingEngine::AttemptsForLevel << ". Trade data:" << data << std::endl;
        }
        else {
            *(TradingEngine::OutputFile) << "Trade SL/TP was hit. Setting TradePlace=False. Trade data:" << data << std::endl;
            Analyzer::TradePlaced = false; //allow engine to place more trades if SL/TP hit
        }
    }
    catch (const std::exception& e) {
		// Catch block to handle the exception and print out the exception information
		std::cerr << "Exception caught when parsing position data: " << e.what() << std::endl;
	}
}