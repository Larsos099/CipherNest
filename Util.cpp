#include "Util.h"

void Util::addToList(QString service, QString password, QListWidget * servicelist, QListWidget *passwordlist, FileAgent *fileAgent){
    servicelist->addItem(service);
    passwordlist->addItem(password);
    if (fileAgent){
        fileAgent->addService(service, password);
    }
    return;
}
