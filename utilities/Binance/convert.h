#ifndef BINANCE_CONVERT
#define BINANCE_CONVERT

#include "../http/http-scaffold.h"
#include "../edges/unique-symbols.h"
#include "../edges/c2c-edge.h"
#include "quote-edge.h"
#include <string>


// Convert endpoints usage
class BinanceConvert {
private:
    UniqueSymbols symbols;

protected:

    // Uses COUNTRY_CURRENCY_CODES to lint out 
    const std::string COUNTRY_CURRENCY_CODES[4] = {"GBP", "AUD", "JPY", "CNY"};
    bool isElligibleCountry(std::string curr);

public:
    BinanceConvert();

    // Pass vect of edges by reference
    void populateEdges(std::vector<C2CEdge>& edges3);

    // Returns number of vertices makng up the edges 
    unsigned short getVerticesCount();

    // Independent function 
    static HttpScaffold sendQuote(const std::string& fromAsset, const std::string& toAsset, const double& fromAmount);

    // Wrapper for sendQuote to jsut return the conversion rate
    // Returns updated sapi UID weight as reference
    static QuoteEdge parseSendQuote(const std::string& fromAsset, const std::string& toAsset, const double& fromAmount, int& weight);
    
    // Accept a quote given for the token pairs
    // Returns the raw response from the API. Response is the quited rate and window of time
    static HttpScaffold acceptQuote(const QuoteEdge& quote);

    // Wrapper for acceptQuote to just return the order status
    // Returns updated sapi UID weight as reference
    static std::string parseAcceptQuote(const QuoteEdge& quote, int& weight);

};



#endif