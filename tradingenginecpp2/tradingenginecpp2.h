#include <string>
#include <fstream>
#include <sstream>
#include <atomic>
#include <mutex>

namespace TradingEngine{
    extern std::string ApiKey;
    extern std::string ApiSecret;
    extern double TakeProfitPercent;
    extern double StopLossPercent;
    extern double TradeAmount;
    extern double Leverage;
    extern double LimitPriceDifferenceFromMarketPrice;
    extern std::string Symbol;
    extern double EmaPeriod;
    extern std::ofstream* OutputFile;
    extern std::atomic<int> AttemptsForLevel;
    extern bool UseMainnet;
    extern std::string ConnectionUrlWs;
    extern std::string ConnectionUrlApi;
    
    extern std::mutex TradeLock;
}
