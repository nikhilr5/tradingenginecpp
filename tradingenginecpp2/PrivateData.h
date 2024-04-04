#pragma once
#include <map>
#include <string>
#include <nlohmann/json.hpp>


using json = nlohmann::json;



// Declare the class
class PrivateData {
public:
    // Constructor
    PrivateData();

    // Member function declaration
    void HandleUpdate(std::string update);

private:

    double buyPositionValue;
    double entryMarkPrice;

    void HandlePositionUpdate(json snapshot);
    double calculate_pnl(double exitMarketPrice);
};