#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
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

private:
    QListWidget* servicelist;
    QListWidget* passwordlist;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
