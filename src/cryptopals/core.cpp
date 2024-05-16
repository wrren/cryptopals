#include <cryptopals/core.hpp>

namespace cpl
{
    size_t hamming_distance_view(const byte_vector_view_t& first, const byte_vector_view_t& second)
    {
        size_t distance = 0;

        for(size_t i = 0; i < std::min(first.size(), second.size()); i++)
        {
            distance += count_set_bits(static_cast<uint8_t>(first[i] ^ second[i]));
        }

        return distance;
    }

    size_t hamming_distance(const byte_vector_t& first, const byte_vector_t& second)
    {
        return hamming_distance_view(byte_vector_view_t(first), byte_vector_view_t(second));
    }
}