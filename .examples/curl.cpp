#include <iostream>
#include <string>
#include <curl/curl.h>

/*
In this example, the response from the API is stored in a std::string object called response. 
You can parse the response and store it in a data structure of your choice.
*/

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

int main()
{
  // Set up the request
  CURL *curl = curl_easy_init();
  std::string url = "https://api.binance.com/sapi/v1/convert/exchangeInfo";
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

  // Set up the response
  std::string response;
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  // Make the request
  CURLcode res = curl_easy_perform(curl);

  // Check for errors
  if (res != CURLE_OK)
  {
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    return 1;
  }

  // Parse the response
  // TODO: Store the response in a data structure

  //std::cout << response << std::endl;

  // Clean up
  curl_easy_cleanup(curl);

  return 0;
}

// This function is called by libcurl to handle the response data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  // Cast the user-provided pointer to a std::string
  std::string *response = static_cast<std::string *>(userp);

  // Append the new data to the response string
  response->append(static_cast<char *>(contents), size * nmemb);

  // Return the size of the response data
  return size * nmemb;
}
