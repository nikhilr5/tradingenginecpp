#pragma once
#include <map>
#include <string>
#include <atomic>


// Declare the class
class KlineData {
public:
    // Constructor
    KlineData();

    // Member function declaration
    void HandleUpdate(std::string update);


private:

    //functions
    void CheckForMinima();
};