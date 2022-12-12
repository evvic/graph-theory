#include "c2c-json.h"
#include "c2c-edge.h"
#include <json/json.h> // for parsing JSON
#include <curl/curl.h> // for making HTTP requests
#include <string>
#include <vector>
#include <iostream> //test

using namespace std;

void RequestC2C::setRates(map<string, double> r) {
    rates = r;
}

size_t RequestC2C::callback(void *contents, size_t size, size_t nmemb, void *userp) {
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

            // If element exchange rate exists in symbols map, continue processing
            if (!findInSymbols(element.fromAsset + element.toAsset)) {
                std::cout << "Error: " << (element.fromAsset + element.toAsset) << " does not have an exchange rate." << std::endl;
                continue;
            }

            // Obtain exchange rate from symbol trading pair
            element.rate = getRateForSymbolPair(element.fromAsset + element.toAsset);

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