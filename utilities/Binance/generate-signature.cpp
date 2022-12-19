#include "generate-signature.h"
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <cstring>
#include <iomanip>          // signature
#include <sstream>          // signature

// Empty constructor
SignatureSHA256::SignatureSHA256() {}

std::string SignatureSHA256::generate(const std::string& secret_key, const std::string& request_path,
    const std::string& request_method, const std::string& total_params)
{
    // Initialize HMAC-SHA256 context
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);

    // Update context with request path and method
    EVP_DigestUpdate(ctx, request_path.c_str(), request_path.size());
    EVP_DigestUpdate(ctx, request_method.c_str(), request_method.size());

    // Update context with request parameters
    EVP_DigestUpdate(ctx, total_params.c_str(), total_params.size());

    // Finalize HMAC-SHA256 calculation
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int result_len;
    EVP_DigestFinal_ex(ctx, result, &result_len);
    EVP_MD_CTX_free(ctx);

    // Convert result to hex string
    std::stringstream ss;
    for (unsigned int i = 0; i < result_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)result[i];
    }
    std::string signature = ss.str();

    // Return signature
    return signature;
}