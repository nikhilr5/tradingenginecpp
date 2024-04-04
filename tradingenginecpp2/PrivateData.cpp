#include <iostream>
#include "PrivateData.h"
#include <nlohmann/json.hpp>
#include <string>
#include "Utils.h"
#include "Analyzer.h"
#include "tradingenginecpp2.h"
#include <mutex>

using json = nlohmann::json;


PrivateData::PrivateData(){
    buyPositionValue = 0;
    entryMarkPrice = 0;
}

/*
Handles private websocket updates and ignores pong message

*/
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
	}
	catch (const std::exception& e) {
		// Catch block to handle the exception and print out the exception information
		std::cerr << "Exception caught in private ws: " << e.what() << std::endl;
	}
}

/*
handles positions updates.
1. if buy order set tradeplaced to false keeps track of entry price
2. If sell order sets tradeplaced=true and calculates pnl and logs info
*/
void PrivateData::HandlePositionUpdate(json snapshot) {
    std::lock_guard<std::mutex> guard(TradingEngine::TradeLock);
    try {
    json data = snapshot["data"][0];
    std::string topic = snapshot["topic"];
        if (data["side"] == "Buy")
        {
            std::string symbol = data["symbol"];
            Analyzer::TradePlaced = true; //trade has been placed
            TradingEngine::AttemptsForLevel -= 1; //one less attempt at level
            Log("Entered long position for symbol=" + symbol + ". Trade data=" + data.dump());
            Log("Decreased by 1 AttemptsForLevel=" + std::to_string(TradingEngine::AttemptsForLevel));
            std::string entryPosValue = data["positionValue"];
            std::string markPrice = data["markPrice"];
            buyPositionValue = std::stod(entryPosValue);
            entryMarkPrice = std::stod(markPrice);
        }
        else {
            Log("Trade SL/TP was hit. Setting TradePlace=False. Trade data:" + data.dump());
            std::string exitPosValue = data["markPrice"];
            double pnl = calculate_pnl(std::stod(exitPosValue));
            Analyzer::TotalPnl += pnl; //increments total pnl by pnl amt
            Log("PnL of trade is "+ std::to_string(pnl) + ". Total Pnl is " + std::to_string(Analyzer::TotalPnl));
            Log("EntryMarketPrice=" + std::to_string(entryMarkPrice) + " ExitMarketPrice=" + exitPosValue);
            buyPositionValue = 0;
            entryMarkPrice = 0;
            Analyzer::TradePlaced = false; //allow engine to place more trades if SL/TP hit
        }
        
    }
    catch (const std::exception& e) {
		// Catch block to handle the exception and print out the exception information
		std::cerr << "Exception caught when parsing position data: " << e.what() << std::endl;
	}
}

double PrivateData::calculate_pnl(double exitMarketPrice) {
    double diff = exitMarketPrice - entryMarkPrice;
    double fees = -0.36;
    return (diff * (buyPositionValue / entryMarkPrice)) + fees;
}