#include "Trade.h"
#include "../../json/include/nlohmann/json.hpp"
#include <iostream>


void PlaceTrade(std::string side) {
    nlohmann::json parameters = {
        {"category", "linear"},
        {"symbol", "BTCUSDT"},
        {"side", side},
        {"positionIdx", 0},
        {"orderType", "Limit"},
        {"qty", "0.001"},
        {"price", "18900"},
        {"timeInForce", "GTC"}
    };
    std::cout << "Trade Placed!" << std::endl;
    //std::string signature = Encryption::GeneratePostSignature(parameters);
    //std::string jsonPayload = parameters.dump();

    //httplib::SSLClient client("api-testnet.bybit.com", 443); // 443 is the default port for HTTPS
    //httplib::Headers headers = {
    //    {"X-BAPI-API-KEY", ApiKey},
    //    {"X-BAPI-SIGN", signature},
    //    {"X-BAPI-SIGN-TYPE", "2"},
    //    {"X-BAPI-TIMESTAMP", Timestamp},
    //    {"X-BAPI-RECV-WINDOW", RecvWindow}
    //};

    //auto res = client.Post("/v5/order/create", headers, jsonPayload, "application/json");
    //if (res) {
    //    std::cout << res->body << std::endl;
    //}
}