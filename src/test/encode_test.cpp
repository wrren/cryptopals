#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <cryptopals/encode.hpp>

TEST_CASE("encoding functions", "[encode]")
{
    std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

    SECTION("hex to byte vector")
    {
        auto bytes = cpl::hex_to_byte_vector(hex);
        REQUIRE(bytes.has_value());

        auto encoded = cpl::byte_vector_to_hex(bytes.value());
        REQUIRE(encoded == hex);
    }

    SECTION("hex to base64")
    {
        auto bytes = cpl::hex_to_byte_vector(hex);
        REQUIRE(bytes.has_value());

        auto encoded = cpl::byte_vector_to_base64(bytes.value());
        REQUIRE(encoded == "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
    }

    SECTION("base64 to ascii")
    {
        auto bytes = cpl::base64_to_byte_vector("bGlnaHQgd29yay4=");
        REQUIRE(bytes.has_value());

        auto decoded = bytes.value();
        decoded = decoded.substr(0, decoded.find_last_not_of("\0"));

        REQUIRE(decoded == "light work.");
    }

    SECTION("ascii to base64")
    {
        auto bytes = cpl::byte_vector_t("light work.");

        auto encoded = cpl::byte_vector_to_base64(bytes);
        REQUIRE(encoded == "bGlnaHQgd29yay4=");

        bytes = cpl::byte_vector_t("light work");

        encoded = cpl::byte_vector_to_base64(bytes);
        REQUIRE(encoded == "bGlnaHQgd29yaw==");
    }
}