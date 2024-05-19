#include <cryptopals/aes.hpp>
#include <cryptopals/xor.hpp>
#include <memory>
#include <random>
#include <set>

namespace cpl
{
    using evp_cipher_ctx_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

    void random_aes_key(size_t key_size, byte_vector_t& key)
    {
        std::mt19937_64 generator;
        generator.seed(std::time(nullptr));

        key.resize(key_size);

        for(size_t i = 0; i < key_size; i++)
        {
            key[i] = static_cast<byte_t>(generator());
        }
    }

    bool is_ecb_encrypted(const byte_vector_view_t& in, size_t key_size, size_t padding)
    {
        std::set<byte_vector_view_t> blocks;

        for(size_t i = padding; i < in.size(); i += key_size)
        {
            byte_vector_view_t view(in.begin() + i, key_size);

            if(blocks.find(view) != blocks.end())
            {
                return true;
            }

            blocks.insert(view);
        }

        return false;
    }

    bool aes_encryption_oracle(const byte_vector_view_t& in, byte_vector_t& out, bool& is_ecb_encrypted, size_t block_size)
    {
        byte_vector_t key;
        random_aes_key(block_size, key);

        std::srand(std::time(nullptr));

        byte_vector_t prefix, suffix;
        random_aes_key((std::rand() % 5) + 5, prefix);
        random_aes_key((std::rand() % 5) + 5, suffix);

        byte_vector_t padded_input = cpl::pad_pkcs7(prefix + in.data() + suffix, block_size);

        size_t size = padded_input.size();
        size_t rand = std::rand();
        is_ecb_encrypted = rand % 2 == 0;

        if(!is_ecb_encrypted)
        {
            byte_vector_t iv;
            random_aes_key(block_size, iv);
            return aes_ecb_cbc_encrypt(padded_input, key, iv, out);
        }
        else
        {
            return aes_ecb_encrypt(padded_input, key, out);
        }
    }

    bool aes_ecb_cbc_decrypt(const byte_vector_view_t& in, const byte_vector_t& key, const byte_vector_t& iv, byte_vector_t& out)
    {
        const size_t block_size = key.size();

        out.clear();
        byte_vector_t last(iv);

        for(size_t i = 0; i < in.size(); i += block_size)
        {
            auto block_view = byte_vector_view_t(in.data() + i, block_size);

            byte_vector_t xor_encrypted;

            if(!aes_ecb_decrypt(block_view, key, xor_encrypted))
            {
                return false;
            }

            byte_vector_t xor_decrypted;
            xor_encrypt(xor_encrypted, last, xor_decrypted);

            last = block_view;

            out.append(xor_decrypted.begin(), xor_decrypted.end());
        }

        return true;
    }

    bool aes_ecb_decrypt(const byte_vector_view_t& in, const byte_vector_t& key, byte_vector_t& out)
    {
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings();
        int length = 0;
        out.resize(in.size());

        evp_cipher_ctx_ptr cipher_ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
        if(!cipher_ctx)
        {
            return false;
        }

        if(!EVP_DecryptInit_ex(cipher_ctx.get(), EVP_aes_128_ecb(), NULL, reinterpret_cast<const unsigned char*>(key.data()), NULL))
        {
            return false;
        }

        if(!EVP_CIPHER_CTX_set_padding(cipher_ctx.get(), 0))
        {
            return false;
        }        

        if(!EVP_DecryptUpdate(cipher_ctx.get(), reinterpret_cast<unsigned char*>(out.data()), &length, reinterpret_cast<const unsigned char*>(in.data()), in.size()))
        {
            return false;
        }

        out.resize(length);
        if(!EVP_DecryptFinal(cipher_ctx.get(), reinterpret_cast<unsigned char*>(out.data()), &length))
        {
            return false;
        }

        return true;
    }

    bool aes_ecb_decrypt(const byte_vector_t& in, const byte_vector_t& key, byte_vector_t& out)
    {
        return aes_ecb_decrypt(byte_vector_view_t(in), key, out);
    }

    bool aes_ecb_cbc_encrypt(const byte_vector_view_t& in, const byte_vector_t& key, const byte_vector_t& iv, byte_vector_t& out)
    {
        const size_t block_size = key.size();

        out.clear();
        byte_vector_t last(iv);

        for(size_t i = 0; i < in.size(); i += block_size)
        {
            auto block_view = byte_vector_view_t(in.data() + i, block_size);
            byte_vector_t xor_encrypted;
            cpl::xor_encrypt(block_view, last, xor_encrypted);

            if(!aes_ecb_encrypt(xor_encrypted, key, last))
            {
                return false;
            }

            out.append(last.begin(), last.end());
        }

        return true;
    }

    bool aes_ecb_encrypt(const byte_vector_t& in, const byte_vector_t& key, byte_vector_t& out)
    {
        return aes_ecb_encrypt(byte_vector_view_t(in), key, out);
    }

    bool aes_ecb_encrypt(const byte_vector_view_t& in, const byte_vector_t& key, byte_vector_t& out)
    {
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings();
        int length = 0;
        out.resize(in.size());

        evp_cipher_ctx_ptr cipher_ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
        if(!cipher_ctx)
        {
            return false;
        }

        if(!EVP_EncryptInit_ex(cipher_ctx.get(), EVP_aes_128_ecb(), NULL, reinterpret_cast<const unsigned char*>(key.data()), NULL))
        {
            return false;
        }

        if(!EVP_CIPHER_CTX_set_padding(cipher_ctx.get(), 0))
        {
            return false;
        }        

        if(!EVP_EncryptUpdate(cipher_ctx.get(), reinterpret_cast<unsigned char*>(out.data()), &length, reinterpret_cast<const unsigned char*>(in.data()), in.size()))
        {
            return false;
        }

        out.resize(length);
        if(!EVP_EncryptFinal(cipher_ctx.get(), reinterpret_cast<unsigned char*>(out.data()), &length))
        {
            return false;
        }

        return true;
    }

}