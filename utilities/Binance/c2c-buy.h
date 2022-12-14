#ifndef C2C_BUY
#define C2C_BUY

#include <string>
#include <curl/curl.h>

class MarketBuyC2C {
private:
    std::string api_key;
    std::string secret_key;

   // Calculates the HMAC SHA256 request signature for the specified request path, request method, and request parameters.
   std::string CalculateSignature(const std::string& request_path, const std::string& request_method,
                              const std::string& total_params);

 public:
   // Constructor: optionally requires filename containing API and secret keys, variable names of API and secret in the file
   MarketBuyC2C(const std::string keysfile = ".env", const std::string api = "api_key", const std::string secret = "secret_key");

   // Makes a market buy request to the Binance API for the specified pair & amount.
   // Returns the raw response from the API.
   std::string marketBuy(const std::string& pair, const double& amount);

   // Request a quote for the requested token pairs
   // Returns the raw response from the API. Response is the quited rate and window of time available
   std::string sendQuote(const std::string& fromAsset, const std::string& toAsset, const double& fromAmount);

   // Makes a request to the Binance API to view the contents of your wallet.
   // Returns the raw response from the API.
   std::string viewWalletContents();
};

#endif