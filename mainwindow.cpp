#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encryptionservice.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Listen mit Passwörtern und so
    servicelist = ui->Services;
    passwordlist = ui->Passwords;

}

MainWindow::~MainWindow()
{
    delete ui;
}
void addToList(QString service, QString password, QListWidget* servicelist, QListWidget* passwordlist){
    servicelist->addItem(service);
    passwordlist->addItem(password);;
    return;
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
        addToList(service, password, servicelist, passwordlist);
    }
    if(reply == QMessageBox::Yes){
        addToList(service, password, servicelist, passwordlist);
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


void MainWindow::on_enc_test_clicked()
{
    QMessageBox::StandardButton reply = {};
    QString input = {};
    QString enc = {};
    std::string inputAsSTDString = {};
    QInputDialog qInDiag;
    QMessageBox qMsgBox;
    input = qInDiag.getText(this, "Test Encryption", "Text to Encrypt: ");
    inputAsSTDString = input.toStdString();
    cipher_nest::EncryptionService("", inputAsSTDString, enc);
    reply = qMsgBox.information(this, "Encryption/Decryption Completed", "Text Before: " + input + "\n After: " + enc);
    qDebug() << "\n\n [!] ENCRYPTION TEST\n";
    qDebug() << "QString passed out of function: " + enc;
}

