#ifndef FILEAGENT_H
#define FILEAGENT_H
#include <nlohmann/json.hpp>
#include <QIODevice>
#include <QFile>
#include <QFileDevice>
#include <QListWidget>
#include <QDir>
#include <encryptionservice.h>
#define homedir QDir::homePath()

class FileAgent
{
    nlohmann::json jsonContent{};
    cipher_nest::EncryptionService encryptionService{};
public:
    void jsonTest();
    bool openJson(QString path, QString masterPassword);
    void addService(QString serviceName, QString password);
    void readJson(QListWidget* servicelist, QListWidget* passwordlist);
    bool writeJson(QString filePath, QString masterPassword);
    FileAgent();
};

#endif // FILEAGENT_H
