#ifndef C2C_EDGE
#define C2C_EDGE

#include <string>

// Struct for crypto-to-crypto edges containing basic information

struct C2CEdge{
    // Variables
    std::string fromAsset;  // Base coin symbol. i.e. BTC
    std::string toAsset;    // Quote coin symbol

    unsigned short from;    // ID of base node (coin)
    unsigned short to;      // ID of 'quote' node (coin)
    double rate;            // Ratio of converting from base coin to quote coin 
    double weight;          // Additive weight from base to quote asset: -log(rate)

    double fromAssetMinAmount;
    double fromAssetMaxAmount;
    double toAssetMinAmount;
    double toAssetMaxAmount;

    // Default constructor
    // Init empty
    C2CEdge();

    // Constructor
    C2CEdge(unsigned short _from, unsigned short _to, double _rate, std::string _fromAsset, std::string _toAsset);

    // Extended constructor
    C2CEdge(unsigned short _from, unsigned short _to, double _rate, std::string _fromAsset, std::string _toAsset,
            double _fromAssetMinAmount, double _fromAssetMaxAmount, double _toAssetMinAmount, double _toAssetMaxAmount);


    // Convert conversion rate to additive weight
    double rateToWeight(double r);

};

// Operator Overloading
std::ostream& operator << (std::ostream& o, const C2CEdge& p);

#endif