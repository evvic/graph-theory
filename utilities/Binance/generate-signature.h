#ifndef GENERATE_SIGNATURE
#define GENERATE_SIGNATURE

#include <string>

// To include a valid request signature in the request to the Binance API:
// Must perform a keyed HMAC SHA256 operation

class SignatureSHA256 {
private:

public:
    SignatureSHA256();

    std::string generate(const std::string& secret_key, const std::string& request_path,
                         const std::string& request_method, const std::string& total_params);

};

#endif