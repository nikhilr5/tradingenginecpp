#include <iostream>
#include "WebsocketConnection.h"
#include <thread>
#include <string>
#include "Analyzer.h"
#include "Trade.h"
#include <cstdlib>
#include "tradingenginecpp2.h"
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include "Utils.h"
#include <atomic>

namespace TradingEngine{
    std::string ApiKey ="";
    std::string ApiSecret="";
    double TakeProfitPercent;
    double StopLossPercent;
    double Quantity;
    double Leverage;
    double LimitPriceDifferenceFromMarketPrice;
    std::string Symbol;
    double EmaPeriod;
    std::ofstream* OutputFile;
    std::atomic<int> AttemptsForLevel;
    std::mutex TradeLock;
}

int main(int argc, char* argv[])
{
    std::string outputFileDirectory = "";
    try {
        Analyzer::Level = std::stod(argv[1]);
        TradingEngine::TakeProfitPercent = std::stod(argv[2]);
        TradingEngine::StopLossPercent = std::stod(argv[3]);
        TradingEngine::Quantity = std::stod(argv[4]);
        TradingEngine::Leverage = std::stod(argv[5]);
        TradingEngine::LimitPriceDifferenceFromMarketPrice = std::stod(argv[6]);
        TradingEngine::Symbol = argv[7];
        TradingEngine::EmaPeriod = std::stod(argv[8]);
        outputFileDirectory = argv[9];
        TradingEngine::AttemptsForLevel = std::stoi(argv[10]);
    }
    catch (const std::exception& e){
        std::cout << "Exception caught when trying to grab all parameters needed for executable. Exception=" << e.what() << std::endl;
        return -1;
    }

    std::string currentTimeStr = GetCurrentTimeStr();
    TradingEngine::OutputFile = new std::ofstream(outputFileDirectory + "/engine_" + currentTimeStr+".txt");
    if (!TradingEngine::OutputFile->is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
        delete TradingEngine::OutputFile; // Clean up the allocated memory
        return 1;
    }

    TradingEngine::ApiKey = std::getenv("BYBIT_API_KEY");
    TradingEngine::ApiSecret = std::getenv("BYBIT_API_SECRET");

    std::cout << "Setting support level to " << Analyzer::Level << std::endl;

    //set leverage
    SetLeverage();

    std::thread private_data_thread(ConnectPrivateWebsocket);
    std::thread orderbook_thread(ConnectOrderbookWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["orderbook.200.)" + TradingEngine::Symbol + R"("]})");
    std::thread kline_thread(ConnectKlineWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["kline.1.)" + TradingEngine::Symbol + R"("]})");

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(30)); //flush file every 30 seconds
        TradingEngine::OutputFile->flush();
    }
    orderbook_thread.join();
    kline_thread.join();
    private_data_thread.join();

    TradingEngine::OutputFile->close();
    delete TradingEngine::OutputFile;
    return 0;
}