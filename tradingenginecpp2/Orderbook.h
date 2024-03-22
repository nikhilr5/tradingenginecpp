#pragma once
#include <map>
#include <string>
#include <atomic>



// Declare the class
class Orderbook {
public:
    // Constructor
    Orderbook();

    //fields
    static std::atomic <double> MarketPrice;


    // Member function declaration
    void HandleUpdate(std::string update);


private:
    //fields
    std::map<double, double> Bids;
    std::map<double, double> Asks;

    //functions
    void SetMarketPrice();
};