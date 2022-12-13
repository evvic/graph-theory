#include "read-keys.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

std::string ReadKeys::getKey(std::string fname = ".env", std::string key = "api_key") {
    std::ifstream inputFile;  // Create an input file stream
    inputFile.open(fname);  // Open the file

    // Check if file could be opened
    if (!inputFile.is_open()) {
        // The file could not be opened
        std::cerr << "Error: unable to open file" << fname << std::endl;
        return ""; 
    }

    std::string line;  // Create a string to store each line as it's read

    while (std::getline(inputFile, line)) {  // Read each line from the file
        // Split the line into a key and a value
        std::size_t pos = line.find("=");
        std::string tkey = line.substr(0, pos);
        std::string tvalue = line.substr(pos + 1);

        // If found key, return it
        if (tkey == key) {

            // Check if the value is surrounded by quotation marks
            if (tvalue.front() == '"' && tvalue.back() == '"') {
            // Remove the quotation marks from the value
                tvalue = tvalue.substr(1, tvalue.length() - 2);
            }

            inputFile.close();  // Close the file
            return tvalue;
        }
    }

    inputFile.close();  // Close the file

    std::cerr << "Error finding " << key << " in " << fname << std::endl;
    return "";
}