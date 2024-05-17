#include <catch2/catch_all.hpp>
#include <cryptopals/encode.hpp>
#include <cryptopals/aes.hpp>
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
}