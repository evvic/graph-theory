#ifndef C2C_EDGE
#define C2C_EDGE

#include <string>

// Struct for crypto-to-crypto edges containing basic information

struct C2CEdge{
    // Variables
    unsigned short from;      // ID of base node (from)
    unsigned short to;      // Pointing to ID of 'quote' node (coin)
    double rate;            // Ratio of converting from base coin to quote coin 
    double weight;          // additive weight from base to quote asset: -log(rate)

    std::string fromAsset;  // Base coin symbol. i.e. BTC
    std::string toAsset;    // Quote coin symbol

    // Constructor
    C2CEdge(unsigned short _from, unsigned short _to, double _rate, std::string _fromAsset, std::string _toAsset);

    // Init empty (Constructor)
    C2CEdge();

};

// Operator Overloading
std::ostream& operator << (std::ostream& o, C2CEdge& p);

#endif