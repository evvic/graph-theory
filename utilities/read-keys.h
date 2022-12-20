#ifndef READ_KEYS
#define READ_KEYS

#include <string>

class ReadKeys {
private:
    // Private helper function for removing header and footer of .pem 
    // file private keys
    std::string removeHeaderFooter(const std::string& pemKey);

public:
    std::string getEnvKey(std::string fname, std::string value);
    
    // Returns RSA/OPENSSH private key from .pem file
    // Already trimmed header and footer (and whitespace)
    //    - using helper funciton removeHeaderFooter
    std::string getRsaPrvKey(const std::string& fname);
};

#endif