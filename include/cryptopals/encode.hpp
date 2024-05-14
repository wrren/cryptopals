#pragma once

#include <cryptopals/core.hpp>

namespace cpl
{
    /**
     * @brief Decode a hex string into a byte vector.
     * 
     * @param hex_string String to be decoded
     * @return std::optional<byte_vector_t> Byte vector or nullopt if the string does not contain valid hex.
     */
    std::optional<byte_vector_t> hex_to_byte_vector(const std::string& hex_string);

    /**
     * @brief Decode a base64 string into a byte vector.
     * 
     * @param base64_string String to be decoded
     * @return std::optional<byte_vector_t> Byte vector or nullopt if the string does not contain valid base64.
     */
    std::optional<byte_vector_t> base64_to_byte_vector(const std::string& base64_string);

    /**
     * @brief Encode a byte vector to a hex string.
     * 
     * @param bytes Byte vector
     * @return std::string String containing the byte vector encoded as hex characters
     */
    std::string byte_vector_to_hex(const byte_vector_t& bytes);

    /**
     * @brief Encode a byte vector to a base-64 string.
     * 
     * @param bytes Byte vector
     * @return std::string String containing the byte vector encoded as base-64
     */
    std::string byte_vector_to_base64(const byte_vector_t& bytes);
}