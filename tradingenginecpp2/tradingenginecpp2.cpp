#include <iostream>
#include "WebsocketConnection.h"
#include <thread>

int main()
{
    //start all threads
    std::thread orderbook_thread(ConnectOrderbookWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["orderbook.200.ETHUSDT"]})");
    std::thread kline_thread(ConnectKlineWebsocket, "/v5/public/linear", R"({"op": "subscribe", "args": ["kline.1.ETHUSDT"]})");

    //wait for all threads to finish
    orderbook_thread.join(); 
    kline_thread.join();

    return 0;
}