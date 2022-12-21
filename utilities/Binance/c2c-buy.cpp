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
    CURL* curl = curl_easy_init();
    if (!curl) {
        return "";
    }

    std::string url = "https://api.binance.com/api/v3/order";
    std::string post_data = "symbol=" + pair + "&side=BUY&type=MARKET&quantity=" + std::to_string(amount);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + api_key).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_USERPWD, (secret_key + ":").c_str());

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, defaultCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        return "";
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return response;
}


// Makes a request to the Binance API to view the contents of your wallet.
// Returns the raw response from the API.
std::string MarketBuyC2C::viewWalletContents() {

    std::string url = "https://api.binance.com/api/v3/account";

    std::string total_params = ""; // No parameters for this request

    // Creates signature and performs all necessary curl requests
    return curlHttpRequest(url, total_params);
}