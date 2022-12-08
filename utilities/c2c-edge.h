#ifndef C2C_EDGE
#define C2C_EDGE

#include <string>

// Struct for crypto-to-crypto edges containing basic information

struct C2CEdge{
    // Variables
    unsigned short id;  // ID of base node
    unsigned short to;  // Pointing to ID of 'quote' node (coin)
    double rate;        // Ratio of converting from base coin to quote coin: -log(r)

    std::string baseAsset;  // Base coin symbol. i.e. BTC
    std::string toAsset;    // Quote coin symbol

    // Constructor
    C2CEdge(unsigned short _id, unsigned short _to, double _rate, std::string _baseAsset, std::string _toAsset);

    // Init empty (Constructor)
    C2CEdge();

};

#endif