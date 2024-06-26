#include <catch2/catch_all.hpp>
#include <cryptopals/core.hpp>

TEST_CASE("core utility functions", "[core]")
{
    SECTION("counting set bits")
    {
        REQUIRE(cpl::count_set_bits(0x01) == 1);
        REQUIRE(cpl::count_set_bits(0x03) == 2);
        REQUIRE(cpl::count_set_bits(0x00) == 0);
        REQUIRE(cpl::count_set_bits(0xFFFFFFFF) == 32);
    }

    SECTION("hamming distance")
    {
        REQUIRE(cpl::hamming_distance({ 0x01 }, { 0x02 }) == 2);
        REQUIRE(cpl::hamming_distance({ 0x11 }, { 0x02 }) == 3);
        REQUIRE(cpl::hamming_distance("this is a test", "wokka wokka!!!") == 37);
    }

    SECTION("PKCS#7 Padding")
    {
        REQUIRE(cpl::pad_pkcs7("YELLOW SUBMARINE", 20) == "YELLOW SUBMARINE\x04\x04\x04\x04");
        REQUIRE(cpl::pad_pkcs7("YELLOW SUBMARINE", 24) == "YELLOW SUBMARINE\x08\x08\x08\x08\x08\x08\x08\x08");
    }
}