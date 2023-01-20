#ifndef QUOTE_EDGE
#define QUOTE_EDGE

#include <string>

// Struct to hold quote return object

struct QuoteEdge {
    // Symbols
    std::string toAsset;    // "USDT"
    std::string fromAsset;  // "BTC"
    
    // API response params 
    std::string quoteId;    // "12415572564",
    double ratio;           // "38163.7",
    double inverseRatio;    // "0.0000262",
    long validTimestamp;    // 1623319461670,
    double toAmount;        // "3816.37",
    double fromAmount;      // "0.1"

    // Default constructor
    QuoteEdge();

    // Constructor to init API response params
    QuoteEdge(std::string _quoteId, double _ratio, double _inverseRatio, long _validTimestamp, double _toAmount, double _fromAmount);

    //  Constructor to init all params
    QuoteEdge(std::string _toAsset, std::string _fromAsset, std::string _quoteId, double _ratio, double _inverseRatio, 
              long _validTimestamp, double _toAmount, double _fromAmount);

    
};

// Operator Overloading
std::ostream& operator << (std::ostream& o, const QuoteEdge& p);

#endif