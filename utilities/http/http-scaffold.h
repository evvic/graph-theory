#ifndef HTTP_SCAFFOLD
#define HTTP_SCAFFOLD

#include <string>
#include <map>

class HttpScaffold {
protected:
    std::string api_key;    // Mandatory keys
    std::string secret_key; // Mandatory keys

    bool isPost;            // Flag for POST or GET

    // http variables
    std::string url; 
    std::string body;
    std::map<std::string, std::string> headers; 
    std::map<std::string, std::string> queryParams;
    

    // Execute the request and return the response (boiler plate)
    std::string executeRequest();

    // Returns HMAC Sha256 signature given query params (must include timestamp)
    // Requires secret_key member variable
    std::string generateSignature(const std::string &queryString);

public:
    // Default Constructor: optionally requires filename containing API and secret keys, variable names of API and secret in the file
    HttpScaffold(const std::string keysfile = ".env", const std::string api = "api_key", const std::string secret = "secret_key");
    
    // Constructor takes api & secret key and initializes members 
    HttpScaffold(const std::string api, const std::string secret);

    // Wrapper for executeRequest function of boilerplate code for a rest HTTP request
    // Requires url (only complete url), map of query parameters, & map of headers
    // Returns string of JSON resposne
    std::string get(const std::string &url, const std::map<std::string, std::string> &queryParams, const std::map<std::string, std::string> &headers);
    std::string post(const std::string &url, const std::map<std::string, std::string> &queryParams, const std::string &body, const std::map<std::string, std::string> &headers);
    
    // HELPERS
    long timestampEpoch_ms();   // Get current timestamp in milliseconds
    long timestampEpoch_s();    // Get current timestamp in seconds
    std::string getQueryString(const std::map<std::string, std::string> &queryParams); // Generate the query string for a request

    // SETTERS
    void setIsPost(bool isPost);
    void setHeaders(const std::map<std::string, std::string> &headers);
    void setQueryParams(const std::map<std::string, std::string> &queryParams);
    void setUrl(const std::string &url);
    void setUrl(const std::string &baseUrl, const std::map<std::string, std::string> &queryParams);
    void setBody(const std::string &body);

};

#endif