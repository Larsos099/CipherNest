#include "encryptionservice.h"

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <iostream>

namespace cipher_nest{

    static const auto CIPHER = EVP_aes_256_cbc();
    static const auto BLOCK_SIZE = 16;

    EncryptionService::EncryptionService(const std::string &masterKeyPath, const std::string &userPassword) {

    }

    std::vector<unsigned char> EncryptionService::generateRandomData(int size){
        std::vector<unsigned char> data{};
        data.reserve(size);
        if (RAND_bytes(data.data(), size) != 1) {
            std::cerr << "Could not generate random data for encryption" << std::endl;
        }
        return data;
    }


    bool EncryptionService::encrypt(const std::vector<unsigned char> &key, const std::vector<unsigned char> &value, std::vector<unsigned char> &result){
        std::cout << "Startign encryption" << std::endl;
        // 2 * block size for. 1 we add the ivec later in the front, 2 for block fillup in encryption
        result.reserve(value.size() + 2 * BLOCK_SIZE);

        EVP_CIPHER_CTX *encryptionContext = EVP_CIPHER_CTX_new();
        if(!encryptionContext){
            std::cerr << "Could not create encryption context" << std::endl;
            return false;
        }

        auto iVec = generateRandomData(BLOCK_SIZE);
        // add Ivec intro the result at the beginning for later decryption
        std::copy(iVec.begin(), iVec.end(), result.begin());


        if (!EVP_EncryptInit_ex(encryptionContext, CIPHER, nullptr, key.data(), iVec.data())) {
            std::cerr << "Could not init encryption" << std::endl;
            EVP_CIPHER_CTX_free(encryptionContext);
            return false;
        }


        int encryptionOutLengthPart1 = 0;
        if (!EVP_EncryptUpdate(encryptionContext, result.data() + BLOCK_SIZE, &encryptionOutLengthPart1, value.data(), value.size())) {
            std::cerr << "Could not update encryption" << std::endl;
            EVP_CIPHER_CTX_free(encryptionContext);
            return false;
        }


        int encryptionOutLengthPart2 = 0;
        if (!EVP_EncryptFinal_ex(encryptionContext, result.data() + BLOCK_SIZE + encryptionOutLengthPart1, &encryptionOutLengthPart2)) {
            std::cerr << "Could not finalize encryption" << std::endl;
            EVP_CIPHER_CTX_free(encryptionContext);
            return false;
        }
        result.resize(encryptionOutLengthPart1 + encryptionOutLengthPart2 + BLOCK_SIZE);
        EVP_CIPHER_CTX_free(encryptionContext);

        return true;
    }

    bool EncryptionService::decrypt(const std::vector<unsigned char> &key, const std::vector<unsigned char> &value, std::vector<unsigned char> &result){}
}
