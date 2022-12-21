#include "curl-scaffold.h"
#include <chrono>         // Mandatory parameter: timestamp
#include "../read-keys.h" // Reading in API keys
#include <iostream>       // cerr
#include <curl/curl.h>
#include "generate-signature.h" // Class for signing SHA256

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


std::string CurlScaffold::curlResponse(const std::string& url_payload, const std::string& signature, const bool& isPost) {
    // Initialize curl
    CURL* curl = curl_easy_init();
    
    std::cout << url_payload << std::endl;

    // Check if curl was successfully initialized
    if (!curl) {
        std::cerr << "Error initializing curl library" << std::endl;
        return "";
    }

    // Set HTTP request method (post || get)
    if (isPost) {
        // Set the request method to POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
    } else {
        // Set the request method to GET
        curl_easy_setopt(curl, CURLOPT_POST, 0L);
    }
    

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + api_key).c_str());
    headers = curl_slist_append(headers, ("X-MBX-SIGNATURE: " + signature).c_str());


    // Set the query parameters to send in the request
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, getQueryparamsFromUrl(url_payload).c_str());

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

// Wrapper for curlResponse function of boilerplate code for curling a HTTP request
// Requires url (only complete url) and structured parameters as query string
// Returns string of JSON resposne
std::string CurlScaffold::curlHttpRequest(std::string url, std::string total_params, const bool& isPost) {
    // Get the current timestamp in milliseconds
    long timestamp = timestampEpoch_ms();

    // Add the timestamp parameter to the request parameters
    // If "timestamp" is not the first param (and '&' not included already), include the '&'
    if (total_params.size() > 0 && total_params.back() != '&' ) {
        total_params += '&';
    }
    total_params += ("timestamp=" + std::to_string(timestamp));

    GenerateSignature sign;
    const std::string signature = sign.hmacSha256(secret_key, total_params);
    // std::cout << signature << std::endl;

    // Append total_params to the URL as a query parameter + '?'
    // Check if '?' already prepended to total_params
    url += ('?' + total_params);
    // Append signature to the URL as a query parameter
    url += ("&signature=" + signature);
    
    return curlResponse(url, urlEncode(signature), isPost);
}

// Helper funtion to URL-encode query parameters (or url)
// Returns encoded string
std::string CurlScaffold::urlEncode(const std::string& input) {
    CURL* curl = curl_easy_init();
    if (curl) {
        char* encoded = curl_easy_escape(curl, input.c_str(), input.size());
        if (encoded) {
            std::string output(encoded);
            curl_free(encoded);
            curl_easy_cleanup(curl);
            return output;
        }
    }
    return "";
}



// Returns substring of url payload given
std::string CurlScaffold::getQueryparamsFromUrl(const std::string url_payload) {
    
    size_t delim = url_payload.find_last_of('?');

    // If '?' was not found return empty string
    if (delim == std::string::npos) {
        return "";
    }

    // Add 1 to exclude the '?' itself
    delim ++;

    std::cout << url_payload.substr(delim, url_payload.length()) << std::endl;
    return url_payload.substr(delim, url_payload.length());
}