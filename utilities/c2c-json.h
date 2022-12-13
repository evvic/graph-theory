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
protected:

    // Struct for tracking each symbol and giving it a unique ID
    struct UniqueSymbols {
        unsigned short _id;
        std::map<std::string, int> symbols; // SYMBOL, ID

        // Constructor, init ID
        UniqueSymbols();

        // If currency symbol does not have an ID assigned create and return one
        // If currency symbol has an ID assigned, return it
        unsigned short getSymbolID(std::string sym);
    };

    // Struct for holding map of symbol pairs to their rates and returning them
    // struct ConversionRates {
    //     std::map<std::string, double> rates_map; // SYMBOLPAIR, exchange rate

    //     // GETTERS
    //     bool getRateForSymbolPair(std::string pair, double& rate);
    // };

private:        

    std::string api_key;
    std::string secret_key;

    UniqueSymbols symbols;
    //ConversionRates rates;
    std::vector<C2CEdge> edges;

    std::map<std::string, double> rates_map; // SYMBOLPAIR, exchange rate

    /* MEMBER FUNCTIONS */
    // Define a callback function that will be called by the curl library
    // to process the response data from the get exchange info API call
    static size_t callbackExchange(char *contents, size_t size, size_t nmemb, void *userp);

    // Define a callback function that will be called by the curl library
    // to process the response data from the ticker API call
    static size_t callbackRates(void *contents, size_t size, size_t nmemb, void *userp);

    // Pass the url and returns the json response <string>
    std::string json_request(const std::string &url);


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

    size_t parseExchangeInfo(void *contents, size_t size, size_t nmemb);   

    bool getRateForSymbolPair(std::string pair, double& rate);

    int ratesSize();

    // Version 2
    void populateEdges2(std::vector<C2CEdge>& edges);

    void initRates2();
   

};


#endif