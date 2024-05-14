#include <cryptopals/xor.hpp>
#include <limits>
#include <map>

namespace cpl
{
    double character_frequency_scorer(const byte_vector_t& in, size_t key_size, size_t offset)
    {
        static const std::map<char, double> frequency = {
            {'a', 8.167}, {'b', 1.492}, {'c', 2.782}, {'d', 4.253}, {'e', 12.702},
            {'f', 2.228}, {'g', 2.015}, {'h', 6.094}, {'i', 6.966}, {'j', 0.153},
            {'k', 0.772}, {'l', 4.025}, {'m', 2.406}, {'n', 6.749}, {'o', 7.507},
            {'p', 1.929}, {'q', 0.095}, {'r', 5.987}, {'s', 6.327}, {'t', 9.056},
            {'u', 2.758}, {'v', 0.978}, {'w', 2.360}, {'x', 0.150}, {'y', 1.974},
            {'z', 0.074}, {' ', 1.000}, {'.', 0.400}, {',', 0.600}
        };

        double score = 0;

        for(size_t i = offset; i < in.size(); i += key_size)
        {
            char l = std::tolower(in[i]);

            auto found = frequency.find(l);
            if(found != frequency.end())
            {
                score += found->second;
            }
            else
            {
                --score;
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

    byte_vector_t xor_decrypt_scored(const byte_vector_t& in, size_t key_size, score_func_t score_func, double& score)
    {
        byte_vector_t key;
        key.resize(key_size);

        byte_vector_t out;
        out.resize(in.size());

        for(size_t i = 0; i < key_size; i++)
        {
            double index_score = std::numeric_limits<double>::min();
            uint8_t best_byte = 0;

            for(uint8_t j = 0; j < std::numeric_limits<uint8_t>::max(); j++)
            {
                key[i] = j;

                xor_encrypt(in, out, key);

                double current_score = score_func(out, key_size, i);

                if(current_score > index_score)
                {
                    index_score = current_score;
                    best_byte = j;
                }
            }

            key[i] = best_byte;
        }

        xor_encrypt(in, out, key);
        score = score_func(out, 1, 0);

        return out;
    }

    std::vector<size_t> guess_xor_key_length(const byte_vector_t& in, size_t max_guesses)
    {
        std::vector<std::pair<size_t, size_t>> results;

        for(size_t length = 1; length < (max_guesses) + 1 && length <= (in.size() / 2); length++)
        {
            auto first_block    = in.substr(0, length);
            auto second_block   = in.substr(length, length);

            size_t distance = cpl::hamming_distance(first_block, second_block);
            results.push_back({ length, distance / length });
        }

        std::sort(results.begin(), results.end(), [](const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b)
        {
            return a.second > b.second;
        });

        std::vector<size_t> lengths;

        for(auto& r : results)
        {
            lengths.push_back(r.first);
        }

        return lengths;
    }
}