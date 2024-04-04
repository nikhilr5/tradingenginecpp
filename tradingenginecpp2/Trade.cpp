#define WIN32_LEAN_AND_MEAN

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "Trade.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <httplib.h>
#include "Utils.h"
#include "tradingenginecpp2.h"
#include "Orderbook.h"
#include <cmath>

const std::string RecvWindow = "10000";

/*
calculates all trade parameters based on engine parameters and executes trade
*/
void PlaceTrade(std::string side) {
    double quantity = CalculateQuantity();
    Log("Placing Order to buy symbol="+ TradingEngine::Symbol + " atempted tradeAmount="+ std::to_string(TradingEngine::TradeAmount) + ", actual quantity="+ std::to_string(quantity));
    std::string limitPrice = std::to_string(CalculateLimitPrice());
    std::string takeProfit = std::to_string(CalculateTakeProfit());
    std::string stopLoss = std::to_string(CalculateStopLoss());

    nlohmann::json parameters = 
    {
        {"category", "linear"},
        {"symbol", TradingEngine::Symbol},
        {"side", side},
        {"positionIdx", 0},
        {"orderType", "Limit"},
        {"price", limitPrice},
        {"takeProfit", takeProfit},
        {"stopLoss", stopLoss},
        {"qty", std::to_string(quantity)},
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

double CalculateStopLoss(){
    double mp = Orderbook::MarketPrice;
    return mp - (mp * (TradingEngine::StopLossPercent / TradingEngine::Leverage / 100));
}

double CalculateTakeProfit(){
    double mp = Orderbook::MarketPrice;
    return mp + (mp * (TradingEngine::TakeProfitPercent / TradingEngine::Leverage /100));
}
double CalculateLimitPrice(){
    double mp = Orderbook::MarketPrice;
    return mp * (1 + TradingEngine::LimitPriceDifferenceFromMarketPrice);
}

/*
Converts TradeAmount to quantity for coin. Rounding will cause TradeAmount to not be exact to quantity value since precision is to the 100th for bybit
*/
double CalculateQuantity() {
    double mp = Orderbook::MarketPrice;
    double quant = TradingEngine::TradeAmount / mp;
    return std::floor(quant * 100) / 100; //round down
}

void SetLeverage() {
    nlohmann::json parameters = 
    {
        {"category", "linear"},
        {"symbol", TradingEngine::Symbol},
        {"buyLeverage", std::to_string(TradingEngine::Leverage)},
        {"sellLeverage", std::to_string(TradingEngine::Leverage)}
    };

    std::cout << "Setting leverage to " << TradingEngine::Leverage << std::endl;
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

    auto res = client.Post("/v5/position/set-leverage", headers, jsonPayload, "application/json");
    if (res) {
        std::cout << res->body << std::endl;
    }
}