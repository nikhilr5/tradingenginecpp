#!/bin/bash


echo "Usage: $0 <Level> <TakeProfitPercent> <StopLossPercent> <Quantity> <Leverage> <LimitPriceDifferenceFromMarketPrice> <Symbol> <EmaPeriod>"


# Assign arguments to variables
Level="3600"
TakeProfitPercent="3"
StopLossPercent="2"
Quantity="0.01"
Leverage="3"
LimitPriceDifferenceFromMarketPrice="0.003"
Symbol="ETHUSDT"
EmaPeriod="25"
LogFileDirectoryLocation="./Logs"

# Run the executable with the provided arguments
./tradingenginecpp2exe "$Level" "$TakeProfitPercent" "$StopLossPercent" "$Quantity" "$Leverage" "$LimitPriceDifferenceFromMarketPrice" "$Symbol" "$EmaPeriod" "$LogFileDirectoryLocation"