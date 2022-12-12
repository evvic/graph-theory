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
    // temporarily here with symbols
    unsigned short _id;
    std::map<std::string, int> symbols; // SYMBOL, ID

    std::map<std::string, double> rates; // SYMBOLPAIR, exchange rate

    std::vector<C2CEdge> edges;

    /* MEMBER FUNCTIONS */
    // Define a callback function that will be called by the curl library
    // to process the response data from the get exchange info API call
    static size_t callbackExchange(void *contents, size_t size, size_t nmemb, void *userp);

    // Define a callback function that will be called by the curl library
    // to process the response data from the ticker API call
    static size_t callbackRates(void *contents, size_t size, size_t nmemb, void *userp);

    // If currency symbol does not have an ID assigned create and return one
    // If currency symbol has an ID assigned, return it
    unsigned short getSymbolID(std::string sym);



public:
    // Constructor: requires csv filename
    RequestC2C();

    // pass vect of edges by reference
    void populateEdges(std::vector<C2CEdge>& edges);

    // Initialize
    void initRates();

    // Create Adjacency List from json request
    void readToAdjList(std::vector<std::vector<C2CEdge>>& graph);

    // SETTERS
    void setRates(std::map<std::string, double> r);

    // GETTERS
    bool getRateForSymbolPair(std::string pair, double& rate);

};


#endif