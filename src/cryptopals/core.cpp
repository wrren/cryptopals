#include <cryptopals/core.hpp>

namespace cpl
{
    size_t hamming_distance(const byte_vector_t& first, const byte_vector_t& second)
    {
        size_t distance = 0;

        for(size_t i = 0; i < std::min(first.size(), second.size()); i++)
        {
            distance += count_set_bits(first[i] ^ second[i]);
        }

        return distance;
    }
}