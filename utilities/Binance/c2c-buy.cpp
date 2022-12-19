#include <string>
#include <curl/curl.h>
#include "c2c-buy.h"
#include "../read-keys.h" // Reading in API keys
#include "generate-signature.h"
#include <iostream>
#include <chrono>         // Mandatory parameter: timestamp

// Constructor runs all the functions to get the data
// Optional paramters: 
//      keysfile: path to file containing api and secret keys
//      api: variable name of api key within specified file
//      secret: variable name of secret key within specified file
MarketBuyC2C::MarketBuyC2C(const std::string keysfile, const std::string api, const std::string secret) {

    // Obtain API keys from .env file
    ReadKeys reader;
    api_key_ = reader.getKey(keysfile, api);
    secret_key_ = reader.getKey(keysfile, secret);

    // Error optaining one or both keys
    if (api_key_.length() <= 0 || secret_key_.length() <= 0) {
        std::cerr << "Could not obtain keys for private client API access." << std::endl;
    }

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
    headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + api_key_).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_USERPWD, (secret_key_ + ":").c_str());

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        return "";
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return response;
}

size_t MarketBuyC2C::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Makes a request to the Binance API to view the contents of your wallet.
// Returns the raw response from the API.
std::string MarketBuyC2C::viewWalletContents() {
    CURL* curl = curl_easy_init();
    if (!curl) {
    std::cerr << "Error initializing curl library" << std::endl;
    return "";
    }

    std::string url = "https://api.binance.com/api/v3/account";

    // Calculate the request signature
    std::string request_path = "/api/v3/account";
    std::string request_method = "POST";
    //std::string total_params = ""; // No parameters for this request

    // Get the current timestamp in milliseconds
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();
    long timestamp = value.count();

    // Add the timestamp parameter to the request parameters
    std::string total_params = "timestamp=" + std::to_string(timestamp);

    std::cout << total_params << std::endl;
    std::cout << "Calculate signature" << std::endl;

    SignatureSHA256 sign;
    std::string signature = sign.generate(secret_key_, request_path, request_method, total_params);
    std::cout << signature << std::endl;

    // Append signature to the URL as a query parameter
    url += "?signature=" + signature;

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + api_key_).c_str());
    headers = curl_slist_append(headers, ("X-MBX-SIGNATURE: " + secret_key_).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
    return "";
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    std::cout << response << std::endl;

    return response;
}