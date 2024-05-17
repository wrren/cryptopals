#include <cryptopals/aes.hpp>

namespace cpl
{
    using evp_cipher_ctx_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

    bool aes_ecb_decrypt(const byte_vector_t& in, const byte_vector_t& key, byte_vector_t& out)
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
}