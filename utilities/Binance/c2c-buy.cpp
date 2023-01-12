#include <string>
#include "../http/http-scaffold.h"
#include "c2c-buy.h"
#include "../read-keys.h" // Reading in API keys
#include <json/json.h>    // for parsing JSON
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

    // Creates signature and performs all necessary http requests
    return request.post(url, params, body, headers);
}

// Makes a request to the Binance API to view the contents of your wallet.
// Returns the raw response from the API.
std::string MarketBuyC2C::viewWalletContents() {

    // Object that handles http request
    HttpScaffold request;

    std::string url = "https://api.binance.com/api/v3/account";
    std::string total_params = ""; // No parameters for this request

    // Creates signature and performs all necessary http requests
    return request.get(url, std::map<std::string, std::string>(), std::map<std::string, std::string>());
}

// Returns users (API key) wallet {"Currency_name" : amount} string, double
// Of only coins that have some amount in them ( > 0)
std::map<std::string, double> MarketBuyC2C::getMappedWallet() {
    // Init wallet map
    std::map<std::string, double> wallet;

    // Get stringified json response
    std::string response = MarketBuyC2C::viewWalletContents();

    // JSON parser
    Json::Value json_value;
    Json::Reader reader;

    // Check if can parse
    if (!reader.parse(response, json_value)) {
        std::cerr << "Error parsing JSON string!" << std::endl;
        return wallet;
    }

    try {
        // name of object holding array

        for (auto coin : json_value["balances"]) {
            std::string asset = coin["asset"].asString();
            double balance = stod(coin["free"].asString());

            // Skip any coins where the balance is 0
            if (balance == 0.0) continue;

            std::cout << coin << std::endl;
            wallet.insert(std::make_pair(asset, balance));
        }
    }  catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return wallet;
}