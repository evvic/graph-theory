#ifndef C2C_BUY
#define C2C_BUY

#include <string>
#include <curl/curl.h>

class MarketBuyC2C {
 private:
    // Callback func for curl respinse
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    // Calculates the HMAC SHA256 request signature for the specified request path, request method, and request parameters.
    std::string CalculateSignature(const std::string& request_path, const std::string& request_method,
                                 const std::string& total_params);

    std::string secret_key_;
    std::string api_key_;

 public:
    // Constructor: optionally requires filename containing API and secret keys, variable names of API and secret in the file
    MarketBuyC2C(const std::string keysfile = ".env", const std::string api = "api_key", const std::string secret = "secret_key");

    // Makes a market buy request to the Binance API for the specified pair & amount.
    // Returns the raw response from the API.
    std::string marketBuy(const std::string& pair, double amount);

    // Makes a request to the Binance API to view the contents of your wallet.
    // Returns the raw response from the API.
    std::string viewWalletContents();
};

#endif