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
    void HandlePositionUpdate(json snapshot);
};