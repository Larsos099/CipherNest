#ifndef ENCRYPTIONSERVICE_H
#define ENCRYPTIONSERVICE_H
#include <vector>
#include <string>

namespace cipher_nest{
    class EncryptionService
    {
        std::string masterKey{};

        std::vector<unsigned char> generateRandomData(int size);

        bool decrypt(const std::vector<unsigned char> &key, const std::vector<unsigned char> &value, std::vector<unsigned char> &result);

        bool encrypt(const std::vector<unsigned char> &key, const std::vector<unsigned char> &value, std::vector<unsigned char> &result);
    public:
        EncryptionService(const std::string &masterKeyPath, const std::string &userPassword);
    };
}
#endif // ENCRYPTIONSERVICE_H
