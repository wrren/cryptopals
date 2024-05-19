#pragma once

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <cryptopals/core.hpp>

namespace cpl
{
    static constexpr size_t default_aes_block_size = 16;

    /**
     * @brief Generates a random AES key of the given size..
     * 
     * @param key_size  Key size in bytes
     * @param key       Output key
     */
    void random_aes_key(size_t key_size, byte_vector_t& key);

    /**
     * @brief Determine whether the given input is encrypted using ECB mode. Assumes that the cleartext
     * input has some repeating content. If this isn't the case, the function is likely to produce a false negative result.
     * 
     * @param in        Encrypted Input
     * @param key_size  Key size in bytes
     * @param padding   Padding added to the beginning and end of the cleartext
     * @return true     If the given input is encrypted using ECB mode
     * @return false    False otherwise.
     */
    bool is_ecb_encrypted(const byte_vector_view_t& in, size_t key_size, size_t padding = 0);

    /**
     * @brief Encrypts the given cleartext using a random key.
     * 
     * @param in                Cleartext
     * @param out               Encrypted output
     * @param block_size        Cipher block size
     * @param is_ecb_encrypted  Indicates whether ECB encryption (non-CBC mode) was used
     * @return true             On success
     * @return false            Otherwise
     */
    bool aes_encryption_oracle(const byte_vector_view_t& in, byte_vector_t& out, bool& is_ecb_encrypted, size_t block_size = default_aes_block_size);

    /**
     * @brief Decrypt an input byte vector using ECB with CBC mode.
     * 
     * @param in        Input Data
     * @param key       Decryption Key
     * @param iv        Initialization Vector
     * @param out       Output decrypted data
     * @return true     If decryption was successful
     * @return false    Otherwise.
     */
    bool aes_ecb_cbc_decrypt(const byte_vector_view_t& in, const byte_vector_t& key, const byte_vector_t& iv, byte_vector_t& out);

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

    /**
     * @brief Decrypt an input byte vector view using the given key and the AES-ECB cipher.
     * 
     * @param in        Input Data View
     * @param key       Decryption Key
     * @param out       Output Decrypted Data
     * @return true     On success
     * @return false    Otherwise
     */
    bool aes_ecb_decrypt(const byte_vector_view_t& in, const byte_vector_t& key, byte_vector_t& out);

    /**
     * @brief Encrypt an input byte vector using ECB with CBC mode.
     * 
     * @param in        Input Data
     * @param key       Encryption Key
     * @param iv        Initialization Vector
     * @param out       Output encrypted data
     * @return true     If encryption was successful
     * @return false    Otherwise.
     */
    bool aes_ecb_cbc_encrypt(const byte_vector_view_t& in, const byte_vector_t& key, const byte_vector_t& iv, byte_vector_t& out);

    /**
     * @brief Encrypt an input byte vector using the given key and the AES-ECB cipher.
     * 
     * @param in        Input Data
     * @param key       Encryption Key
     * @param out       Output Encrypted Data
     * @return true     On success
     * @return false    Otherwise
     */
    bool aes_ecb_encrypt(const byte_vector_t& in, const byte_vector_t& key, byte_vector_t& out);

    /**
     * @brief Encrypt an input byte vector view using the given key and the AES-ECB cipher.
     * 
     * @param in        Input Data View
     * @param key       Encryption Key
     * @param out       Output Encrypted Data
     * @return true     On success
     * @return false    Otherwise
     */
    bool aes_ecb_encrypt(const byte_vector_view_t& in, const byte_vector_t& key, byte_vector_t& out);
}