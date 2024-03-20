#include <iostream>
#include "Orderbook.h"
#include "../../json/include/nlohmann/json.hpp"
#include <vector>

using json = nlohmann::json;


// Constructor definition
Orderbook::Orderbook() {}

// Member function definition
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
        std::cout << Bids.size() << std::endl;
        SetMarketPrice();
    }
    catch (const std::exception& e) {
        // Catch block to handle the exception and print out the exception information
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

void Orderbook::SetMarketPrice() {
    if (Bids.empty() || Asks.empty())
        MarketPrice = -1;

    MarketPrice = Bids.rbegin()->first;
}
