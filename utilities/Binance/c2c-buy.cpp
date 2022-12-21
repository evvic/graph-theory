#include <string>
#include <curl/curl.h>
#include "c2c-buy.h"
#include "../read-keys.h" // Reading in API keys
#include <iostream>
#include <chrono>         // Mandatory parameter: timestamp

// Constructor runs all the functions to get the data
// Optional paramters: 
//      keysfile: path to file containing api and secret keys
//      api: variable name of api key within specified file
//      secret: variable name of secret key within specified file
MarketBuyC2C::MarketBuyC2C(const std::string keysfile, const std::string api, const std::string secret) 
    : CurlScaffold (keysfile, api, secret) {

    // Obtain API keys from .env file
    // Performed in CurlScaffold constructor

    // ...   
}

// Makes a market buy request to the Binance API for the specified pair and amount.
// Returns the raw response from the API.
std::string MarketBuyC2C::marketBuy(const std::string& pair, double amount) {

    // append /test 
    // Creates and validates a new order but does not send it into the matching engin
    std::string url = "https://api.binance.com/api/v3/order/test";
    //std::string total_params = "symbol=" + pair + "&side=BUY&type=MARKET&quantity=" + std::to_string(amount);

    std::string total_params = "symbol=USDTBNB&side=BUY&type=MARKET&timeInForce=IOC&quantity=0.01";
    //std::string total_params = "symbol=USDTBNB&side=BUY&type=LIMIT&timeInForce=GTC&quantity=0.2&price=0.1&recvWindow=5000";

    // This http  request rewuires it to be a post
    const bool isPost = true;

    // Creates signature and performs all necessary curl requests
    return curlHttpRequest(url, total_params, isPost);
}

// Makes a request to the Binance API to view the contents of your wallet.
// Returns the raw response from the API.
std::string MarketBuyC2C::viewWalletContents() {

    std::string url = "https://api.binance.com/api/v3/account";
    std::string total_params = ""; // No parameters for this request

    // Creates signature and performs all necessary curl requests
    return curlHttpRequest(url, total_params);
}