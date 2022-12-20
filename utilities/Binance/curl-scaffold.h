#ifndef CURL_SCAFFOLD
#define CURL_SCAFFOLD

#include <string>

// Think about renaming file & class

class CurlScaffold {
protected:
    std::string api_key;
    std::string secret_key;

    // Callback func for curl respinse
    static size_t defaultCallback(void* contents, size_t size, size_t nmemb, void* userp);

public:
    // Constructor: optionally requires filename containing API and secret keys, variable names of API and secret in the file
    CurlScaffold(const std::string keysfile = ".env", const std::string api = "api_key", const std::string secret = "secret_key");

    // Contains all boilerplate code for curling a HTTP request
    // Requires url (+total_params appended) and HMAC generated signature
    std::string curlResponse(const std::string& url, const std::string& signature);

    // Get current timestamp in milliseconds
    long timestampEpoch_ms();

};

#endif