#include "fileagent.h"
#include <QtCore/qdebug.h>
#include <qdir.h>
#include "Util.h"

FileAgent::FileAgent() {


}
using json = nlohmann::json;
void FileAgent::jsonTest(){
    // JSON string to be parsed
    /*std::string jsonString = R"({"service": "test", "password": "very secure password"})";

    // Parse the JSON string
    json j = json::parse(jsonString);*/

    // Get the home directory path and create the full file path
    QString homeDir = QDir::homePath();
    QString filePath = homeDir + "/workspace/letsdev-git/private/CipherNest/test.json";

    // Open the file for writing
    QFile savefile(filePath);
    if (!savefile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Handle file opening error
        qDebug() << "Error opening file for writing.";
        return;
    }

    // Create a QTextStream to write to the file
    QTextStream streamout(&savefile);

    // Write the JSON content to the file (dumping the JSON into a string)
    streamout << QString::fromStdString(jsonContent.dump(4));  // Pretty print with an indent of 4

    // Close the file
    savefile.close();
}

bool FileAgent::openJson(QString path, QString masterPassword){
    QFile filePath (path);
    qDebug() << "Will open file with mp: " << masterPassword;
    if(filePath.open(QIODevice::ReadOnly)) {;
        QByteArray contents = filePath.readAll();
        qDebug() << contents;
        std::string convertedString = contents.toStdString();
        qDebug() << "File Size read: " << convertedString.size();
        std::vector <unsigned char> encryptedValue{convertedString.begin(), convertedString.end()};
        auto key = encryptionService.getKeyFromPassword(masterPassword);
        qDebug() << "Will open file with converted mp: " << key;
        std::vector <unsigned char> decrypted{};
        encryptionService.decrypt(key, encryptedValue, decrypted);
        std::string decryptedString{decrypted.begin(), decrypted.end()};
        qDebug() << decryptedString;
        jsonContent = json::parse(decryptedString);
        filePath.close();
        return true;
    }
    return false;

}

void FileAgent::addService(QString serviceName, QString password){
    jsonContent[serviceName.toStdString()] = password.toStdString();
}

void FileAgent::readJson(QListWidget* servicelist, QListWidget* passwordlist){
    // Iterate over the JSON object and print all keys and values
    for (auto & element : jsonContent.items()) {
        QString key = QString::fromStdString(element.key());
        QString value = QString::fromStdString(element.value());
        qDebug() << key + "\n" + value;
        Util::addToList(key, value, servicelist, passwordlist);
    }
}
bool FileAgent::writeJson(QString filePath, QString masterPassword){
    QFile file(filePath);
    qDebug() << "Will seave file with mp: " << masterPassword;
    std::string jsonContentString = jsonContent.dump(4);
    std::vector <unsigned char> valueToEncrypt{jsonContentString.begin(), jsonContentString.end()};
    auto key = encryptionService.getKeyFromPassword(masterPassword);
    qDebug() << "Will seave file with coverted mp: " << key;
    std::vector <unsigned char> encrypted{};
    encryptionService.encrypt(key, valueToEncrypt, encrypted);
    std::string encryptedString{encrypted.begin(), encrypted.end()};
    qDebug() << "File Size written: " << encryptedString.size();

    if(file.open(QIODevice::WriteOnly)){

        const char* data = encryptedString.c_str();
        qint64 totalBytes = static_cast<qint64>(encryptedString.size());
        qint64 bytesWritten = 0;

        while (bytesWritten < totalBytes) {
            qint64 result = file.write(data + bytesWritten, totalBytes - bytesWritten);
            if (result == -1) {
                qDebug() << "Error writing to file:" << file.errorString();
                file.close();
                return false;
            }
            bytesWritten += result;
        }

        file.flush();
        file.close();
        return true;
    }
    return false;
}
