#include <iostream>
#include "PrivateData.h"
#include <nlohmann/json.hpp>
#include <string>
#include "Utils.h"
#include "Analyzer.h"

using json = nlohmann::json;


PrivateData::PrivateData(){}

// Member function definition
void PrivateData::HandleUpdate(std::string update) {
	try {
		json snapshot = json::parse(update);

		std::cout << "Private Data: " << snapshot << std::endl;
	}
	catch (const std::exception& e) {
		// Catch block to handle the exception and print out the exception information
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
}