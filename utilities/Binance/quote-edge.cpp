#include "quote-edge.h"
#include <string>
#include <iomanip>  // std::setw

QuoteEdge::QuoteEdge(double _quoteId, double _ratio, double _inverseRatio, long _validTimestamp, double _toAmount, double _fromAmount)
    : quoteId(_quoteId), ratio(_ratio), inverseRatio(_inverseRatio), validTimestamp(_validTimestamp),
    toAmount(_toAmount), fromAmount(_fromAmount) {}

QuoteEdge::QuoteEdge() {
    quoteId = 0.0000;
    ratio = 0;
    inverseRatio = 0;
    validTimestamp = 0.0;
    toAmount = 0;
    fromAmount = 0;
}

using namespace std;

ostream& operator << (ostream& o, const QuoteEdge& p)
{
    o << left << setw(10) << (to_string(p.fromAmount) + "->" + to_string(p.toAmount)) << setw(12) << right << p.ratio;
    return o;
}