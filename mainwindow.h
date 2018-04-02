#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QInputDialog>
#include <QTime>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void enter();
    void connectToServer();
    void closeEvent(QCloseEvent* event);
    void disconnectClient();


    QTcpSocket* tcpSocket;
    QString ip;
    int port;
    QString userName;

public slots:
    void readyRead();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
