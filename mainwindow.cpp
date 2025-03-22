#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    qDebug() << service;
    qDebug() << password;
    servicelist->addItem(service);
    passwordlist->addItem(password);
    qDebug() << servicelist->item(0);
    qDebug() << passwordlist->item(0);
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
        addToList(service, password, servicelist, passwordlist);
    }
    if(reply == QMessageBox::Yes){
        addToList(service, password, servicelist, passwordlist);
    }
    else if(reply == QMessageBox::No){
        on_new_entry_clicked();
    }
}

