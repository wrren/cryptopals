#pragma once

#include <string>
#include <cstdint>
#include <optional>
#include <string_view>

namespace cpl
{
    typedef uint8_t             byte_t;
    typedef std::string         byte_vector_t;
    typedef std::string_view    byte_vector_view_t;

    /**
     * @brief Calculates the hamming distance between two byte vector views.
     * 
     * @param first     First byte vector view
     * @param second    Second byte vector view
     * @return size_t   Hamming distance between the two vector views
     */
    size_t hamming_distance_view(const byte_vector_view_t& first, const byte_vector_view_t& second);

    /**
     * @brief Calculates the hamming distance between two byte vectors.
     * 
     * @param first     First byte vector
     * @param second    Second byte vector
     * @return size_t   Hamming distance between the two vectors
     */
    size_t hamming_distance(const byte_vector_t& first, const byte_vector_t& second);

    /**
     * @brief Count the number of bits set to 1 in the given value.
     * 
     * @tparam T        Input integral type
     * @param in        Input value
     * @return size_t   Number of bits in the given value that have been set to 1
     */
    template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    size_t count_set_bits(T in)
    {
        size_t set = 0;

        for(size_t i = 0; i < sizeof(T) * 8; i++)
        {
            if(0x01 & static_cast<uint8_t>((in >> i)))
            {
                ++set;
            }
        }

        return set;
    }
}