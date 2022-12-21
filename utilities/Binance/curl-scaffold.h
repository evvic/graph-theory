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

    // Helper funtion to URL-encode query parameters (or url)
    // Returns encoded string
    std::string urlEncode(const std::string& input);

    // Contains all boilerplate code for curling a HTTP request
    // Requires url (+total_params appended) and HMAC generated signature
    // Defaults bool isPost to false. True == POST, False == GET
    std::string curlResponse(const std::string& url, const std::string& signature, const bool& isPost = false);

    std::string getQueryparamsFromUrl(const std::string url_payload);

public:
    // Constructor: optionally requires filename containing API and secret keys, variable names of API and secret in the file
    CurlScaffold(const std::string keysfile = ".env", const std::string api = "api_key", const std::string secret = "secret_key");

    

    // Wrapper for curlResponse function of boilerplate code for curling a HTTP request
    // Requires url (only complete url) and structured parameters as query string
    // Returns string of JSON resposne
    // Defaults bool isPost to false. True == POST, False == GET
    std::string curlHttpRequest(std::string url, std::string total_params, const bool& isPost = false);

    // Get current timestamp in milliseconds
    long timestampEpoch_ms();

};

#endif