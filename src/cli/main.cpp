#include <iostream>
#include <cryptopals/encode.hpp>
#include <cryptopals/xor.hpp>
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

        std::cout << cpl::byte_vector_to_hex(cpl::xor_encrypt(*in, *pad)) << std::endl;

        return EXIT_SUCCESS;
    }
    else if(command == "xor-encrypt" && argc == 4)
    {
        std::cout << cpl::byte_vector_to_hex(cpl::xor_encrypt(argv[2], argv[3])) << std::endl;

        return EXIT_SUCCESS;
    }
    else if(command == "xor-decrypt" && argc == 3)
    {
        auto in = cpl::hex_to_byte_vector(argv[2]);
        double score = 0;

        if(!in)
        {
            std::cerr << "Invalid hex input specified" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout   << cpl::xor_decrypt_scored(*in, 1, cpl::character_frequency_scorer, score) 
                    << std::endl << "Score: " << score << std::endl;

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
        double highest_score = std::numeric_limits<double>::min();

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
            auto out = cpl::xor_decrypt_scored(*decoded, 1, cpl::character_frequency_scorer, score);
            if(score > highest_score)
            {
                cleartext = out;
                highest_score = score;
            }
        }

        std::cout   << cleartext
                    << std::endl << "Score: " << highest_score << std::endl;

        return EXIT_SUCCESS;
    }
    else if(command == "xor-decrypt-repeating" && argc == 3)
    {
        std::ifstream input_file(argv[2]);

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

        auto lengths = cpl::guess_xor_key_length(decoded.value(), 10);

        cpl::byte_vector_t cleartext;
        double highest_score = std::numeric_limits<double>::min();

        for(auto l : lengths)
        {
            double score;
            auto out = cpl::xor_decrypt_scored(decoded.value(), l, cpl::character_frequency_scorer, score);
            if(score > highest_score)
            {
                cleartext = out;
                highest_score = score;
            }
        }

        std::cout   << cleartext
                    << std::endl << "Score: " << highest_score << std::endl;

        return EXIT_SUCCESS;
    }
    
    return usage();
}
