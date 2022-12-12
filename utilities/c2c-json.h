#ifndef C2C_JSON
#define C2C_JSON

#include <string>
#include <vector>
#include <map>
#include "c2c-edge.h"

// Class for reading/writing crypto-to-crypto JSON!
// Parse json format into edges of a graph to be used
// for Graph Theory

class RequestC2C {
private:
    std::string fname;
    std::map<std::string, int> symbols; // SYMBOL, ID
    std::map<std::string, double> rates; // SYMBOLPAIR, exchange rate

    std::vector<C2CEdge> edges;

    /* MEMBER FUNCTIONS */
    // Define a callback function that will be called by the curl library
    // to process the response data from the API
    size_t callback(void *contents, size_t size, size_t nmemb, void *userp);

    // Checks symbols map if symbols pair exists
    // Relevant if exchange rate is needed
    bool findInSymbols(std::string pair);

    // If currency symbol does not have an ID assigned create and return one
    // If currency symbol has an ID assigned, return it
    unsigned short getSymbolID(std::string sym);



public:
    // Constructor: requires csv filename
    RequestC2C(std::string _fname);

    // pass vect of edges by reference
    void readPairsToEdge(std::vector<C2CEdge>& edges);

    // Create Adjacency List from json request
    void readToAdjList(std::vector<std::vector<C2CEdge>>& graph);

    // SETTERS
    void setRates(std::map<std::string, double> r);

    // GETTERS
    double getRateForSymbolPair(std::string pair);

};


#endif