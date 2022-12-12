#include "c2c-edge.h"
#include <string>
#include <vector>
#include <iostream> //test
#include <iomanip>  // std::setw


using namespace std;

// Constructor
C2CEdge::C2CEdge(unsigned short _id, unsigned short _to, double _rate, std::string _fromAsset, std::string _toAsset)
    : id(_id), to(_to), rate(_rate), fromAsset(_fromAsset), toAsset(_toAsset) {}

// Empty Constructor
C2CEdge::C2CEdge() {
    id = 0;
    to = 0;
    rate = 0.0;
    fromAsset = "XXX";
    toAsset = "XXX";
}

ostream& operator << (ostream& o, C2CEdge& p)
{
    o << left << setw(10) << (p.fromAsset + "-" + p.toAsset) << setw(12) << right << p.id << "->" << p.to << setw(14) << right << p.rate;
    return o;
}