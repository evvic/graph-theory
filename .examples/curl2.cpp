#include <iostream>
#include <string>
#include <unordered_map>
#include <curl/curl.h>
#include <json/json.h>

using namespace std;

// Struct to store the trading pair and spot price
struct TradingPairPrice {
    string symbol;
    double price;
};

// Function to handle the response data from the API
size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string* response = (string*) userdata;
    response->append(ptr, size * nmemb);

    // Parse the response data as JSON
    Json::Value root;
    Json::Reader reader;
    bool success = reader.parse(*response, root);
    if (!success) {
        cerr << "Failed to parse JSON: " << reader.getFormattedErrorMessages() << endl;
        return 0;
    }

    // Create a vector to store the trading pair and spot price data
    vector<TradingPairPrice> data;

    // Loop through the array of trading pairs and spot prices
    for (const auto& element : root) {
        TradingPairPrice tradingPairPrice;
        cout << element << endl;
        tradingPairPrice.symbol = element["symbol"].asString();
        tradingPairPrice.price = element["price"].asDouble();
        data.push_back(tradingPairPrice);
    }

    return size * nmemb;
}


int main()
{
  // Set up the request
  CURL *curl = curl_easy_init();
  std::string url = "https://api.binance.com/api/v3/ticker/bookTicker";
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

  cout << "Set up response" << endl;

  // Set up the response
  std::string response;
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  // Make the request
  CURLcode res = curl_easy_perform(curl);

  // Check for errors
  if (res != CURLE_OK)
  {
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    return 1;
  }

  // Parse the response and store it in a data structure
  // TODO: Parse the response and store it in a data structure

  // Clean up
  curl_easy_cleanup(curl);

  return 0;
}
