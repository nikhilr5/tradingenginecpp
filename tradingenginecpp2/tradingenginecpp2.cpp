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
    double TradeAmount;
    double Leverage;
    double LimitPriceDifferenceFromMarketPrice;
    std::string Symbol;
    double EmaPeriod;
    std::ofstream* OutputFile;
    std::atomic<int> AttemptsForLevel;
    std::mutex TradeLock;
    bool UseMainnet;
    std::string ConnectionUrlWs;
    std::string ConnectionUrlApi;
}

/*
main function that connects to 3 websocket sockets on three seperate threads
*/
int main(int argc, char* argv[])
{
    int success = GetEngineParameters(argv);
    if (success != 0)
        return success;

    std::cout << "Setting support level to " << Analyzer::Level << std::endl;

    //set leverage
    SetLeverage();

    std::thread private_data_thread(ConnectPrivateWebsocket);
    std::thread orderbook_thread(ConnectOrderbookWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["orderbook.200.)" + TradingEngine::Symbol + R"("]})");
    std::thread kline_thread(ConnectKlineWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["kline.1.)" + TradingEngine::Symbol + R"("]})");

    FlushToFile();

    orderbook_thread.join();
    kline_thread.join();
    private_data_thread.join();

    TradingEngine::OutputFile->close();
    delete TradingEngine::OutputFile;
    return 0;
}