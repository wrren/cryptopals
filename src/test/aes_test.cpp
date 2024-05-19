#include <catch2/catch_all.hpp>
#include <cryptopals/encode.hpp>
#include <cryptopals/aes.hpp>
#include <cryptopals/core.hpp>
#include <cryptopals-test/support.hpp>
#include <fstream>
#include <sstream>

TEST_CASE("AES ECB Encryption/Decryption", "[aes]")
{
    SECTION("decryption using known key")
    {
        std::ifstream input_file(cpl_test::get_input("aes-ecb.txt").value());

        REQUIRE(input_file.is_open());

        std::stringstream buffer;
        buffer << input_file.rdbuf();

        auto decoded = cpl::base64_to_byte_vector(buffer.str());
        REQUIRE(decoded);

        cpl::byte_vector_t decrypted;

        REQUIRE(cpl::aes_ecb_decrypt(decoded.value(), "YELLOW SUBMARINE", decrypted));

        REQUIRE(decrypted.substr(0, 8) == "I'm back");
    }

    SECTION("Detect ECB Mode")
    {
        std::ifstream input_file(cpl_test::get_input("s1c8.txt").value());

        REQUIRE(input_file.is_open());

        cpl::byte_vector_t line;
        bool found_one = false;

        while(std::getline(input_file, line))
        {
            auto decoded = cpl::hex_to_byte_vector(line);
            REQUIRE(decoded);

            found_one |= cpl::is_ecb_encrypted(decoded.value(), 16);
        }

        REQUIRE(found_one);
    }

    SECTION("CBC Mode")
    {
        std::ifstream input_file(cpl_test::get_input("s2c2.txt").value());

        REQUIRE(input_file.is_open());

        std::stringstream buffer;
        buffer << input_file.rdbuf();

        auto decoded = cpl::base64_to_byte_vector(buffer.str());
        REQUIRE(decoded);

        cpl::byte_vector_t encrypted, decrypted;

        REQUIRE(cpl::aes_ecb_cbc_encrypt(decoded.value(), "YELLOW SUBMARINE", cpl::byte_vector_t(16, '\x00'), encrypted));
        REQUIRE(cpl::aes_ecb_cbc_decrypt(encrypted, "YELLOW SUBMARINE", cpl::byte_vector_t(16, '\x00'), decrypted));
        REQUIRE(decrypted == decoded.value());

        REQUIRE(cpl::aes_ecb_cbc_decrypt(decoded.value(), "YELLOW SUBMARINE", cpl::byte_vector_t(16, '\x00'), decrypted));
        REQUIRE(decrypted.substr(0, 8) == "I'm back");
    }

    SECTION("Oracle Encryption")
    {
        std::ifstream input_file(cpl_test::get_input("s2c2.txt").value());

        REQUIRE(input_file.is_open());

        std::stringstream buffer;
        buffer << input_file.rdbuf();

        auto decoded = cpl::base64_to_byte_vector(buffer.str());
        REQUIRE(decoded);

        cpl::byte_vector_t decrypted;
        
        REQUIRE(cpl::aes_ecb_cbc_decrypt(decoded.value(), "YELLOW SUBMARINE", cpl::byte_vector_t(16, '\x00'), decrypted));

        for(size_t i = 0; i < 20; i++)
        {
            cpl::byte_vector_t encrypted;
            bool is_ecb_encrypted;
            REQUIRE(cpl::aes_encryption_oracle(decrypted, encrypted, is_ecb_encrypted));

            bool identified_as_ecb = false;

            for(size_t p = 0; p < 10; p++)
            {
                if(cpl::is_ecb_encrypted(encrypted, cpl::default_aes_block_size, p))
                {
                    identified_as_ecb = true;
                    break;
                }
            }

            if(is_ecb_encrypted)
            {
                REQUIRE(identified_as_ecb);
            }
            else
            {
                REQUIRE_FALSE(identified_as_ecb);
            }
        }
    }
}