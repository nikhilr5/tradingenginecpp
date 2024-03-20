#pragma once
#include <map>
#include <string>
#include <atomic>

// Declare the class
class KlineData {
public:
    // Constructor
    KlineData();

    //fields
    std::atomic<double> MostRecentEmaPriceMinima;

    std::atomic<double> EmaPrice;
    std::atomic<double> PreviousEmaPrice;


    // Member function declaration
    void HandleUpdate(std::string update);


private:

    //functions
    void CheckForMinima();
};