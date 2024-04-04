#include <iostream>
#include "KlineData.h"
#include <nlohmann/json.hpp>
#include <string>
#include "Utils.h"
#include "Analyzer.h"

using json = nlohmann::json;


// Constructor definition
KlineData::KlineData() {}

/*
Grabs all kline data on close of bar and then calls analzyer function UpdateEmaInfo to handle updates
*/
void KlineData::HandleUpdate(std::string update) {
	try {
		json snapshot = json::parse(update);

		json data = snapshot["data"][0];
		std::string closeStr = data["close"];
		std::string lowStr = data["low"];
		std::string openStr = data["open"];
		std::string highStr = data["high"];

		long long timestamp = data["timestamp"];
		double close = std::stod(closeStr);
		bool confirm = data["confirm"];
		double high = std::stod(highStr);
		double low = std::stod(lowStr);
		double open = std::stod(openStr);

		if (!confirm)
			return; //not end of stick

		Analyzer::UpdateEmaInfo(close, timestamp);
	}
	catch (const std::exception& e) {
		// Catch block to handle the exception and print out the exception information
		std::cerr << "Exception caught in kline ws: " << e.what() << std::endl;
	}
}