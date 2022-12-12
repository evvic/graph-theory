  #include <iostream>
  #include <string>
  #include <vector>
  #include <map>
  #include <json/json.h> // for parsing JSON
  #include <curl/curl.h> // for making HTTP requests
  #include "../utilities/c2c-edge.h" // for edge data structure

  /*
  Fills a map with "SYMBOLPAIR" and rate of exchange of all tradeable
  c2c currency pairs
  */    

  // Define a callback function that will be called by the curl library
  // to process the response data from the API
  size_t callback(void *contents, size_t size, size_t nmemb, void *userp)
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

  int main()
  {
    // Set up the CURL request
    CURL *curl = curl_easy_init();
    const std::string url = "https://api.binance.com/sapi/v1/convert/exchangeInfo";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set up a vector to store the product pairs and their spot prices
    std::map<std::string, double> rates;

    // Set up a buffer to store the response from the API
    std::string response;

    // Reserve enough memory for the response
    response.reserve(4096); // reserve 4 KB of memory

    // Set the callback function and user data pointer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
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

    return 0;
  }
