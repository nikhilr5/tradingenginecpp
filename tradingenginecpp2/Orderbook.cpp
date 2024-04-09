#include <iostream>
#include "Orderbook.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <mutex>
#include "Analyzer.h"

using json = nlohmann::json;

//fields
std::atomic <double> Orderbook::MarketPrice;

// Constructor definition
Orderbook::Orderbook() {
}

/*
1. Keeps track of a bid and ask dictionary 200 depth each way
2. updates every 200ms the market price
*/
void Orderbook::HandleUpdate(std::string update) {
    try {
        json snapshot = json::parse(update);

        json asks = snapshot["data"]["a"];
        json bids = snapshot["data"]["b"];


        std::vector<std::vector<std::string>> asks_vec = asks.get < std::vector < std::vector<std::string> >>();
        std::vector<std::vector<std::string>> bids_vec = bids.get < std::vector < std::vector<std::string> >>();

        for (const auto& bid : bids_vec) {
            double price = std::stod(bid[0]);
            double quantity = std::stod(bid[1]);

            if (quantity == 0) {
                Bids.erase(price);
                continue;
            }

            Bids.insert_or_assign(price, quantity);
        }

        for (const auto& ask : asks_vec) {
            double price = std::stod(ask[0]);
            double quantity = std::stod(ask[1]);

            if (quantity == 0) {
                Asks.erase(price);
                continue;
            }

            Asks.insert_or_assign(price, quantity);
        }
        SetMarketPrice();
    }
    catch (const std::exception& e) {
        // Catch block to handle the exception and print out the exception information
        std::cerr << "Exception caught in orderbook ws: " << e.what() << std::endl;
    }
}

void Orderbook::SetMarketPrice() {
    std::lock_guard<std::mutex> guard(Analyzer::IndicatorLock); //scoped lock

    if (Bids.empty() || Asks.empty())
        MarketPrice = -1;

    Orderbook::MarketPrice = (Bids.rbegin()->first + Asks.begin()->first) / 2; // (highest bid + lowest ask) /2 
}
