#pragma once
#include <map>
#include <string>
#include <mutex>


// Declare the class
class Orderbook {
public:
    // Constructor
    Orderbook();

    // Member function declaration
    void HandleUpdate(std::string update);

    double GetMarketPrice();

private:
    std::map<double, double> Bids;
    std::map<double, double> Asks;
    std::mutex lock;
};