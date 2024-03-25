#include <string>
#include <fstream>
#include <sstream>

namespace TradingEngine{
    extern std::string ApiKey;
    extern std::string ApiSecret;
    extern double TakeProfitPercent;
    extern double StopLossPercent;
    extern double Quantity;
    extern double Leverage;
    extern double LimitPriceDifferenceFromMarketPrice;
    extern std::string Symbol;
    extern double EmaPeriod;
    extern std::ofstream* OutputFile;
}
