#include "convert.h"
#include "../http/http-scaffold.h"
#include "../edges/c2c-edge.h"
#include "quote-edge.h"
#include <json/json.h> // for parsing JSON
#include <vector>
#include <map>
#include <iostream> // testing


#include <thread>   // thread sleeping
#include <chrono>   // thread sleeping
#include <iomanip>  // cout time

BinanceConvert::BinanceConvert() {}

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

            // Conversion limits
            edge.fromAssetMinAmount = stod(obj["fromAssetMinAmount"].asString());
            edge.fromAssetMaxAmount = stod(obj["fromAssetMaxAmount"].asString());
            edge.toAssetMinAmount = stod(obj["toAssetMinAmount"].asString());
            edge.toAssetMaxAmount = stod(obj["toAssetMaxAmount"].asString());

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

            // std::cout << edge.fromAsset << ": fromAssetMinAmount " << edge.fromAssetMinAmount << std::endl;

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

// Independant (static)
// Wrapper for sendQuote to jsut return the conversion rate
QuoteEdge BinanceConvert::parseSendQuote(const std::string& fromAsset, const std::string& toAsset, const double& fromAmount) {
    
    std::cout << "fromAmount = " << fromAmount << std::endl;

    // Creates signature and performs all necessary http requests
    std::string response = sendQuote(fromAsset, toAsset, fromAmount);

    // JSON parser
    Json::Value json_value;
    Json::Reader reader;

    if (!reader.parse(response, json_value)) {
        std::cerr << "Error parsing JSON string!" << std::endl;
        return QuoteEdge();
    }

    try {
        QuoteEdge temp;

        // Check if quoteId is a member of the object
        if (json_value.isMember("quoteId"))
            temp.quoteId = stod(json_value["quoteId"].asString());
        else
            temp.quoteId = 0;

        temp.ratio = stod(json_value["ratio"].asString());
        temp.inverseRatio = stod(json_value["inverseRatio"].asString());
        temp.validTimestamp = (long)json_value["validTimestamp"].asDouble();
        temp.toAmount = stod(json_value["toAmount"].asString());
        temp.fromAmount = stod(json_value["fromAmount"].asString());

        std::cout << temp << std::endl;

        return temp;

    } catch (std::exception& e) {
        std::cerr << "parseRefRate Error: " << e.what() << std::endl;

        // Handle exception
        int errCode = json_value["code"].asInt();

        if (errCode == 345103) {
            // Your hourly quotation limit is reached. Please try again later in the next hour.
            // Get the current time
            auto now = std::chrono::system_clock::now();

            // Get the time point representing the next hour
            auto next_hour = std::chrono::time_point_cast<std::chrono::hours>(now) + std::chrono::hours(1);

            // Calculate the duration until the next hour
            auto duration = next_hour - now;

            // Print the duration until the next hour
            std::cout << "Sleeping for " << duration.count() << " seconds" << std::endl;

            // Put the thread to sleep until the next hour
            std::this_thread::sleep_until(next_hour);

            auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << "Woken up! Time: " <<std::put_time(std::localtime(&now_time), "%F %T") << std::endl;
            
        } else if (errCode == 345239) {
            // Your daily quotation limit is reached. Please try again later next day.
            // Get the current time
            auto now = std::chrono::system_clock::now();

            // Get the time point representing the next day
            auto next_day_seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::hours(24));
            auto next_day = now + next_day_seconds;
            next_day = std::chrono::time_point_cast<std::chrono::seconds>(next_day);

            // Calculate the duration until the next day
            auto duration = next_day - now;

            // Print the duration until the next day
            std::cout << "Sleeping for " << duration.count() << " seconds" << std::endl;

            // Put the thread to sleep until the next day
            std::this_thread::sleep_until(next_day);

            auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << "Woken up! Time: " <<std::put_time(std::localtime(&now_time), "%F %T") << std::endl;
        } else {
            std::cerr << response << std::endl;
        }

        
        return QuoteEdge();
    }
}