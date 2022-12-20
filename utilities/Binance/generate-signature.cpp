#include "generate-signature.h"
#include "../read-keys.h"
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <cstring>
#include <iomanip>          // signature
#include <sstream>          // signature
#include <curl/curl.h>
#include <chrono>
#include <iostream>
#include <string>

// Empty constructor
SignatureSHA256::SignatureSHA256() {}

std::string SignatureSHA256::generate(const std::string& secret_key, const std::string& request_path,
    const std::string& request_method, const std::string& total_params)
{
    // Arrange the list of parameters into a string. Separate each parameter with a &
    // Encode signature payload as ASCII data
    //std::string payload = "symbol=BTCUSDT&side=SELL&type=LIMIT&timeInForce=GTC&quantity=1&price=0.2&timestamp=1668481559918recvWindow=5000";

    // Get RSA private key from file (trimemed)
    ReadKeys keys;
    std::string prv_key = keys.getRsaPrvKey("rsa.pem"); // rsa.pem

    std::cout << prv_key << std::endl;

    // Sign payload using RSASSA-PKCS1-v1_5 algorithm with SHA-256 hash function
    std::string signature = compute_signature(prv_key, prependQuestionMark(total_params));

    // Encode output as base64 string
    std::string enc_signature = base64_encode(reinterpret_cast<const unsigned char*>(signature.c_str()) , signature.length());
    std::cout << "signature b64: " << std::endl;
    std::cout << enc_signature << std::endl;

    return enc_signature;
    
    // Delete any newlines in signature

    // URL encoded

    // curl command

}

///
std::string SignatureSHA256::compute_signature(const std::string& key, const std::string& message) {
    // Initialize HMAC-SHA256 context
    HMAC_CTX* ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, (unsigned char*)key.c_str(), key.size(), EVP_sha256(), NULL);

    // Update context with message
    HMAC_Update(ctx, (unsigned char*)message.c_str(), message.size());

    // Finalize HMAC-SHA256 calculation
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int result_len;
    HMAC_Final(ctx, result, &result_len);
    HMAC_CTX_free(ctx);

    // Convert result to hex string
    std::stringstream ss;
    for (unsigned int i = 0; i < result_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)result[i];
    }
    std::string signature = ss.str();

    // Return signature
    return signature;
}

std::string SignatureSHA256::base64_encode(const unsigned char* input, int input_len) {
    // Initialize output buffer
    size_t output_len = (input_len / 3 + 1) * 4 + 1;
    unsigned char* output = new unsigned char[output_len];

    // Encode input to base64
    EVP_EncodeBlock(output, input, input_len);

    // Convert output to string and free memory
    std::string output_str(reinterpret_cast<char*>(output));
    delete[] output;
    return output_str;
}



////// new method
// std::string url_encode(const std::string& input) {
//     std::stringstream ss;
//     for (unsigned char c : input) {
//         if (c == ' ') {
//             ss << '+';
//         } else if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
//             ss << c;
//         } else {
//             ss << '%' << std::hex << std::uppercase << (int)c;
//         }
//     }
//     return ss.str();
// }



// std::string generate_signature(const std::string& api_secret, const std::string& api_call, const std::string& api_method, const std::string& api_params) {
//     // Initialize HMAC-SHA256 context
//     HMAC_CTX* ctx = HMAC_CTX_new();
//     HMAC_Init_ex(ctx, api_secret.c_str(), api_secret.size(), EVP_sha256(), NULL);

//     // Get the current timestamp in milliseconds
//     auto now = std::chrono::system_clock::now();
//     auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
//     auto value = now_ms.time_since_epoch();
//     long timestamp = value.count();

//     // Add the timestamp parameter to the request parameters
//     std::string total_params = api_params + "&timestamp=" + std::to_string(timestamp);

//     // Update context with request path and method
//     HMAC_Update(ctx, (unsigned char*)api_call.c_str(), api_call.size());
//     HMAC_Update(ctx, (unsigned char*)api_method.c_str(), api_method.size());

//     // Update context with request parameters
//     HMAC_Update(ctx, (unsigned char*)total_params.c_str(), total_params.size());

//     // Finalize HMAC-SHA256 calculation
//     unsigned char result[EVP_MAX_MD_SIZE];
//     unsigned int result_len;
//     HMAC_Final(ctx, result, &result_len);
//     HMAC_CTX_free(ctx);

//     // Convert result to base64 string
//     std::string signature = base64_encode(result, result_len);

//     // Return signature
//     return signature;
// }

std::string SignatureSHA256::prependQuestionMark(const std::string& s)
{
    if (s.empty() || s[0] == '?')
    {
        return s;
    }
    else
    {
        return "?" + s;
    }
}

/// 
std::string SignatureSHA256::hmacSha256(const std::string& key, const std::string& payload)
{
    // Compute the HMAC-SHA256 signature
    EVP_MD_CTX* ctx = EVP_MD_CTX_create();
    EVP_PKEY* pkey = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, nullptr,
                                           reinterpret_cast<const unsigned char*>(key.data()), key.size());
    EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, pkey);
    EVP_DigestSignUpdate(ctx, payload.data(), payload.size());
    size_t sigLen = 0;
    EVP_DigestSignFinal(ctx, nullptr, &sigLen);
    unsigned char* sig = new unsigned char[sigLen];
    EVP_DigestSignFinal(ctx, sig, &sigLen);
    EVP_PKEY_free(pkey);
    EVP_MD_CTX_destroy(ctx);

    // Convert the signature to a hexadecimal string
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < sigLen; ++i)
    {
        ss << std::setw(2) << static_cast<int>(sig[i]);
    }
    delete[] sig;
    return ss.str();
}