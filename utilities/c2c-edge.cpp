#include "c2c-edge.h"
#include <string>
#include <vector>
#include <iostream> //test

using namespace std;

// Constructor
C2CEdge::C2CEdge(unsigned short _id, unsigned short _to, double _rate, std::string _baseAsset, std::string _toAsset)
    : id(id), to(_to), rate(_rate), baseAsset(_baseAsset), toAsset(_toAsset) {}

// Empty Constructor
C2CEdge::C2CEdge() {
    id = 0;
    to = 0;
    rate = 0.0;
    baseAsset = "XXX";
    toAsset = "XXX";
}

ostream& operator << (ostream& o, C2CEdge& p)
{
    o << p.baseAsset << "-" << p.toAsset << ", rate: " << p.rate;
    return o;
}