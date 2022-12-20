#include "read-keys.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

std::string ReadKeys::getEnvKey(std::string fname = ".env", std::string key = "api_key") {
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

std::string ReadKeys::getRsaPrvKey(const std::string& fname) {
    std::ifstream inputFile;  // Create an input file stream
    inputFile.open(fname);  // Open the file

    // Check if file could be opened
    if (!inputFile.is_open()) {
        // The file could not be opened
        std::cerr << "Error: unable to open file" << fname << std::endl;
        return ""; 
    }

    // Read the contents of the .pem file into a string
    std::string pemKey((std::istreambuf_iterator<char>(inputFile)),
                       std::istreambuf_iterator<char>());

    inputFile.close();

    return removeHeaderFooter(pemKey);
}

std::string ReadKeys::removeHeaderFooter(const std::string& pemKey)
{
    // Split the private key into separate lines
    std::vector<std::string> lines;
    size_t start = 0;
    size_t end = pemKey.find('\n');
    while (end != std::string::npos)
    {
        lines.push_back(pemKey.substr(start, end - start));
        start = end + 1;
        end = pemKey.find('\n', start);
    }
    lines.push_back(pemKey.substr(start));

    // Concatenate the lines of the private key, skipping the header and footer
    std::string key;
    bool skip = true;
    for (const auto& line : lines)
    {
        if (line.find("-----BEGIN") == 0)
        {
            skip = false;
            continue;
        }
        if (line.find("-----END") == 0)
        {
            break;
        }
        if (!skip)
        {
            key += line;
        }
    }

    // Remove any whitespace or newline characters
    key.erase(std::remove(key.begin(), key.end(), '\n'), key.end());
    key.erase(std::remove(key.begin(), key.end(), '\r'), key.end());
    key.erase(std::remove(key.begin(), key.end(), ' '), key.end());

    return key;
}
