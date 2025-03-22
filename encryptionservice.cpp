#include "encryptionservice.h"

#include <vector>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <QDebug>

namespace cipher_nest{

    static const auto CIPHER = EVP_aes_256_cbc();
    static constexpr auto BLOCK_SIZE = 16;
    static constexpr auto KEY_SIZE = 32;

    EncryptionService::EncryptionService(const std::string &masterKeyPath, const std::string &userPassword) {
        // TDOD load master key from file and decrypt with password
        // TODO load actual passwords and decryp with master key


        // TODO remove test code below
        auto key = generateRandomData(KEY_SIZE);

        std::string testString = "I will be necypted and decrypted";
        std::vector<unsigned char> testStringAsVector(testString.begin(), testString.end());

        qDebug() << "Before encryptio: " << testString;

        std::vector<unsigned char> encryptedVector{};
        encrypt(key, testStringAsVector, encryptedVector);
        std::string encryptedString(encryptedVector.begin(), encryptedVector.end());
        qDebug() << "After encryptio: " << encryptedString;


        std::vector<unsigned char> decryptedVector{};
        decrypt(key, encryptedVector, decryptedVector);
        std::string decryptedString(decryptedVector.begin(), decryptedVector.end());
        qDebug() << "After decryption: " << decryptedString;

    }

    std::vector<unsigned char> EncryptionService::generateRandomData(int size){
        std::vector<unsigned char> data{};
        data.resize(size);
        if (RAND_bytes(data.data(), size) != 1) {
            qCritical() << "Could not generate random data for encryption";
        }
        return data;
    }


    bool EncryptionService::encrypt(const std::vector<unsigned char> &key, const std::vector<unsigned char> &value, std::vector<unsigned char> &result){
        qDebug() << "Startign encryption";
        // 2 * block size for. 1 we add the ivec later in the front, 2 for block fillup in encryption
        result.resize(value.size() + 2 * BLOCK_SIZE);

        EVP_CIPHER_CTX *encryptionContext = EVP_CIPHER_CTX_new();
        if(!encryptionContext){
            qCritical() << "Could not create encryption context";
            return false;
        }

        auto iVec = generateRandomData(BLOCK_SIZE);
        // add Ivec intro the result at the beginning for later decryption
        std::copy(iVec.begin(), iVec.end(), result.begin());


        if (!EVP_EncryptInit_ex(encryptionContext, CIPHER, nullptr, key.data(), iVec.data())) {
            qCritical() << "Could not init encryption";
            EVP_CIPHER_CTX_free(encryptionContext);
            return false;
        }


        int encryptionOutLengthPart1 = 0;
        if (!EVP_EncryptUpdate(encryptionContext, result.data() + BLOCK_SIZE, &encryptionOutLengthPart1, value.data(), value.size())) {
            qCritical() << "Could not update encryption";
            EVP_CIPHER_CTX_free(encryptionContext);
            return false;
        }


        int encryptionOutLengthPart2 = 0;
        if (!EVP_EncryptFinal_ex(encryptionContext, result.data() + BLOCK_SIZE + encryptionOutLengthPart1, &encryptionOutLengthPart2)) {
            qCritical() << "Could not finalize encryption";
            EVP_CIPHER_CTX_free(encryptionContext);
            return false;
        }
        result.resize(encryptionOutLengthPart1 + encryptionOutLengthPart2 + BLOCK_SIZE);
        EVP_CIPHER_CTX_free(encryptionContext);

        return true;
    }

    bool EncryptionService::decrypt(const std::vector<unsigned char> &key, const std::vector<unsigned char> &value, std::vector<unsigned char> &result){
        qDebug() << "Startign decryption";
        std::vector<unsigned char> iVec{};
        iVec.resize(BLOCK_SIZE);
        std::copy(value.begin(), value.begin() + BLOCK_SIZE, iVec.begin());
        // value is alwas bigger than the decrypted text
        result.resize(value.size());


        EVP_CIPHER_CTX *encryptionContext = EVP_CIPHER_CTX_new();
        if(!encryptionContext){
            qCritical() << "Could not create encryption context";
            return false;
        }

        if (!EVP_DecryptInit_ex(encryptionContext, CIPHER, nullptr, key.data(), iVec.data())) {
            qCritical() << "Could not init decryption";
            EVP_CIPHER_CTX_free(encryptionContext);
            return false;
        }

        int decryptionOutLengthPart1 = 0;
        if (!EVP_DecryptUpdate(encryptionContext, result.data(), &decryptionOutLengthPart1, value.data() + BLOCK_SIZE, value.size() - BLOCK_SIZE)) {
            qCritical() << "Could not update decryption";
            EVP_CIPHER_CTX_free(encryptionContext);
            return false;
        }

        int decryptionOutLengthPart2 = 0;
        if (!EVP_DecryptFinal_ex(encryptionContext, result.data()  + decryptionOutLengthPart1, &decryptionOutLengthPart2)) {
            qCritical() << "Could not finalize encryption";
            EVP_CIPHER_CTX_free(encryptionContext);
            return false;
        }
        result.resize(decryptionOutLengthPart1 + decryptionOutLengthPart2);
        EVP_CIPHER_CTX_free(encryptionContext);
        return true;
    }
}
