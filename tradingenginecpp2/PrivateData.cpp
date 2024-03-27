#include <iostream>
#include "PrivateData.h"
#include <nlohmann/json.hpp>
#include <string>
#include "Utils.h"
#include "Analyzer.h"
#include "tradingenginecpp2.h"

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
        }
		std::cout << "Private Data: " << snapshot << std::endl;
	}
	catch (const std::exception& e) {
		// Catch block to handle the exception and print out the exception information
		std::cerr << "Exception caught in private ws: " << e.what() << std::endl;
	}
}

void Private::HandlePositionUpdate(json snapshot) {
    try {
        json data = snapshot["data"][0]
        if (data["side"] == "Buy")
        {
            Analyzer::TradePlaced = true; //trade has been placed
            TradingEngine::AttemptsForLevel -= 1; //one less attempt at level
        }
        else {
            Analyzer::TradePlaced = false; //allow engine to place more trades if SL/TP hit
        }
    }
    catch (const std::exception& e) {
		// Catch block to handle the exception and print out the exception information
		std::cerr << "Exception caught when parsing position data: " << e.what() << std::endl;
	}
}