#define WIN32_LEAN_AND_MEAN

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "Trade.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <httplib.h>
#include "Utils.h"
#include "tradingenginecpp2.h"

const std::string RecvWindow = "10000";
const std::string Symbol = "ETHUSDT";
const std::string Quantity = "0.01";

void PlaceTrade(std::string side) {
    
    std::cout << "Placing Order to buy symbol=" << Symbol << " quantity=" << Quantity << std::endl;

    nlohmann::json parameters = 
    {
        {"category", "linear"},
        {"symbol", Symbol},
        {"side", side},
        {"positionIdx", 0},
        {"orderType", "Market"},
        {"qty", Quantity},
        {"timeInForce", "GTC"}
    };
    auto timestamp = GetTimestamp();
    std::string signature = GeneratePostSignature(parameters, timestamp, RecvWindow);
    std::string jsonPayload = parameters.dump();


    httplib::SSLClient client("api.bybit.com", 443); // 443 is the default port for HTTPS
    httplib::Headers headers = {
        {"X-BAPI-API-KEY", TradingEngine::ApiKey},
        {"X-BAPI-SIGN", signature},
        {"X-BAPI-SIGN-TYPE", "2"},
        {"X-BAPI-TIMESTAMP", timestamp},
        {"X-BAPI-RECV-WINDOW", RecvWindow}
    };

    auto res = client.Post("/v5/order/create", headers, jsonPayload, "application/json");
    if (res) {
        std::cout << res->body << std::endl;
    }
}