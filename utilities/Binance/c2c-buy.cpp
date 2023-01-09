#include <string>
#include <curl/curl.h>
#include "curl-scaffold.h"
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
    // Performed in CurlScaffold constructor

    // ...   
}

// Makes a market buy request to the Binance API for the specified pair and amount.
// Returns the raw response from the API.
std::string MarketBuyC2C::marketBuy(const std::string& pair, double amount) {

    CurlScaffold request;

    // append /test 
    // Creates and validates a new order but does not send it into the matching engin
    std::string url = "https://api.binance.com/api/v3/order/test";
    //std::string total_params = "symbol=" + pair + "&side=BUY&type=MARKET&quantity=" + std::to_string(amount);

    std::string total_params = "symbol=USDTBNB&side=BUY&type=MARKET&timeInForce=IOC&quantity=0.01&recvWindow=5000";
    
    // MUST CHANGE PARAMS IN THE FUTURE (copy total_params)
    std::map<std::string, std::string> params;
    params.insert(std::make_pair("fromAsset", "DAI"));
    params.insert(std::make_pair("toAsset", "BNB"));
    params.insert(std::make_pair("fromAmount", "2"));

    std::string body = "";

    std::map<std::string, std::string> headers;

    // Creates signature and performs all necessary curl requests
    return request.post(url, params, body, headers);
}

// Request a quote for the requested token pairs
// Returns the raw response from the API. Response is the quited rate and window of time
std::string MarketBuyC2C::sendQuote(const std::string& fromAsset, const std::string& toAsset) {

    CurlScaffold request;

    // Creates and validates a new quote
    std::string url = "https://api.binance.com/sapi/v1/convert/getQuote";

    std::map<std::string, std::string> params;
    params.insert(std::make_pair("fromAsset", "DAI"));
    params.insert(std::make_pair("toAsset", "BNB"));
    params.insert(std::make_pair("fromAmount", "2"));

    std::string body = "";

    std::map<std::string, std::string> headers;

    // Setters
    // request.setIsPost(true);
    // request.setBody();
    // request.setHeaders();
    // request.setQueryParams();
    // request.setUrl(url);

    return request.post(url, params, body, headers);

    // std::string total_params = "fromAsset=DAI&toAsset=BNB&fromAmount=2";

    // // This http  request rewuires it to be a post
    // const bool isPost = true;

    // // Creates signature and performs all necessary curl requests
    // return curlHttpRequest(url, total_params, isPost);
}

// Makes a request to the Binance API to view the contents of your wallet.
// Returns the raw response from the API.
std::string MarketBuyC2C::viewWalletContents() {

    CurlScaffold request;

    std::string url = "https://api.binance.com/api/v3/account";
    std::string total_params = ""; // No parameters for this request

    // Creates signature and performs all necessary curl requests
    return request.get(url, std::map<std::string, std::string>(), std::map<std::string, std::string>());
}