#ifndef QUOTE_EDGE
#define QUOTE_EDGE

#include <string>

// Struct to hold quote return object

struct QuoteEdge {
    // 
    
    std::string quoteId;    // "12415572564",
    double ratio;           // "38163.7",
    double inverseRatio;    // "0.0000262",
    long validTimestamp;    // 1623319461670,
    double toAmount;        // "3816.37",
    double fromAmount;      // "0.1"

    // Constructor to init all params
    QuoteEdge(std::string _quoteId, double _ratio, double _inverseRatio, long _validTimestamp, double _toAmount, double _fromAmount);

    // Default constructor
    QuoteEdge();
};

// Operator Overloading
std::ostream& operator << (std::ostream& o, const QuoteEdge& p);

#endif