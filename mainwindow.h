#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QInputDialog>
#include <QTime>
#include <QCloseEvent>
#include <QMessageBox>

#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectToServer();
    void closeEvent(QCloseEvent* event);

    QString ip;
    int port;
    QString userName;


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
