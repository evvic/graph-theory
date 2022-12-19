#ifndef C2C_JSON
#define C2C_JSON

#include <string>
#include <vector>
#include <map>
#include "../c2c-edge.h"

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

    
    // Uses COUNTRY_CURRENCY_CODES to lint out 
    const std::string COUNTRY_CURRENCY_CODES[4] = {"GBP", "AUD", "JPY", "CNY"};
    bool isElligibleCountry(std::string);

    // Actually runs the API call to get all C2C pair info
    void populateEdges2(std::vector<C2CEdge>& edges);

    // API call to get all C2C pairs and their conversion rates
    void initRates2();


private:        

    std::string api_key;
    std::string secret_key;

    UniqueSymbols symbols;
    //ConversionRates rates;
    std::vector<C2CEdge> edges;

    std::map<std::string, double> rates_map; // SYMBOLPAIR, exchange rate

    // Pass the url and returns the json response <string>
    std::string json_request(const std::string &url);


public:
    // Constructor: optionally requires filename containing API and secret keys, variable names of API and secret in the file
    RequestC2C(const std::string keysfile = ".env", const std::string api = "api_key", const std::string secret = "secret_key");

    // pass vect of edges by reference
    void populateEdges(std::vector<C2CEdge>& edges3);

    // Retrurn the rate given the symbol pair i.e. "BTCUSDT" -> 17420.6
    bool getRateForSymbolPair(std::string pair, double& rate);

    // Returns the size of the rates_map datastructure
    int ratesSize();    

    // Returns number of vertices makng up the edges 
    unsigned short getVerticesCount();

    // Create Adjacency List from json request
    // NOT YET IMPLEMNETED
    //void readToAdjList(std::vector<std::vector<C2CEdge>>& graph);

    // In case rates has been set outside of the function, this can populate rates internally
    void setRates(std::map<std::string, double> r);
};


#endif