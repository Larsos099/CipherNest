#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encryptionservice.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cipher_nest::EncryptionService("", "");

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
    // WIP
}


