#pragma once
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>
#include "Orderbook.h"
#include "WebsocketConnection.h"
#include "KlineData.h"
#include "Analyzer.h"
#include "Trade.h"
#include <thread>
#include "tradingenginecpp2.h"
#include <chrono>
#include <nlohmann/json.hpp>


namespace net = boost::asio;
namespace ssl = net::ssl;
namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;

using tcp = net::ip::tcp;
using Request = http::request<http::string_body>;
using Stream = beast::ssl_stream<beast::tcp_stream>;
using Response = http::response<http::dynamic_body>;
using json = nlohmann::json;



class Exchange {

public:
    Exchange(std::string name, const std::string& http_host);

    void init_http(std::string const& host);

    void init_webSocket(std::string const& host, std::string const& port, const char* p = "");

    void read_Socket();

    bool is_socket_open();

    template <typename DataHandler>
    void connect(DataHandler dh);

    void write_Socket(const std::string& text);

    std::string get_socket_data();

    void buffer_clear();

    void webSocket_close();

    void authenticate();

private:
    std::string     m_name;
    net::io_context ioc;
    ssl::context    ctx{ ssl::context::tlsv12_client };
    tcp::resolver   resolver{ ioc };
    Stream          stream{ ioc, ctx };
    std::string        m_web_socket_host;
    std::string        m_web_socket_port;
    beast::flat_buffer buffer;
    net::io_context    ioc_webSocket;
    ssl::context       ctx_webSocket{ ssl::context::tlsv12_client };
    tcp::resolver      resolver_webSocket{ ioc_webSocket };
    websocket::stream<beast::ssl_stream<tcp::socket>> ws{ ioc_webSocket,
                                                         ctx_webSocket };

    std::string sign_message(const std::string& message, const std::string& secret);


};

void ConnectOrderbookWebsocket(const char* extension, std::string subscription_message);

void ConnectKlineWebsocket(const char* extension, std::string subscription_message);

void ConnectPrivateWebsocket();