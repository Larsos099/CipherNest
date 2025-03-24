#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encryptionservice.h"
#include <QFileDialog>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Listen mit Passwörtern und so
    servicelist = ui->Services;
    passwordlist = ui->Passwords;
    removeentry = ui->remove_entry;
    searchentry = ui->search_entry;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_new_entry_clicked()
{
    QMessageBox::StandardButton reply;
    QString password;
    QInputDialog inDiag;
    QMessageBox msgbox;
    QString service = inDiag.getText(this, "Service", "Service Name:");
    password = inDiag.getText(this, "Password", "Password:");
    if(password.length() < 8){
        reply = msgbox.question(this, "Insecure Password", "Are you sure you want to use a password under 8 Characters?",
                                QMessageBox::Yes|QMessageBox::No);
    }
    else{
        reply = {};
        Util::addToList(service, password, servicelist, passwordlist, &fileAgent);
    }
    if(reply == QMessageBox::Yes){
        Util::addToList(service, password, servicelist, passwordlist, &fileAgent);
    }
    else if(reply == QMessageBox::No){
        on_new_entry_clicked();
    }
}


void MainWindow::on_actionCipherNest_schlie_en_triggered()
{
    QApplication::quit();
}


void MainWindow::on_change_entry_clicked()
{
    bool found = false;
    QMessageBox::StandardButton reply = {};
    QList<QListWidgetItem*> foundItems;
    QListWidgetItem* itemtochange;
    QInputDialog qInDiag;
    QMessageBox qMsgBox;
    QString entryToChange, newPassword = {};
    entryToChange = qInDiag.getText(this, "Entry Select", "Which Entry's Password should be changed?");
    foundItems = servicelist->findItems(entryToChange, Qt::MatchContains);
    for(int i = 0; i < foundItems.size(); i++){
        QString mod = entryToChange.toLower();
        QString modFound = foundItems[i]->text().toLower();
        if(mod == modFound){
            found = true;
            itemtochange = foundItems[i];
            break;
        }
    }

    if(found){
        int row = servicelist->row(itemtochange);
        newPassword = qInDiag.getText(this, "Set New Password for " + itemtochange->text(), "New Password: ");
        passwordlist->item(row)->setText(newPassword);
    }
    else{

        reply = qMsgBox.critical(this, "Entry not found.", "The entry named " + entryToChange +" wasnt found. Try again?",
                                 QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            on_change_entry_clicked();
        }
        else{
            return;
        }
    }

}




void MainWindow::on_action_ffnen_triggered()

{   QFileDialog explorer;
    QMessageBox::StandardButton reply = {};
    QMessageBox qMsgBox;
    QString fln = explorer.getOpenFileName(this, "Wähle eine Speicherdatei aus", homedir);
    QInputDialog qInDiag;
    QString masterPassword = {};
    masterPassword = qInDiag.getText(this, "Enter Master-Password", "Master-Password: ");
    if(fileAgent.openJson(fln, masterPassword)){
        fileAgent.readJson(servicelist, passwordlist);
    }
    else{
        reply = qMsgBox.critical(this, "Opening File failed", "Opening file failed.. Try Again?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            on_action_ffnen_triggered();
        }
        else{
            return;
        }
    }

}


void MainWindow::on_actionSpeichern_triggered()
{
    QMessageBox::StandardButton reply = {};
    QMessageBox qMsgBox;
    QFileDialog explorer;
    QInputDialog qInDiag;
    QString masterPassword = {};
    QString fln = explorer.getSaveFileName(this, "Wähle einen Speicherort aus", homedir);
    qDebug() << homedir;
    masterPassword = qInDiag.getText(this, "Enter Master-Password", "Master-Password: ");
    if(fileAgent.writeJson(fln, masterPassword)){
        reply = qMsgBox.information(this, "File Saved successfully!", "File saved successfully");
    }
    else {
        reply = qMsgBox.critical(this, "File Saving failed", "File Saving failed... Try Again?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            on_actionSpeichern_triggered();
        }
        else{
            return;
        }
    }
}


void MainWindow::on_remove_entry_clicked()
{
    bool found = false;
    QMessageBox qMsgBox;
    QInputDialog qInDiag;
    QString entry = qInDiag.getText(this, "Which Entry should be removed?", "Entry Service Name: ");
    QList<QListWidgetItem*> foundEntrys;
    QListWidgetItem* entryToRemove;
    foundEntrys = servicelist->findItems(entry, Qt::MatchContains);
    for(int i = 0; i < foundEntrys.size(); i++){
        QString mod = entry.toLower();
        QString modFound = foundEntrys[i]->text().toLower();
        if( mod == modFound){
           entryToRemove = foundEntrys[i];
            found = true;

        }
    }

    if(found){
        int row = servicelist->row(entryToRemove);
        delete servicelist->takeItem(row);
        delete passwordlist->takeItem(row);
        passwordlist->update(); servicelist->update();
    }
    else{
        auto reply = qMsgBox.information(this, "Entry not found.", "Entry " + entry + "not found, try again?",
                                      QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) MainWindow::on_remove_entry_clicked();
        else return;
    }
}


void MainWindow::on_search_btn_clicked()
{
    bool found = false;
    QString toSearchFor = searchentry->text();
    QList<QListWidgetItem*> foundEntries;
    QListWidgetItem* SearchedEntry;
    foundEntries = servicelist->findItems(toSearchFor, Qt::MatchContains);
    for(int i = 0; i < foundEntries.size(); i++){
        QString mod = toSearchFor.toLower();
        QString modFound = foundEntries[i]->text().toLower();
        if(mod == modFound){
            SearchedEntry = foundEntries[i];
            found = true;
        }
    }
    if(found){
        servicelist->setCurrentItem(SearchedEntry);
        QListWidgetItem* passwordEqiv = passwordlist->item(servicelist->row(SearchedEntry));
        passwordlist->setCurrentItem(passwordEqiv);
        servicelist->scrollToItem(SearchedEntry);
        passwordlist->scrollToItem(passwordEqiv);
        servicelist->update(); passwordlist->update();
    }
}
