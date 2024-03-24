#include <iostream>
#include "WebsocketConnection.h"
#include <thread>
#include <string>
#include "Analyzer.h"
#include "Trade.h"
#include <cstdlib>
#include "tradingenginecpp2.h"
#include <chrono>


std::string TradingEngine::ApiKey ="";
std::string TradingEngine::ApiSecret="";
double TradingEngine::TakeProfitPercent;
double TradingEngine::StopLossPercent;
double TradingEngine::Quantity;
double TradingEngine::Leverage;
double TradingEngine::LimitPriceDifferenceFromMarketPrice;
std::string TradingEngine::Symbol;

int main(int argc, char* argv[])
{
    try {
    Analyzer::Level = std::stod(argv[1]);
    TradingEngine::TakeProfitPercent = std::stod(argv[2]);
    TradingEngine::StopLossPercent = std::stod(argv[3]);
    TradingEngine::Quantity = std::stod(argv[4]);
    TradingEngine::Leverage = std::stod(argv[5]);
    TradingEngine::LimitPriceDifferenceFromMarketPrice = std::stod(argv[6]);
    TradingEngine::Symbol = argv[7];
    }
    catch (const std::exception& e){
        std::cout << "Exception caught when trying to grab all parameters needed for executable. Exception=" << e.what() << std::endl;
        return -1;
    }

    TradingEngine::ApiKey = std::getenv("BYBIT_API_KEY");
    TradingEngine::ApiSecret = std::getenv("BYBIT_API_SECRET");

    std::cout << "Setting support level to " << Analyzer::Level << std::endl;

    //set leverage
    SetLeverage();

    //start all threads
    std::thread orderbook_thread(ConnectOrderbookWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["orderbook.200.)" + TradingEngine::Symbol + R"("]})");
    std::thread kline_thread(ConnectKlineWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["kline.1.)" + TradingEngine::Symbol + R"("]})");

    std::this_thread::sleep_for(std::chrono::seconds(10));
    PlaceTrade("Buy");

    //wait for all threads to finish
    orderbook_thread.join(); 
    kline_thread.join();

    return 0;
}