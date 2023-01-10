#include "convert.h"
#include "../http/http-scaffold.h"
#include "../edges/c2c-edge.h"
#include <json/json.h> // for parsing JSON
#include <vector>
#include <map>
#include <iostream> // testing

BinanceConvert::BinanceConvert() {

}

void BinanceConvert::populateEdges(std::vector<C2CEdge>& edges3) {
    // Object that handles http request
    HttpScaffold request;

    // url for convert API to get all pairs
    const std::string url = "https://api.binance.com/sapi/v1/convert/exchangeInfo";
    
    std::map<std::string, std::string> params;  // Query parameters (appended to url)    
    std::map<std::string, std::string> headers; // Additional header

    // Creates signature and performs all necessary http requests
    std::string response = request.get(url, params, headers);

    // JSON parser
    Json::Value json_value;
    Json::Reader reader;

    if (!reader.parse(response, json_value)) {
        std::cerr << "Error parsing JSON string!" << std::endl;
        return;
    }

    // Iterate through the JSON object and extract the data for each struct
    for (const auto &obj : json_value)
    {
        C2CEdge edge;       // init temp edge struct to append to vect
        double _rate = 0.0; // init temp rate var to pass as reference

        try {
            // Set values from json obj
            edge.fromAsset = obj["fromAsset"].asString();
            edge.toAsset = obj["toAsset"].asString();

            // Temporarily remove DAI as a tradeable edge
            // if (edge.fromAsset == "DAI" || edge.toAsset == "DAI") {
            //     std::cout << "Ignoring symbol pair " << edge.fromAsset << '-' << edge.toAsset << std::endl;
            //     continue;
            // }

            // CHANGE TO JUST CREATE UNIQUE SYMBOL ID
            // If element exchange rate exists in symbols map, continue processing
            // if (!getRateForSymbolPair((edge.fromAsset + edge.toAsset), _rate)) {
            //     //std::cerr << "Error: " << (edge.fromAsset + edge.toAsset) << " does not have an exchange rate." << std::endl;
            //     continue;
            // }

            // Set rate from passed previous function 
            edge.rate = _rate;

            // Lint out country currencies (i.e. "GBP")
            if (!isElligibleCountry(edge.fromAsset) || !isElligibleCountry(edge.toAsset)) {
                //std::cerr << "Warning. " << (edge.fromAsset + '-' + edge.toAsset) << " contains an unuseable country code." << std::endl;
                continue;
            }

            // Get ID for fromAsset symbol or create one if not one assigned yet
            edge.from = symbols.getSymbolID(edge.fromAsset);

            // Get ID for toAsset symbol or create one if not one assigned yet
            edge.to = symbols.getSymbolID(edge.toAsset);

            edges3.push_back(edge);
        } catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            continue;
        }
    }

}

bool BinanceConvert::isElligibleCountry(std::string curr) {
    for (auto str : COUNTRY_CURRENCY_CODES) {
        if (str == curr) return false;
    }
    return true;
}

unsigned short BinanceConvert::getVerticesCount() {
    return symbols._id;
}

// Independant (static)
// Request a quote for the requested token pairs
// Returns the raw response from the API. Response is the quited rate and window of time
std::string BinanceConvert::sendQuote(const std::string& fromAsset, const std::string& toAsset, const double& fromAmount) {

    // Object that handles http request
    HttpScaffold request;

    // Creates and validates a new quote
    std::string url = "https://api.binance.com/sapi/v1/convert/getQuote";

    // Query parameters (appended to url)
    std::map<std::string, std::string> params;
    params.insert(std::make_pair("fromAsset", fromAsset));
    params.insert(std::make_pair("toAsset", toAsset));
    params.insert(std::make_pair("fromAmount", std::to_string(fromAmount)));

    // Any message to attach to the body
    std::string body = "";

    // Additional header
    std::map<std::string, std::string> headers;

    return request.post(url, params, body, headers);
}