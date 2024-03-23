#include <iostream>
#include "WebsocketConnection.h"
#include <thread>
#include <string>
#include "Analyzer.h"
#include "Trade.h"


int main(int argc, char* argv[])
{
    Analyzer::Level = std::stod(argv[1]);
    std::cout << "Setting support level to " << Analyzer::Level << std::endl;

    PlaceTrade("Buy");

    std::cin.get();
    //start all threads
    std::thread orderbook_thread(ConnectOrderbookWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["orderbook.200.ETHUSDT"]})");
    std::thread kline_thread(ConnectKlineWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["kline.1.ETHUSDT"]})");

    //wait for all threads to finish
    orderbook_thread.join(); 
    kline_thread.join();

    return 0;
}