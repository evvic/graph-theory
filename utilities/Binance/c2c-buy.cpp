#include <string>
#include <curl/curl.h>
#include "c2c-buy.h"
#include "../read-keys.h" // Reading in API keys
#include <iostream>
#include <openssl/hmac.h>   // signature
#include <openssl/sha.h>    // signature
#include <iomanip>          // signature
#include <sstream>          // signature

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
        return "";
    }

    std::string url = "https://api.binance.com/api/v3/account";

    // Calculate the request signature
    std::string request_path = "/api/v3/account";
    std::string request_method = "GET";
    std::string total_params = "";  // No parameters for this request
    std::string signature = CalculateSignature(request_path, request_method, total_params);

    // Construct the POST data for the request
    std::string post_data = "signature=" + signature;

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

// Calculates the HMAC SHA256 request signature for the specified request path, request method, and request parameters.
std::string MarketBuyC2C::CalculateSignature(const std::string& request_path, const std::string& request_method,
                                    const std::string& total_params) {
    // Concatenate the request method, request path, and total_params into a single string
    std::string data = request_method + request_path + total_params;

    // Calculate the HMAC SHA256 hash of the data using the secret key as the key
    unsigned char hash[SHA256_DIGEST_LENGTH];
    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, secret_key_.c_str(), secret_key_.length(), EVP_sha256(), NULL);
    HMAC_Update(&ctx, (unsigned char*)data.c_str(), data.length());
    HMAC_Final(&ctx, hash, &SHA256_DIGEST_LENGTH);
    HMAC_CTX_cleanup(&ctx);

    // Convert the hash to a hexadecimal string
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}