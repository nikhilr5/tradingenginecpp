#include "Trade.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <cpp-httplib/httplib.h>
#include "Utils.h"


void PlaceTrade(std::string side) {

    std::string apiKey = "lGKEymIrY3QeL11QeS";

    std::string apiSecret = "fMODkw1OBA80tsZadoyCEbXW4EGBsGbQgheI";

    nlohmann::json parameters = 
    {
        {"category", "linear"},
        {"symbol", "ETHUSDT"},
        {"side", side},
        {"positionIdx", 0},
        {"orderType", "Limit"},
        {"qty", "0.01"},
        {"price", "18900"},
        {"timeInForce", "GTC"}
    };
    auto timestamp = GetTimestamp();
    std::string signature = GeneratePostSignature(parameters, apiKey, apiSecret, timestamp);
    std::string jsonPayload = parameters.dump();


    httplib::Client client("api.bybit.com", 443); // 443 is the default port for HTTPS
    httplib::Headers headers = {
        {"X-BAPI-API-KEY", apiKey},
        {"X-BAPI-SIGN", signature},
        {"X-BAPI-SIGN-TYPE", "2"},
        {"X-BAPI-TIMESTAMP", timestamp},
        {"X-BAPI-RECV-WINDOW", "5000"}
    };

    auto res = client.Post("/v5/order/create", headers, jsonPayload, "application/json");
    if (res) {
        std::cout << res->body << std::endl;
    }
}