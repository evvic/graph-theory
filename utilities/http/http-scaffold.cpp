#include "http-scaffold.h"
#include <chrono>         // Mandatory parameter: timestamp
#include "../read-keys.h" // Reading in API keys
#include <iostream>       // cerr
#include "generate-signature.h" // Class for signing SHA256
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <map>

using namespace utility;     // Common utilities like string conversions
using namespace web;         // Common features like URIs.
using namespace web::http;   // Common HTTP functionality
using namespace web::http::client;    // HTTP client features
using namespace concurrency::streams; // Asynchronous streams

// Constructor runs all the functions to get the keys
// Optional paramters:
// keysfile: path to file containing api and secret keys
// api: variable name of api key within specified file
// secret: variable name of secret key within specified file
HttpScaffold::HttpScaffold(const std::string keysfile, const std::string api, const std::string secret)
{
    // Obtain API keys from .env file
    ReadKeys reader;
    api_key = reader.getEnvKey(keysfile, api);
    secret_key = reader.getEnvKey(keysfile, secret);

    // Error optaining one or both keys
    if (api_key.length() <= 0 || secret_key.length() <= 0) {
        std::cerr << "Could not obtain keys " << api << " & " << secret << " from " << keysfile << std::endl;
    }
}

// Constructor takes api & secret key and initializes members 
HttpScaffold::HttpScaffold(const std::string api, const std::string secret) : 
    api_key(api), secret_key(secret)
{}

// Set the request method to POST
void HttpScaffold::setIsPost(bool isPost) {
    this->isPost = isPost;
}

// Set the headers to send in the request
void HttpScaffold::setHeaders(const std::map<std::string, std::string> &headers) {
    this->headers = headers;
}

// Set the query parameters to send in the request
void HttpScaffold::setQueryParams(const std::map<std::string, std::string> &queryParams) {
    this->queryParams = queryParams;
}

// Set the url for the request
void HttpScaffold::setUrl(const std::string &url) {
    this->url = url;
}

// Set the body for the request
void HttpScaffold::setBody(const std::string &body) {
    this->body = body;
}

void HttpScaffold::setUrl(const std::string &baseUrl, const std::map<std::string, std::string> &queryParams) {
    std::string url = baseUrl;
    if (!queryParams.empty()) {
        url += "?";
        bool first = true;
        for (const auto &param : queryParams) {
            if (!first) {
                url += "&";
            }
            url += param.first + "=" + param.second;
            first = false;
        }
    }
    this->url = url;
}

// Set the response headers and convert from http_headers to map
void HttpScaffold::setResponseHeaders(web::http::http_headers response_headers) {
    for (auto &header : response_headers) {
        responseHeaders.insert(std::make_pair(header.first, header.second));
    }
}

// Execute the request and return the response
std::string HttpScaffold::executeRequest() {

    // Create timestamp & add to params
    std::string timestamp = std::to_string(timestampEpoch_ms());
    queryParams.insert(std::make_pair("timestamp", timestamp));

    // Generate signature & add to params
    std::string signature = generateSignature(getQueryString(queryParams));
    queryParams.insert(std::make_pair("signature", signature));

    this->setUrl(url, queryParams);

    http_client client(this->url);

    http_request request;
    request.headers().add("X-MBX-APIKEY", this->api_key);

    for (const auto &header : this->headers) {
        request.headers().add(header.first, header.second);
    }

    if (this->isPost) {
        request.set_method(http::methods::POST);
        request.set_body(this->body);
    } else {
        request.set_method(http::methods::GET);
    }

    http_response response = client.request(request).get();

    // Store any response headers
    this->setResponseHeaders(response.headers());
    
    // Return the object "stringified" resp
    return response.extract_string().get();
}


// Get the current timestamp in milliseconds
long HttpScaffold::timestampEpoch_ms() {
    // Get current time with chrono library
    auto now = std::chrono::system_clock::now();

    // Cast to milliseconds
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();

    auto ret = value.count();

    return ret;
}

// Get the current timestamp in seconds
long HttpScaffold::timestampEpoch_s() {
    return timestampEpoch_ms() / 1000;
}

// Generate the query string for a GET request
std::string HttpScaffold::getQueryString(const std::map<std::string, std::string> &queryParams) {
    std::string queryString = "";
    bool first = true;
    for (const auto &param : queryParams) {
        if (!first) {
            queryString += "&";
        }
        queryString += param.first + "=" + param.second;
        first = false;
    }
    return queryString;
}

// Generate the signature for a request using the HMAC-SHA256 method
std::string HttpScaffold::generateSignature(const std::string &queryString) {
    GenerateSignature signatureGenerator;
    return signatureGenerator.hmacSha256(secret_key, queryString);
}

// Make a GET request to the specified url
std::string HttpScaffold::get(const std::string &url, const std::map<std::string, std::string> &queryParams, const std::map<std::string, std::string> &headers) {
    this->setUrl(url);
    this->setQueryParams(queryParams);
    this->setHeaders(headers);
    this->setIsPost(false);

    return this->executeRequest();
}

// Make a POST request to the specified url
std::string HttpScaffold::post(const std::string &url, const std::map<std::string, std::string> &queryParams, const std::string &body, const std::map<std::string, std::string> &headers) {
    this->setUrl(url);
    this->setQueryParams(queryParams);
    this->setHeaders(headers);
    this->setBody(body);
    this->setIsPost(true);

    return this->executeRequest();
}

std::map<std::string, std::string> HttpScaffold::getResponseHeaders() {
    return responseHeaders;
}



// OLD CURL METHOD

// std::string HttpScaffold::curlResponse(const std::string& url_payload, const std::string& signature, const bool& isPost) {
//     // Initialize curl
//     CURL* curl = curl_easy_init();
    
//     std::cout << url_payload << std::endl;

//     // Check if curl was successfully initialized
//     if (!curl) {
//         std::cerr << "Error initializing curl library" << std::endl;
//         return "";
//     }

//     // Set HTTP request method (post || get)
//     if (isPost) {
//         // Set the request method to POST
//         curl_easy_setopt(curl, CURLOPT_POST, 1L);
//     } else {
//         // Set the request method to GET
//         curl_easy_setopt(curl, CURLOPT_POST, 0L);
//     }

//     // Set the error buffer
//     char errorBuffer[CURL_ERROR_SIZE];
//     curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

//     // enable verbose output
//     //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    

//     struct curl_slist* headers = NULL;
//     //headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
//     headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + api_key).c_str());
//     //headers = curl_slist_append(headers, ("X-MBX-SIGNATURE: " + signature).c_str());


//     // Set the query parameters to send in the request
//     //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, getQueryparamsFromUrl(url_payload).c_str());

//     curl_easy_setopt(curl, CURLOPT_URL, url_payload.c_str());
//     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

//     std::string response;
//     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, defaultCallback);
//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

//     CURLcode res = curl_easy_perform(curl);

//     if (res != CURLE_OK) {
//         std::cout << errorBuffer << std::endl;
//         return "";
//     }

//     curl_easy_cleanup(curl);
//     curl_slist_free_all(headers);

//     std::cout << response << std::endl;

//     return response;
// }

// // Get the current timestamp in milliseconds
// long HttpScaffold::timestampEpoch_ms() {
//     // Get current time with chrono library
//     auto now = std::chrono::system_clock::now();

//     // Cast to milliseconds
//     auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
//     auto value = now_ms.time_since_epoch();

//     auto ret = value.count();

//     std::cout << "timestamp " << ret << std::endl;
//     std::cout << "timestamp " << "1673275084565" << std::endl;

//     return ret;
// }

// // Wrapper for curlResponse function of boilerplate code for curling a HTTP request
// // Requires url (only complete url) and structured parameters as query string
// // Returns string of JSON resposne
// std::string HttpScaffold::curlHttpRequest(std::string url, std::string total_params, const bool& isPost) {
//     // Get the current timestamp in milliseconds
//     long timestamp = timestampEpoch_ms();

    // // Add the timestamp parameter to the request parameters
    // // If "timestamp" is not the first param (and '&' not included already), include the '&'
    // if (total_params.size() > 0 && total_params.back() != '&' ) {
    //     total_params += '&';
    // }
    // total_params += ("timestamp=" + std::to_string(timestamp));

    // GenerateSignature sign;
    // const std::string signature = sign.hmacSha256(secret_key, total_params);
    // // std::cout << signature << std::endl;

//     // Append total_params to the URL as a query parameter + '?'
//     // Check if '?' already prepended to total_params
//     url += ('?' + total_params);
//     // Append signature to the URL as a query parameter
//     url += ("&signature=" + signature);

//     std::cout << "signature " << "8625254220fbda3e6382aa5470a790871ccf1869a99511bb205c8ee49de59698" << std::endl;
//     std::cout << "signature " << signature << std::endl;
    
//     return curlResponse(url, signature, isPost);
// }

// // Helper funtion to URL-encode query parameters (or url)
// // Returns encoded string
// std::string HttpScaffold::urlEncode(const std::string& input) {
//     CURL* curl = curl_easy_init();
//     if (curl) {
//         char* encoded = curl_easy_escape(curl, input.c_str(), input.size());
//         if (encoded) {
//             std::string output(encoded);
//             curl_free(encoded);
//             curl_easy_cleanup(curl);
//             return output;
//         }
//     }
//     return "";
// }



// // Returns substring of url payload given
// std::string HttpScaffold::getQueryparamsFromUrl(const std::string url_payload) {
    
//     size_t delim = url_payload.find_last_of('?');

//     // If '?' was not found return empty string
//     if (delim == std::string::npos) {
//         return "";
//     }

//     // Add 1 to exclude the '?' itself
//     delim ++;

//     std::cout << url_payload.substr(delim, url_payload.length()) << std::endl;
//     return url_payload.substr(delim, url_payload.length());
// }