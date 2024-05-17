#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <cryptopals/core.hpp>

namespace cpl
{
    /**
     * @brief Decrypt an input byte vector using the given key and the AES-ECB cipher.
     * 
     * @param in        Input Data
     * @param key       Decryption Key
     * @param out       Output Decrypted Data
     * @return true     On success
     * @return false    Otherwise
     */
    bool aes_ecb_decrypt(const byte_vector_t& in, const byte_vector_t& key, byte_vector_t& out);
}