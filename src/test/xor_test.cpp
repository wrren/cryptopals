#include <catch2/catch_all.hpp>
#include <cryptopals/encode.hpp>
#include <cryptopals/xor.hpp>
#include <cryptopals-test/support.hpp>
#include <fstream>

TEST_CASE("XOR Encryption/Decryption", "[xor]")
{
    SECTION("single-byte decryption")
    {
        std::ifstream input_file(cpl_test::get_input("xor-inputs.txt").value());

        REQUIRE(input_file.is_open());

        cpl::byte_vector_t cleartext;
        cpl::byte_vector_t key;
        double highest_score = std::numeric_limits<double>::lowest();

        cpl::byte_vector_t input;

        while(std::getline(input_file, input))
        {
            auto decoded = cpl::hex_to_byte_vector(input);
            REQUIRE(decoded);

            double score;
            auto out = cpl::xor_decrypt_scored(*decoded, 1, key, cpl::character_frequency_scorer, score);
            if(score > highest_score)
            {
                cleartext = out;
                highest_score = score;
            }
        }

        REQUIRE(cleartext == "Now that the party is jumping\n");
    }
}