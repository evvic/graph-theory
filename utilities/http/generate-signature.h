#ifndef GENERATE_SIGNATURE
#define GENERATE_SIGNATURE

#include <string>

// To include a valid request signature in the request to the Binance API:
// Must perform a keyed HMAC SHA256 operation

class GenerateSignature {
private:

    /////// Functions supporting generate() function ///////
    // 2.2
    // Takes the key and message as inputs, and returns a string containing the HMAC-SHA256 signature in hexadecimal format.
    std::string compute_signature(const std::string& key, const std::string& message);

    // 2.3
    // Encode output as base64 strin
    std::string base64_encode(const unsigned char* input, int input_len);

    // Prepend '?' to total_params payload
    std::string prependQuestionMark(const std::string& s);

public:
    GenerateSignature();

    // Returns a calculated SHA 256 signature required for authorized API calls
    std::string rsaHmacSha256(const std::string& secret_key, const std::string& request_path,
                         const std::string& request_method, const std::string& total_params);

    // Returns an HMAC SHA256 signature required for authorized API calls
    // secret_key is used as the key and total_params (payload) as the value
    // for the HMAC operation
    std::string hmacSha256(const std::string& key, const std::string& payload);
};

#endif