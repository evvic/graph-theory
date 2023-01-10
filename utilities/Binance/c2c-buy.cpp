#include <string>
#include <curl/curl.h>
#include "http-scaffold.h"
#include "c2c-buy.h"
#include "../read-keys.h" // Reading in API keys
#include <iostream>
#include <chrono>         // Mandatory parameter: timestamp

// Constructor runs all the functions to get the data
// Optional paramters: 
//      keysfile: path to file containing api and secret keys
//      api: variable name of api key within specified file
//      secret: variable name of secret key within specified file
MarketBuyC2C::MarketBuyC2C(const std::string keysfile, const std::string api, const std::string secret) {

    // Obtain API keys from .env file
    // Performed in HttpScaffold constructor
}

// Makes a market buy request to the Binance API for the specified pair and amount.
// Returns the raw response from the API.
std::string MarketBuyC2C::marketBuy(const std::string& pair, const double& amount) {
    
    // Object that handles http request
    HttpScaffold request;

    // append /test 
    // Creates and validates a new order but does not send it into the matching engin
    std::string url = "https://api.binance.com/api/v3/order/test";
    //std::string total_params = "symbol=" + pair + "&side=BUY&type=MARKET&quantity=" + std::to_string(amount);

    std::string total_params = "symbol=USDTBNB&side=BUY&type=MARKET&timeInForce=IOC&quantity=0.01&recvWindow=5000";
    
    // Query parameters (appended to url)
    std::map<std::string, std::string> params;
    params.insert(std::make_pair("symbol", "USDTBNB"));
    params.insert(std::make_pair("side", "BUY"));
    params.insert(std::make_pair("type", "MARKET"));
    params.insert(std::make_pair("quantity", "0.1"));

    // Any message to attach to the body
    std::string body = "";

    // Additional header
    std::map<std::string, std::string> headers;

    // Creates signature and performs all necessary curl requests
    return request.post(url, params, body, headers);
}

// Request a quote for the requested token pairs
// Returns the raw response from the API. Response is the quited rate and window of time
std::string MarketBuyC2C::sendQuote(const std::string& fromAsset, const std::string& toAsset, const double& fromAmount) {

    // Object that handles http request
    HttpScaffold request;

    // Creates and validates a new quote
    std::string url = "https://api.binance.com/sapi/v1/convert/getQuote";

    // Query parameters (appended to url)
    std::map<std::string, std::string> params;
    params.insert(std::make_pair("fromAsset", fromAsset));
    params.insert(std::make_pair("toAsset", toAsset));
    params.insert(std::make_pair("fromAmount", std::to_string(fromAmount)));

    // Any message to attach to the body
    std::string body = "";

    // Additional header
    std::map<std::string, std::string> headers;

    return request.post(url, params, body, headers);
}

// Makes a request to the Binance API to view the contents of your wallet.
// Returns the raw response from the API.
std::string MarketBuyC2C::viewWalletContents() {

    HttpScaffold request;

    std::string url = "https://api.binance.com/api/v3/account";
    std::string total_params = ""; // No parameters for this request

    // Creates signature and performs all necessary curl requests
    return request.get(url, std::map<std::string, std::string>(), std::map<std::string, std::string>());
}