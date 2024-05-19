#include <iostream>
#include <cryptopals/encode.hpp>
#include <cryptopals/xor.hpp>
#include <cryptopals/aes.hpp>
#include <fstream>
#include <limits>
#include <sstream>

int usage()
{
    std::cout << "Usage: cli COMMAND ..." << std::endl;
    return EXIT_FAILURE;
}

int main(int argc, char** argv) 
{
    if(argc == 1)
    {
        return usage();
    }

    std::string command(argv[1]);

    if(command == "hex-to-base64" && argc == 3)
    {
        auto bytes = cpl::hex_to_byte_vector(argv[2]);
        if(!bytes)
        {
            return usage();
        }

        std::cout << cpl::byte_vector_to_hex(*bytes) << std::endl;
        return EXIT_SUCCESS;
    }
    else if(command == "xor" && argc == 4)
    {
        auto in     = cpl::hex_to_byte_vector(argv[2]);
        auto pad    = cpl::hex_to_byte_vector(argv[3]);

        if(!in || !pad)
        {
            std::cerr << "Invalid hex inputs specified" << std::endl;
            return EXIT_FAILURE;
        }

        cpl::byte_vector_t encrypted;
        cpl::xor_encrypt(*in, *pad, encrypted);

        std::cout << cpl::byte_vector_to_hex(encrypted) << std::endl;

        return EXIT_SUCCESS;
    }
    else if(command == "xor-encrypt" && argc == 4)
    {
        cpl::byte_vector_t encrypted;
        cpl::xor_encrypt(argv[2], argv[3], encrypted);

        std::cout << cpl::byte_vector_to_hex(encrypted) << std::endl;

        return EXIT_SUCCESS;
    }
    else if(command == "xor-decrypt" && argc == 3)
    {
        auto in = cpl::hex_to_byte_vector(argv[2]);
        double score = 0;
        cpl::byte_vector_t key;

        if(!in)
        {
            std::cerr << "Invalid hex input specified" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout   << cpl::xor_decrypt_scored(*in, 1, key, cpl::character_frequency_scorer, score) 
                    << std::endl << "Score: " << score << std::endl
                    << "Key: " << key << std::endl;

        return EXIT_SUCCESS;
    }
    else if(command == "xor-decrypt-file" && argc == 3)
    {
        std::ifstream input_file(argv[2]);

        if(!input_file.is_open())
        {
            std::cerr << "Failed to open input file." << std::endl;
            return EXIT_FAILURE;
        }

        cpl::byte_vector_t cleartext;
        cpl::byte_vector_t key;
        double highest_score = std::numeric_limits<double>::lowest();

        cpl::byte_vector_t input;

        while(std::getline(input_file, input))
        {
            auto decoded = cpl::hex_to_byte_vector(input);
            if(!decoded)
            {
                std::cerr << "Invalid hex input specified" << std::endl;
                return EXIT_FAILURE;
            }

            double score;
            auto out = cpl::xor_decrypt_scored(*decoded, 1, key, cpl::character_frequency_scorer, score);
            if(score > highest_score)
            {
                cleartext = out;
                highest_score = score;
            }
        }

        std::cout   << cleartext
                    << std::endl << "Score: " << highest_score << std::endl
                    << "Key: " << key << std::endl;

        return EXIT_SUCCESS;
    }
    else if(command == "xor-decrypt-repeating" && argc >= 3)
    {
        std::ifstream input_file(argv[2], std::ios::binary);

        size_t max_keysize = 40;
        if(argc == 4)
        {
            max_keysize = std::stoull(argv[3]);
        }

        if(!input_file.is_open())
        {
            std::cerr << "Failed to open input file." << std::endl;
            return EXIT_FAILURE;
        }

        std::stringstream buffer;
        buffer << input_file.rdbuf();

        auto decoded = cpl::base64_to_byte_vector(buffer.str());
        if(!decoded)
        {
            std::cerr << "Failed to decode input file contents from base64." << std::endl;
            return EXIT_FAILURE;
        }

        auto lengths = cpl::guess_xor_key_length(decoded.value(), 50);
        size_t best_length = lengths[0];

        cpl::byte_vector_t cleartext;
        double highest_score = std::numeric_limits<double>::lowest();
        cpl::byte_vector_t key;
        cpl::byte_vector_t best_key;

        for(size_t i = 0; i < 20; i++)
        {
            double score;
            auto out = cpl::xor_decrypt_scored(decoded.value(), lengths[i], key, cpl::character_frequency_scorer, score);
            if(score > highest_score)
            {
                cleartext       = out;
                highest_score   = score;
                best_length     = lengths[i];
                best_key        = key;
            }
        }

        std::cout   << cleartext << std::endl
                    << std::endl << "Score: " << highest_score << std::endl
                    << "Length: " << best_length << std::endl
                    << "Key: " << best_key << std::endl;

        return EXIT_SUCCESS;
    }
    else if(command == "aes-decrypt" && argc == 4)
    {
        cpl::byte_vector_t key = argv[2];
        std::ifstream input_file(argv[3], std::ios::binary);
        
        if(!input_file.is_open())
        {
            std::cerr << "Failed to open input file." << std::endl;
            return EXIT_FAILURE;
        }

        std::stringstream buffer;
        buffer << input_file.rdbuf();

        auto decoded = cpl::base64_to_byte_vector(buffer.str());
        if(!decoded)
        {
            std::cerr << "Failed to decode input file contents from base64." << std::endl;
            return EXIT_FAILURE;
        }

        cpl::byte_vector_t decrypted;

        if(!cpl::aes_ecb_decrypt(decoded.value(), key, decrypted))
        {
            std::cerr << "Failed to decrypt input file contents." << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << decrypted << std::endl;

        return EXIT_SUCCESS;
    }
    
    return usage();
}
