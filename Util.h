#ifndef UTIL_H
#define UTIL_H
#include <QString>
#include <QListWidget>
#include <fileagent.h>

class Util {
    public:
    static void addToList(QString service, QString password, QListWidget * servicelist, QListWidget *passwordlist, FileAgent *fileAgent = nullptr);
};


#endif // UTIL_H
