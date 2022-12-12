#include "c2c-json.h"
#include "c2c-edge.h"
#include <json/json.h> // for parsing JSON
#include <curl/curl.h> // for making HTTP requests
#include <string>
#include <vector>
#include <iostream> //test

using namespace std;


// Constructor runs all the functions to get the data
RequestC2C::RequestC2C() {
    _id = 0; // init id to 0
}

void RequestC2C::setRates(map<string, double> r) {
    rates = r;
}

size_t RequestC2C::callbackExchange(void *contents, size_t size, size_t nmemb, void *userp) {
    // Cast the userp pointer to a vector of edges
    std::vector<C2CEdge> *edges = static_cast<std::vector<C2CEdge>*>(userp);

    // Convert the void pointer to a char pointer and calculate the size
    // of the response data
    char *data = static_cast<char*>(contents);
    size_t dataSize = size * nmemb;

    // Parse the JSON response and store the product pairs and spot prices in the vector
    Json::Value jsonResponse;
    Json::Reader reader;
    reader.parse(data, data + dataSize, jsonResponse, false);

    for (const auto& item : jsonResponse)
    {
        // init empty C2CEdge struct
        C2CEdge element;

        std::cout << item << std::endl;

        try {
            // Collect relevant data from json object
            element.fromAsset = item["fromAsset"].asString();
            element.toAsset = item["toAsset"].asString();
            // ...other data can be parsed in the future...

            // init temp rate var to pass as reference
            double _rate = 0.0;
            // If element exchange rate exists in symbols map, continue processing
            if (!getRateForSymbolPair((element.fromAsset + element.toAsset), _rate)) {
                std::cout << "Error: " << (element.fromAsset + element.toAsset) << " does not have an exchange rate." << std::endl;
                continue;
            }

            // Set rate from passed previous function 
            element.rate = _rate;

            // Get ID for fromAsset symbol or create one if not one assigned yet
            element.id = getSymbolID(element.fromAsset);

            // Get ID for toAsset symbol or create one if not one assigned yet
            element.to = getSymbolID(element.toAsset);

            // Append to vect
            edges->emplace_back(element);

        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    // Return the size of the response data to indicate that it was processed successfully
    return dataSize;
}

size_t RequestC2C::callbackRates(void *contents, size_t size, size_t nmemb, void *userp)
{

    // Cast the userp pointer to a vector of Pair structs
    std::map<std::string, double> *rates = static_cast<std::map<std::string, double>*>(userp);

    // Convert the void pointer to a char pointer and calculate the size
    // of the response data
    char *data = static_cast<char*>(contents);
    size_t dataSize = size * nmemb;

    // Parse the JSON response and store the product pairs and spot prices in the vector
    Json::Value jsonResponse;
    Json::Reader reader;
    reader.parse(data, data + dataSize, jsonResponse, false);

    for (const auto& item : jsonResponse)
    {
        std::pair<std::string, double> _symbolpair;

        std::cout << item << std::endl;

        try {
            _symbolpair.first = item["symbol"].asString();
            _symbolpair.second = std::stod(item["price"].asString());
            rates->insert(_symbolpair);
            
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    // Return the size of the response data to indicate that it was processed successfully
    return dataSize;
}

void RequestC2C::populateEdges(vector<C2CEdge>& edges) {
    // Set up the CURL request
    CURL *curl = curl_easy_init();
    const std::string url = "https://api.binance.com/sapi/v1/convert/exchangeInfo";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set up a buffer to store the response from the API
    std::string response;

    // Reserve enough memory for the response
    response.reserve(4096); // reserve 4 KB of memory

    // Set the callback function and user data pointer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackExchange);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &edges);

    // Perform the request
    curl_easy_perform(curl);

    // Print out the edges and their spot prices
    for (const auto& edge : edges)
    {
      std::cout << edge.fromAsset << "->" << edge.toAsset << edge.rate << std::endl;
    }

    // Clean up the CURL pointer
    curl_easy_cleanup(curl);

}

void RequestC2C::initRates() {
    // Set up the CURL request
    CURL *curl = curl_easy_init();
    const std::string url = "https://www.binance.com/api/v3/ticker/price";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set up a buffer to store the response from the API
    std::string response;

    // Reserve enough memory for the response
    response.reserve(4096); // reserve 4 KB of memory

    // Set the callback function and user data pointer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackRates);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rates);

    // Perform the request
    curl_easy_perform(curl);

    // Print out the product pairs and their spot prices
    for (const auto& pair : rates)
    {
      std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Clean up the CURL pointer
    curl_easy_cleanup(curl);

}

// helpers

// returns bool of whether function contains symbol pair in map
// double passed by reference is how the rate is returned
bool RequestC2C::getRateForSymbolPair(std::string pair, double& rate) {

    // Check if symbol pair exists in rates map
    const map<string,double>::iterator it_rate = rates.find(string(pair));

    // If not in map, return false, set rate to 0
    return (it_rate != rates.end())? true : false;

    if (it_rate == rates.end()) {
        rate = 0.0;
        return false;
    }
    // If in map, set ref var rate and return true
    else {
        rate = it_rate->second;
        return true;
    }
}

unsigned short RequestC2C::getSymbolID(std::string sym) {
    // Check if baseAsset symbol has an ID already
    const map<string,int>::iterator it_base = symbols.find(string(sym));

    // If cannot find in map, add it with a unique ID
    if (it_base == symbols.end()) {

        // Insert new pair to map
        symbols.insert(make_pair(string(sym), _id));
        _id++; // increment _id
    }
    // If in map already, get ID from symbol
    else {

        return it_base->second;
    }
}