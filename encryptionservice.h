#ifndef ENCRYPTIONSERVICE_H
#define ENCRYPTIONSERVICE_H
#include <vector>
#include <string>
#include <QString>
namespace cipher_nest{
    class EncryptionService
    {
        std::string masterKey{};

        std::vector<unsigned char> generateRandomData(int size);

    public:

        std::vector<unsigned char> getKeyFromPassword(QString password);

        bool decrypt(const std::vector<unsigned char> &key, const std::vector<unsigned char> &value, std::vector<unsigned char> &result);

        bool encrypt(const std::vector<unsigned char> &key, const std::vector<unsigned char> &value, std::vector<unsigned char> &result);

        EncryptionService();
    };
}
#endif // ENCRYPTIONSERVICE_H
