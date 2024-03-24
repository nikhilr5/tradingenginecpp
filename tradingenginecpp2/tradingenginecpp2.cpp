#include <iostream>
#include "WebsocketConnection.h"
#include <thread>
#include <string>
#include "Analyzer.h"
#include "Trade.h"
#include <cstdlib>
#include "tradingenginecpp2.h"


std::string TradingEngine::ApiKey ="";
std::string TradingEngine::ApiSecret="";

int main(int argc, char* argv[])
{
    Analyzer::Level = std::stod(argv[1]);

    TradingEngine::ApiKey = std::getenv("BYBIT_API_KEY");
    TradingEngine::ApiSecret = std::getenv("BYBIT_API_SECRET");

    std::cout << "Setting support level to " << Analyzer::Level << std::endl;

    //start all threads
    std::thread orderbook_thread(ConnectOrderbookWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["orderbook.200.ETHUSDT"]})");
    std::thread kline_thread(ConnectKlineWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["kline.1.ETHUSDT"]})");

    //wait for all threads to finish
    orderbook_thread.join(); 
    kline_thread.join();

    return 0;
}