#include <iostream>
#include "KlineData.h"
#include "../../json/include/nlohmann/json.hpp"
#include <string>
#include

using json = nlohmann::json;


// Constructor definition
KlineData::KlineData() {}

// Member function definition
void KlineData::HandleUpdate(std::string update) {
	try {
		json snapshot = json::parse(update);

		json data = snapshot["data"][0];
		std::string closeStr = data["close"];
		std::string lowStr = data["low"];
		std::string openStr = data["open"];
		std::string highStr = data["high"];

		double close = std::stod(closeStr);
		bool confirm = data["confirm"];
		double high = std::stod(highStr);
		double low = std::stod(lowStr);
		double open = std::stod(openStr);


		if (!confirm)
			return; //not end of stick

		PreviousEmaPrice = EmaPrice;
		EmaPrice = CalculateEma(close, PreviousEma, 2);

		std::cout << "ema price: " << EMAPrice << std::endl;
	}
	catch (const std::exception& e) {
		// Catch block to handle the exception and print out the exception information
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
}