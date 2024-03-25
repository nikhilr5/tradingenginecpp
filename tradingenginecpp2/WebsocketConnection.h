#pragma once
#include <string>
#include <iostream>


void ConnectOrderbookWebsocket(const char* extension, std::string subscription_message);

void ConnectKlineWebsocket(const char* extension, std::string subscription_message);

void ConnectPrivateWebsocket();