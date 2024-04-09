#!/bin/bash


echo "Usage: $0 <Level> <TakeProfitPercent> <StopLossPercent> <TradeAmount> <Leverage> <LimitPriceDifferenceFromMarketPrice> <Symbol> <EmaPeriod>"


# Assign arguments to variables
Level="3600"
TakeProfitPercent="3"
StopLossPercent="2"
TradeAmount="50" #in USDT
Leverage="3"
LimitPriceDifferenceFromMarketPrice="0.003"
Symbol="ETHUSDT"
EmaPeriod="25"
LogFileDirectoryLocation="./Logs"
AttemptsForLevel="3"
UseMainnet="0" # 1 = true

# Run the executable with the provided arguments
./tradingenginecpp2exe "$Level" "$TakeProfitPercent" "$StopLossPercent" "$TradeAmount" "$Leverage" "$LimitPriceDifferenceFromMarketPrice" "$Symbol" "$EmaPeriod" "$LogFileDirectoryLocation" "$AttemptsForLevel" "$UseMainnet"
