// CREATE BINANCE CONVERT API CLASS

// 1. populate edges funvc
// 

#ifndef BINANCE_CONVERT
#define BINANCE_CONVERT

#include "../edges/unique-symbols.h"
#include "../edges/c2c-edge.h"
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

};

#endif