#include "curl-scaffold.h"
#include <chrono>         // Mandatory parameter: timestamp
#include "../read-keys.h" // Reading in API keys
#include <iostream>       // cerr
#include <curl/curl.h>

// Constructor runs all the functions to get the data
// Optional paramters: 
//      keysfile: path to file containing api and secret keys
//      api: variable name of api key within specified file
//      secret: variable name of secret key within specified file
CurlScaffold::CurlScaffold(const std::string keysfile, const std::string api, const std::string secret)
{
    // Obtain API keys from .env file
    ReadKeys reader;
    api_key = reader.getEnvKey(keysfile, api);
    secret_key = reader.getEnvKey(keysfile, secret);

    // Error optaining one or both keys
    if (api_key.length() <= 0 || secret_key.length() <= 0) {
        std::cerr << "Could not obtain keys for private client API access." << std::endl;
    }
}

// Default callback function used for curl http requests
// Appends and stringifies all json response data to a string variable
size_t CurlScaffold::defaultCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


std::string CurlScaffold::curlResponse(const std::string& url_payload, const std::string& signature) {
    // Initialize curl
    CURL* curl = curl_easy_init();
    
    // Check if curl was successfully initialized
    if (!curl) {
        std::cerr << "Error initializing curl library" << std::endl;
        return "";
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + api_key).c_str());
    headers = curl_slist_append(headers, ("X-MBX-SIGNATURE: " + signature).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url_payload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, defaultCallback);
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

// Get the current timestamp in milliseconds
long CurlScaffold::timestampEpoch_ms() {
    // Get current time with chrono library
    auto now = std::chrono::system_clock::now();

    // Cast to milliseconds
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();

    return value.count();
}