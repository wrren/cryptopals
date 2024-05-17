#include <cryptopals/encode.hpp>
#include <sstream>

namespace cpl
{
    std::optional<byte_vector_t> hex_to_byte_vector(const std::string& hex_string)
    {
        if(hex_string.length() % 2 != 0)
        {
            return std::nullopt;
        }

        auto char_to_byte = [](char h) -> std::optional<byte_t>
        {
            if(h >= '0' && h <= '9')
            {
                return h - '0';
            }
            else if(h >= 'A' && h <= 'F')
            {
                return h - 'A' + 10;
            }
            else if(h >= 'a' && h <= 'f')
            {
                return h - 'a' + 10;
            }

            return std::nullopt;
        };

        byte_vector_t bytes;
        bytes.reserve(hex_string.length() / 2);

        for(size_t i = 0; i < hex_string.length(); i += 2)
        {
            auto upper = char_to_byte(hex_string[i]);
            auto lower = char_to_byte(hex_string[i+1]);

            if(upper.has_value() && lower.has_value())
            {
                bytes.push_back((*upper << 4) | *lower);
            }
            else
            {
                return std::nullopt;
            }
        }

        return bytes;
    }

    std::optional<byte_vector_t> base64_to_byte_vector(const std::string& base64_string)
    {
        byte_vector_t out;

        size_t i = 0;

        for(auto it = base64_string.begin(); it != base64_string.end(); ++it)
        {
            char c = *it;
            char d = 0;

            if(c >= 'a' && c <= 'z')
            {
                d = c - 'a' + 26;
            }
            else if(c >= 'A' && c <= 'Z')
            {
                d = c - 'A';
            }
            else if(c >= '0' && c <= '9')
            {
                d = c - '0' + 52;
            }
            else if(c == '/')
            {
                d = 63;
            }
            else if(c == '+')
            {
                d = 62;
            }
            else if(c == '=')
            {
                d = 0;
            }
            else if(c == '\n')
            {
                continue;
            }
            else
            {
                return std::nullopt;
            }

            switch(i % 4)
            {
            case 0:
                out.push_back(d << 2);
                break;
            case 1:
                out.back() |= (d >> 4);
                out.push_back(d << 4);
                break;
            case 2:
                out.back() |= (d >> 2);
                out.push_back(d << 6);
                break;
            case 3:
                out.back() |= d;
                break;
            }

            ++i;
        }

        return out;
    }

    std::string byte_vector_to_hex(const byte_vector_t& bytes)
    {
        std::stringstream hex_stream;

        auto byte_to_char = [](byte_t b) -> char
        {
            if(b < 10)
            {
                return static_cast<char>(b + '0');
            }
            return static_cast<char>((b - 10) + 'a');
        };

        for(auto b : bytes)
        {
            auto upper = (b & 0xF0) >> 4;
            auto lower = (b & 0x0F);

            hex_stream << byte_to_char(upper) << byte_to_char(lower);
        }

        return hex_stream.str();
    }

    std::string byte_vector_to_base64(const byte_vector_t& bytes)
    {
        std::stringstream b64_stream;

        auto byte_to_b64_char = [](const byte_t b) -> char
        {
            if(b >= 0 && b <= 25)
            {
                return static_cast<char>(b + 'A');
            }
            else if(b >= 26 && b <= 51)
            {
                return static_cast<char>((b - 26) + 'a');
            }
            else if(b >= 52 && b <= 61)
            {
                return static_cast<char>((b - 52) + '0');
            }
            else if(b == 62)
            {
                return '+';
            }
            else
            {
                return '/';
            }
        };

        for(size_t i = 0; i < bytes.size(); i++)
        {
            byte_t byte;

            switch(i % 3)
            {
                case 0:
                    b64_stream << byte_to_b64_char(bytes[i] >> 2);
                    if(i == (bytes.size() - 1))
                    {
                        b64_stream << byte_to_b64_char(((bytes[i] & 0x03) << 4)) << "==";
                    }
                break;
                case 1:
                    b64_stream << byte_to_b64_char(((bytes[i-1] & 0x03) << 4) | ((bytes[i] & 0xF0) >> 4));
                    if(i == (bytes.size() - 1))
                    {
                        b64_stream << byte_to_b64_char(((bytes[i] & 0x0F) << 2)) << "=";
                    }
                break;
                case 2:
                    b64_stream << byte_to_b64_char(((bytes[i-1] & 0x0F) << 2) | (bytes[i] >> 6)) << byte_to_b64_char(bytes[i] & 0x3F);
                break;
            }
        }

        return b64_stream.str();
    }
}