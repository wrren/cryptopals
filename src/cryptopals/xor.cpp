#include <cryptopals/xor.hpp>
#include <limits>
#include <map>
#include <numeric>
#include <string_view>

namespace cpl
{
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

    byte_vector_t xor_encrypt(const byte_vector_t& in, const byte_vector_t& pad)
    {
        byte_vector_t out;
        out.resize(in.size());

        for(size_t i = 0; i < in.length(); i++)
        {
            out[i] = in[i] ^ pad[i % pad.size()];
        }

        return out;
    }

    void xor_encrypt(const byte_vector_t& in, byte_vector_t& out, const byte_vector_t& pad)
    {
        out.resize(in.size());

        for(size_t i = 0; i < in.length(); i++)
        {
            out[i] = in[i] ^ pad[i % pad.size()];
        }
    }

    byte_vector_t xor_decrypt_scored(const byte_vector_t& in, size_t key_size, byte_vector_t& key, score_func_t score_func, double& score)
    {
        key.resize(key_size);
        for(auto& c : key)
        {
            c = 0;
        }

        std::vector<uint8_t> key_vector;
        key_vector.resize(key_size);

        byte_vector_t out;
        out.resize(in.size());

        for(size_t i = 0; i < key_size; i++)
        {
            double index_score = std::numeric_limits<double>::lowest();
            uint8_t best_byte = 0;

            for(uint8_t j = 0; j < std::numeric_limits<uint8_t>::max(); j++)
            {
                key[i] = static_cast<char>(j);
                key_vector[i] = j;

                xor_encrypt(in, out, key);

                double current_score = score_func(out, key_size, i);

                if(current_score > index_score)
                {
                    index_score = current_score;
                    best_byte = j;
                }
            }

            key[i] = best_byte;
            key_vector[i] = best_byte;
        }

        xor_encrypt(in, out, key);
        score = score_func(out, 1, 0);

        return out;
    }

    std::vector<size_t> guess_xor_key_length(const byte_vector_t& in, size_t max_guesses)
    {
        std::vector<std::pair<size_t, size_t>> results;
        constexpr size_t max_blocks = 10;

        for(size_t length = 1; length < (max_guesses) + 1 && length <= (in.size() / 2); length++)
        {
            size_t distance = 0;
            std::vector<size_t> distances;

            auto first_view = std::string_view(in.c_str(), length);

            for(size_t b = 1; b < max_blocks; b++)
            {
                if((b * length + length + length) < in.length())
                {
                    auto second_start   = (b * length);
                    auto second_view    = std::string_view(in.c_str() + second_start, length);

                    distance += cpl::hamming_distance_view(first_view, second_view);
                }
            }

            distance /= length;

            results.push_back({ length, distance });
        }

        std::sort(results.begin(), results.end(), [](const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b)
        {
            return a.second < b.second;
        });

        std::vector<size_t> lengths;

        for(auto& r : results)
        {
            lengths.push_back(r.first);
        }

        return lengths;
    }
}