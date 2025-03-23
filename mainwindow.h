#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include "fileagent.h"
#include "Util.h"
#include <QDir>
#define homedir QDir::homePath()
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_new_entry_clicked();

    void on_actionCipherNest_schlie_en_triggered();

    void on_change_entry_clicked();

    void on_action_ffnen_triggered();

    void on_actionSpeichern_triggered();

    void on_remove_entry_clicked();

private:
    QPushButton* entrysearch;
    QListWidget* servicelist;
    QListWidget* passwordlist;
    Ui::MainWindow *ui;
    FileAgent fileAgent{};
};
#endif // MAINWINDOW_H
