#pragma once

#include <cryptopals/core.hpp>
#include <functional>
#include <string>
#include <array>
#include <limits>

namespace cpl
{
    /**
     * @brief Perform a simple XOR encryption against the input byte vector using
     * the given one-time pad.
     * 
     * @param in    Input Data
     * @param pad   XOR Encryption Pad
     * @param out   Output Byte Vector
     */
    void xor_encrypt(const byte_vector_view_t& in, const byte_vector_t& pad, byte_vector_t& out);

    /**
     * @brief Given an encrypted input, attempt to decrypt it using single-byte XOR
     * keys based on the given scoring function.
     * 
     * @param in                Input encrypted using single-byte XOR
     * @param key_size          XOR key size
     * @param key               Output key
     * @param score_func        Scoring function used to find the most likely cleartext result
     * @param score             Highest score recorded
     * @return byte_vector_t    Highest scoring decryption result
     */
    byte_vector_t xor_decrypt_scored(const byte_vector_view_t& in, size_t key_size, byte_vector_t& key, score_func_t score_func, double& score);

    /**
     * @brief Attempt to guess the length of the XOR key used to encrypt a given input. Returns a list
     * of key lengths ordered by likelihood of correctness
     * 
     * @param in            Input encrypted using an XOR key of unknown length
     * @param max_guesses   Maximum number of results to return
     * @return std::vector<xor_key> List of XOR key lengths orderedin descending likelihood of correctness
     */
    std::vector<size_t> guess_xor_key_length(const byte_vector_view_t& in, size_t max_guesses);
}