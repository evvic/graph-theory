#include "c2c-json.h"
#include "../c2c-edge.h"
#include "../read-keys.h" // Reading in API keys
#include <json/json.h> // for parsing JSON
#include <curl/curl.h> // for making HTTP requests
#include <string>
#include <vector>
#include <iostream> //test

//////////////////////////
// DEPRECATE THIS CLASS //
//////////////////////////

// Constructor runs all the functions to get the data
// Optional paramters: 
//      keysfile: path to file containing api and secret keys
//      api: variable name of api key within specified file
//      secret: variable name of secret key within specified file
RequestC2C::RequestC2C(const std::string keysfile, const std::string api, const std::string secret) {

    // Obtain API keys from .env file
    ReadKeys reader;
    api_key = reader.getEnvKey(keysfile, api);
    secret_key = reader.getEnvKey(keysfile, secret);

    // Error optaining one or both keys
    if (api_key.length() <= 0 || secret_key.length() <= 0) {
        std::cerr << "Could not obtain keys for private client API access." << std::endl;
    }

    // ...    
}


void RequestC2C::setRates(std::map<std::string, double> r) {
    rates_map = r;
}

// Custom callback function that concatenates the received data into a single string
static size_t concat_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::string *data = static_cast<std::string *>(userdata);
    size_t realsize = size * nmemb;
    data->append(ptr, realsize);
    return realsize;
}


// helpers

// returns bool of whether function contains symbol pair in map
// double passed by reference is how the rate is returned
bool RequestC2C::getRateForSymbolPair(std::string pair, double& rate) {

    //std::cout << "ratepair " << pair;

    // Check if symbol pair exists in rates map
    const std::map<std::string,double>::iterator it_rate = rates_map.find(std::string(pair));

    // If not in map, return false, set rate to 0
    //return (it_rate != rates_map.end())? true : false;

    if (it_rate == rates_map.end()) {
        //std::cout << " is not in rates_map";
        rate = 0.0;
        return false;
    }
    // If in map, set ref var rate and return true
    else {
        
        rate = it_rate->second;
        
        //std::cout << " is in rates_map with value " <<  rate << std::endl;
        return true;
    }
}

/* STRUCT UniqueSymbols */

// Constructor
RequestC2C::UniqueSymbols::UniqueSymbols() {
    _id = 0; // init id to 0
}

unsigned short RequestC2C::UniqueSymbols::getSymbolID(std::string sym) {
    // Check if baseAsset symbol has an ID already
    const std::map<std::string,int>::iterator it_base = symbols.find(std::string(sym));

    // If cannot find in map, add it with a unique ID
    if (it_base == symbols.end()) {

        // Insert new pair to map
        symbols.insert(std::make_pair(std::string(sym), _id));

        const unsigned short tmp = _id;

        _id++; // increment _id

        return tmp;
    }
    // If in map already, get ID from symbol
    else {

        return it_base->second;
    }
}

int RequestC2C::ratesSize() {
    return rates_map.size();
}

///// Version 2

std::string RequestC2C::json_request(const std::string &url)
{

    // Set the custom callback function
    std::string response;
    // Reserve enough memory for the response
    response.reserve(4096); // reserve 4 KB of memory

    CURL *curl = curl_easy_init();
    if (curl)
    {
        // Set the URL for the request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the request to use HTTP GET method
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

        // Set the API key and secret in the request headers
        struct curl_slist *headers = NULL;
        std::string apiKeyHeader = "X-MBX-APIKEY: " + api_key;
        headers = curl_slist_append(headers, apiKeyHeader.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set a callback function to handle the response data 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, concat_callback); 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        //std::cout << response << std::endl;

        // Check for errors
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

    } else {
        std::cerr << "curl failed to initialize." << std::endl;
    }

    // Clean up
    curl_easy_cleanup(curl);

    return response;
}

void RequestC2C::initRates2() {
    std::string response = json_request("https://www.binance.com/api/v3/ticker/price");

    // Parse the JSON response and store the product pairs and spot prices in the rates_map
    Json::Value json_value;
    Json::Reader reader;

    if (!reader.parse(response, json_value)) {
        std::cerr << "Error parsing JSON string!" << std::endl;
        return;
    }

    for (const auto& item : json_value)
    {
        std::pair<std::string, double> _symbolpair;

        try {
            _symbolpair.first = item["symbol"].asString();
            _symbolpair.second = std::stod(item["price"].asString());

            rates_map.insert(_symbolpair);
            
        } catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            continue;
        }
    }


}


void RequestC2C::populateEdges2(std::vector<C2CEdge>& edges3) {
    // populate rates_map

    // 
    std::string response = json_request("https://api.binance.com/sapi/v1/convert/exchangeInfo");

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
            if (edge.fromAsset == "DAI" || edge.toAsset == "DAI") {
                std::cout << "Ignoring symbol pair " << edge.fromAsset << '-' << edge.toAsset << std::endl;
                continue;
            }

            
            // If element exchange rate exists in symbols map, continue processing
            if (!getRateForSymbolPair((edge.fromAsset + edge.toAsset), _rate)) {
                //std::cerr << "Error: " << (edge.fromAsset + edge.toAsset) << " does not have an exchange rate." << std::endl;
                continue;
            }

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

bool RequestC2C::isElligibleCountry(std::string curr) {
    for (auto str : COUNTRY_CURRENCY_CODES) {
        if (str == curr) return false;
    }
    return true;
}

unsigned short RequestC2C::getVerticesCount() {
    return symbols._id;
}

// Combines other function calls to make it simple for implementing
// Returns vactor of all feasable edges
void RequestC2C::populateEdges(std::vector<C2CEdge>& edges3) {

    // Make API call to receive all C2C pairs and their rates
    initRates2();

    populateEdges2(edges3);
}