#include "convert.h"
#include "../http/http-scaffold.h"
#include "../edges/c2c-edge.h"
#include "api-limits.h"
#include "quote-edge.h"
#include <json/json.h> // for parsing JSON
#include <vector>
#include <map>
#include <iostream> // testing

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
HttpScaffold BinanceConvert::sendQuote(const std::string& fromAsset, const std::string& toAsset, const double& fromAmount) {

    // Object that handles http request
    HttpScaffold request;

    // Creates and validates a new quote
    std::string url = "https://api.binance.com/sapi/v1/convert/getQuote";

    // Query parameters (appended to url)
    std::map<std::string, std::string> params;
    params.insert(std::make_pair("fromAsset", fromAsset));
    params.insert(std::make_pair("toAsset", toAsset));
    params.insert(std::make_pair("fromAmount", std::to_string(fromAmount)));
    params.insert(std::make_pair("validTime", "2m"));   // Time the quote is valid for (2m is max)

    // Any message to attach to the body
    std::string body = "";

    // Additional header
    std::map<std::string, std::string> headers;

    request.post(url, params, body, headers);

    return request;
}

// Independant (static)
// Accept a quote given for the token pairs
// Returns the raw response from the API. Response is the quited rate and window of time
HttpScaffold BinanceConvert::acceptQuote(const QuoteEdge& quote) {

    // Object that handles http request
    HttpScaffold request;

    // Creates and validates a new quote
    std::string url = "https://api.binance.com/sapi/v1/convert/acceptQuote";

    // Query parameters (appended to url)
    std::map<std::string, std::string> params;
    params.insert(std::make_pair("quoteId", quote.quoteId));

    // Any message to attach to the body
    std::string body = "";

    // Additional header
    std::map<std::string, std::string> headers;

    request.post(url, params, body, headers);

    return request;

}

// Independant (static)
// Wrapper for sendQuote to jsut return the conversion rate
// Returns updated sapi UID weight as reference
QuoteEdge BinanceConvert::parseSendQuote(const std::string& fromAsset, const std::string& toAsset, const double& fromAmount, int& weight) {
    
    // Creates signature and performs all necessary http requests
    HttpScaffold httpResponse = sendQuote(fromAsset, toAsset, fromAmount);

    // update UID weight from given header
    weight = stoi(httpResponse.getHeader(LimitTracker::HEADER_SAPI_UID_1M_NAME));

    //httpResponse.printResponseHeader();
    std::cout << "weight = " << weight << std::endl;

    std::string response = httpResponse.getResponseString();

    // JSON parser
    Json::Value json_value;
    Json::Reader reader;

    // Check if can parse
    if (!reader.parse(response, json_value)) {
        std::cerr << "Error parsing JSON string!" << std::endl;
        return QuoteEdge();
    }

    // Check header for limit
    if (stoi(httpResponse.getHeader(LimitTracker::HEADER_SAPI_UID_1M_NAME)) >= LimitTracker::SAPI_UID_1M_WEIGHT_LIMIT) {
        std::cerr << "Reached " << LimitTracker::HEADER_SAPI_UID_1M_NAME << " limit." << std::endl;
    }

    try {
        QuoteEdge temp;

        // Check if quoteId is a member of the object
        // If not givenId, continue checking arbitrage but cannot execute the conversion
        if (json_value.isMember("quoteId"))
            temp.quoteId = json_value["quoteId"].asString();
        else
            temp.quoteId = "";

        // Parse JSON object into QuoteEdge strcut
        temp.ratio = stod(json_value["ratio"].asString());
        temp.inverseRatio = stod(json_value["inverseRatio"].asString());
        temp.validTimestamp = (long)json_value["validTimestamp"].asDouble();
        temp.toAmount = stod(json_value["toAmount"].asString());
        temp.fromAmount = stod(json_value["fromAmount"].asString());

        // Include non-mandatory symbols
        temp.toAsset = toAsset;
        temp.fromAsset = fromAsset;

        std::cout << temp << std::endl;

        return temp;

    } catch (std::exception& e) {
        // Handle exception

        std::cerr << "parseRefRate Error: " << e.what() << std::endl;
        //std::cout << json_value << std::endl;

        // Check if json value is a parseable error message
        if (!json_value.isMember("code")) {
            std::cerr << "Not a valid json object: " << json_value << std::endl;
        }
        int errCode = json_value["code"].asInt();
        std::string errMsg = json_value["msg"].asString();

        if (errCode == LimitTracker::ERR_HOUR_LIMIT_CODE) {
            // Your hourly quotation limit is reached. Please try again later in the next hour.
            std::cerr << errMsg << std::endl;
            LimitTracker::waitTillNextHour();
            
        } else if (errCode == LimitTracker::ERR_DAY_LIMIT_CODE) {
            // Your daily quotation limit is reached. Please try again later next day.
            std::cerr << errMsg << std::endl;
            LimitTracker::waitTillNextDay();
        } else {
            // Unhandled error message from Binance
            std::cerr << errMsg << std::endl;
        }
        
        return QuoteEdge();
    }
}

// Independant (static)
// Wrapper for sendQuote to jsut return the conversion rate
// Returns string orderStatus and UID weight as reference
std::string BinanceConvert::parseAcceptQuote(const QuoteEdge& quote, int& weight) {

    const std::string ORDER_ERROR = "ERROR";
    
    // Creates signature and performs all necessary http requests
    HttpScaffold httpResponse = acceptQuote(quote);

    // update UID weight from given header
    weight = stoi(httpResponse.getHeader(LimitTracker::HEADER_SAPI_UID_1M_NAME));

    //httpResponse.printResponseHeader();
    std::cout << "weight = " << weight << std::endl;

    std::string response = httpResponse.getResponseString();

    // JSON parser
    Json::Value json_value;
    Json::Reader reader;

    // Check if can parse
    if (!reader.parse(response, json_value)) {
        std::cerr << "Error parsing JSON string!" << std::endl;
        return ORDER_ERROR;
    }

    // Check header for limit
    if (stoi(httpResponse.getHeader(LimitTracker::HEADER_SAPI_UID_1M_NAME)) >= LimitTracker::SAPI_UID_1M_WEIGHT_LIMIT) {
        std::cerr << "Reached " << LimitTracker::HEADER_SAPI_UID_1M_NAME << " limit." << std::endl;
    }

    try {

        // Parse JSON object
        std::string orderId = json_value["orderId"].asString();         // "933256278426274426"
        std::string orderStatus = json_value["orderStatus"].asString(); // "PROCESS"
        long createTime = (long)json_value["createTime"].asDouble();    // 1623381330472

        return orderStatus;

    } catch (std::exception& e) {
        // Handle exception

        std::cerr << "parseRefRate Error: " << e.what() << std::endl;
        //std::cout << json_value << std::endl;

        // Check if json value is a parseable error message
        if (!json_value.isMember("code")) {
            std::cerr << "Not a valid json object: " << json_value << std::endl;
        }
        int errCode = json_value["code"].asInt();
        std::string errMsg = json_value["msg"].asString();

        if (errCode == LimitTracker::ERR_HOUR_LIMIT_CODE) {
            // Your hourly quotation limit is reached. Please try again later in the next hour.
            std::cerr << errMsg << std::endl;
            LimitTracker::waitTillNextHour();
            
        } else if (errCode == LimitTracker::ERR_DAY_LIMIT_CODE) {
            // Your daily quotation limit is reached. Please try again later next day.
            std::cerr << errMsg << std::endl;
            LimitTracker::waitTillNextDay();
        } else {
            // Unhandled error message from Binance
            std::cerr << errMsg << std::endl;
        }
        
        return ORDER_ERROR;
    }
}

