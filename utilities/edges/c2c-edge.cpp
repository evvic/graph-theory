#include "c2c-edge.h"
#include <string>
#include <vector>
//#include <iostream> //test
#include <iomanip>  // std::setw

using namespace std;

// Constructor
// weight is the additive inverse of rate, therefore weight can jsut be calculated by giving the rate parameter
C2CEdge::C2CEdge(unsigned short _from, unsigned short _to, double _rate, std::string _fromAsset, std::string _toAsset)
    : from(_from), to(_to), rate(_rate), weight(rateToWeight(_rate)), fromAsset(_fromAsset), toAsset(_toAsset) {}

// Empty Constructor
C2CEdge::C2CEdge() {
    from = 0;
    to = 0;
    rate = 1.0;
    weight = rateToWeight(1.0);
    fromAsset = "XXX";
    toAsset = "XXX";

    fromAssetMinAmount = 0;
    fromAssetMaxAmount = 0;
    toAssetMinAmount = 0;
    toAssetMaxAmount = 0;
}

// Extended constructor
C2CEdge::C2CEdge(unsigned short _from, unsigned short _to, double _rate, std::string _fromAsset, std::string _toAsset,
                double _fromAssetMinAmount, double _fromAssetMaxAmount, double _toAssetMinAmount, double _toAssetMaxAmount)
                : from(_from), to(_to), rate(_rate), weight(rateToWeight(_rate)), fromAsset(_fromAsset), toAsset(_toAsset),
                fromAssetMinAmount(_fromAssetMinAmount), fromAssetMaxAmount(_fromAssetMaxAmount), 
                toAssetMinAmount(_toAssetMinAmount), toAssetMaxAmount(_toAssetMaxAmount) {}


double C2CEdge::rateToWeight(double r) {
    return -1.0 / r;
}

ostream& operator << (ostream& o, const C2CEdge& p)
{
    o << left << setw(10) << (p.fromAsset + "-" + p.toAsset) << setw(12) << right << p.from << "->" << p.to << setw(14) << right << p.rate;
    return o;
}