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
#include <chrono>
#include <iostream>
#include <string>

// Empty constructor
GenerateSignature::GenerateSignature() {}

std::string GenerateSignature::rsaHmacSha256(const std::string& secret_key, const std::string& request_path,
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

std::string GenerateSignature::compute_signature(const std::string& key, const std::string& message) {
    return hmacSha256(key, message);
}



std::string GenerateSignature::base64_encode(const unsigned char* input, int input_len) {
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

// Prepend '?' to total_params payload
std::string GenerateSignature::prependQuestionMark(const std::string& s)
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

// Returns an HMAC SHA256 signature
// secret_key is used as the key and total_params (payload) as the value
// for the HMAC operation
std::string GenerateSignature::hmacSha256(const std::string& key, const std::string& payload)
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