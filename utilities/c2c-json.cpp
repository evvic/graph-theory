#include "c2c-json.h"
#include "c2c-edge.h"
#include <json/json.h> // for parsing JSON
#include <curl/curl.h> // for making HTTP requests
#include <string>
#include <vector>
#include <iostream> //test

//using namespace std;


// Constructor runs all the functions to get the data
RequestC2C::RequestC2C() {
    
}

void RequestC2C::setRates(std::map<std::string, double> r) {
    rates_map = r;
}

// size_t RequestC2C::callbackExchange(void *contents, size_t size, size_t nmemb, void *userp) {
//     // Cast the userp pointer to a vector of edges
//     //std::vector<C2CEdge> *edges = static_cast<std::vector<C2CEdge>*>(userp);

//     // Cast the userp pointer to UniqueSymbols struct
//     //UniqueSymbols *symbols = static_cast<UniqueSymbols*>(userp);

//     // Cast the userp pointer to UniqueSymbols struct
//     //ConversionRates *rates = static_cast<ConversionRates*>(userp);

//     // Convert the void* userdata to a pointer to the class instance
//     RequestC2C *self = static_cast<RequestC2C*>(userp);

//     size_t dataSize = self->parseExchangeInfo(contents, size, nmemb);


//     return dataSize;
// }

// Custom callback function that concatenates the received data into a single string
static size_t concat_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::string *data = static_cast<std::string *>(userdata);
    size_t realsize = size * nmemb;
    data->append(ptr, realsize);
    return realsize;
}

size_t RequestC2C::callbackExchange(char *contents, size_t size, size_t nmemb, void *userp) {
    // Convert the user-defined data to a pointer to the RequestC2C object
    RequestC2C *request = static_cast<RequestC2C*>(userp);

    // Convert the response data to a string
    std::string responseData(static_cast<char*>(contents), size * nmemb);

    std::cout << responseData << std::endl;  

    // Create a JSON reader to parse the response
    Json::Reader reader;

    // Parse the JSON response
    Json::Value jsonResponse;
    if (reader.parse(responseData, jsonResponse))
    {
        // Get the list of edges from the response
        Json::Value edgesJson = jsonResponse["edges"];

        // Iterate over the list of edges
        for (const auto& item : jsonResponse)
        {

            // Create a C2CEdge struct from the edge data
            C2CEdge edge;
            edge.fromAsset = item["from"].asString();
            edge.toAsset = item["to"].asString();

            // Push the C2CEdge struct onto the vector of edges
            request->edges.push_back(edge);
        }
    } else {
        std::cerr << "Failed to parse JSON response: " << reader.getFormattedErrorMessages() << std::endl;
    }

    return size * nmemb;
}


size_t RequestC2C::parseExchangeInfo(void *contents, size_t size, size_t nmemb) {
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
            element.id = symbols.getSymbolID(element.fromAsset);

            // Get ID for toAsset symbol or create one if not one assigned yet
            element.to = symbols.getSymbolID(element.toAsset);

            // Append to vect
            edges.emplace_back(element);

            std::cout << "Added edge " << element << " to edges" << std::endl;

        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    //cout << "edges size after emplacing: " << edges.size() << endl;

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

        //std::cout << item << std::endl;

        try {
            _symbolpair.first = item["symbol"].asString();
            _symbolpair.second = std::stod(item["price"].asString());
            rates->insert(_symbolpair);
            
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            continue;
        }
    }

    // Return the size of the response data to indicate that it was processed successfully
    return dataSize;
}



void RequestC2C::populateEdges(std::vector<C2CEdge>& edges) {
    
    // API url
    const std::string url = "https://api.binance.com/sapi/v1/convert/exchangeInfo";

    std::vector<C2CEdge> edges2;

    // Set up a buffer to store the response from the API
    std::string response;

    // Reserve enough memory for the response
    response.reserve(4096); // reserve 4 KB of memory

    // Set the callback function and user data pointer


    // Set up the CURL request
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
        //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RequestC2C::callbackExchange);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, concat_callback); 
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        std::cout << response << std::endl;

        // Check for errors
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

    } else {
        std::cerr << "curl failed to initialize." << std::endl;
    }

    std::cout << "Cleaning up..." << std::endl;

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
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rates_map);

    // Perform the request
    curl_easy_perform(curl);

    // Print out the product pairs and their spot prices
    for (const auto& pair : rates_map)
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

    std::cout << "ratepair " << pair;

    // Check if symbol pair exists in rates map
    const std::map<std::string,double>::iterator it_rate = rates_map.find(std::string(pair));

    // If not in map, return false, set rate to 0
    //return (it_rate != rates_map.end())? true : false;

    if (it_rate == rates_map.end()) {
        std::cout << " is not in rates_map";
        rate = 0.0;
        return false;
    }
    // If in map, set ref var rate and return true
    else {
        
        rate = it_rate->second;
        
        std::cout << " is in rates_map with value " <<  rate << std::endl;
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

        std::cout << response << std::endl;

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
            std::cout << "Error: " << e.what() << std::endl;
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
        C2CEdge edge;

        try {
            edge.fromAsset = obj["fromAsset"].asString();
            edge.toAsset = obj["toAsset"].asString();

            edges3.push_back(edge);
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            continue;
        }
    }

}