#ifndef C2C_BUY
#define C2C_BUY

#include <string>

class MarketBuyC2C {
private:
    std::string api_key;
    std::string secret_key;

public:
    // Constructor: optionally requires filename containing API and secret keys, variable names of API and secret in the file
    MarketBuyC2C(const std::string keysfile = ".env", const std::string api = "api_key", const std::string secret = "secret_key");

    // Makes a market buy request to the Binance API for the specified pair & amount.
    // Returns the raw response from the API.
    std::string marketBuy(const std::string& pair, const double& amount);

    // Request a quote for the requested token pairs
    // Returns the raw response from the API. Response is the quited rate and window of time available
    //std::string sendQuote(const std::string& fromAsset, const std::string& toAsset, const double& fromAmount);

    // Makes a request to the Binance API to view the contents of your wallet.
    // Returns the raw response from the API.
    static std::string viewWalletContents();

    // Returns users (API key) wallet {"Currency_name" : amount} string, double
    // Of only coins that have some amount in them ( > 0)
    static std::map<std::string, double> getMappedWallet();
};

#endif