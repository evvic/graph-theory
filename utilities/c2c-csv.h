#ifndef C2C_CSV
#define C2C_CSV

#include <string>
#include <vector>
#include "c2c-edge.h"

// Class for reading/writing crypto-to-crypto csvs!
// Parse csv format into edges of a graph to be used
// for Graph Theory

class ParseC2C {
private:
    std::string fname;

public:
    // Constructor: requires csv filename
    ParseC2C(std::string _fname);

    // pass vect of edges by reference
    void readPairsToEdge(std::vector<C2CEdge>& edges);

};


#endif