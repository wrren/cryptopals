#include <cryptopals/core.hpp>
#include <map>

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

    double character_frequency_scorer(const byte_vector_t& in, size_t key_size, size_t offset)
    {
        static const std::map<char, double> frequency = {
            {'a', 08.12}, {'b', 01.49}, {'c', 02.71}, {'d', 04.32}, {'e', 12.02},
            {'f', 02.30}, {'g', 02.03}, {'h', 05.92}, {'i', 07.31}, {'j', 00.10},
            {'k', 00.69}, {'l', 03.98}, {'m', 02.61}, {'n', 06.95}, {'o', 07.68},
            {'p', 01.82}, {'q', 00.11}, {'r', 06.02}, {'s', 06.28}, {'t', 09.10},
            {'u', 02.88}, {'v', 01.11}, {'w', 02.09}, {'x', 00.17}, {'y', 02.11},
            {'z', 00.07}, {' ', 02.00}, {'.', 00.30}, {',', 00.70}, {'\'', 0.10},
            {'!', 00.10}
        };

        double score = 0;

        for(size_t i = offset; i < in.size(); i += key_size)
        {
            auto found = frequency.find(std::tolower(in[i]));
            if(found != frequency.end())
            {
                if(std::tolower(in[i]) != in[i])
                {
                    score += found->second * 0.2;
                }
                else
                {
                    score += found->second;
                }
            }
            else
            {
                score -= 5.0;
            }
        }

        return score;
    }

    byte_vector_t pad_pkcs7(const byte_vector_t& in, size_t block_size)
    {
        size_t in_size      = in.size();
        size_t modulo       = (in_size % block_size);

        if(modulo == 0)
        {
            return in;
        }

        size_t pad_bytes    = block_size - modulo;

        return byte_vector_t(in) + byte_vector_t(pad_bytes, pad_bytes);
    }
}