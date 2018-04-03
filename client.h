#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QtNetwork>
#include <QInputDialog>
#include <QTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0, QString ip = 0, int port = 0, QString userName = 0);
    ~Client();
    void closeEvent(QCloseEvent* event);
    void disconnectClient();
    void saveChat();


    QTcpSocket* tcpSocket;
    QString ip;
    int port;
    QString userName;
    std::ofstream outputFile;

public slots:
    void readyRead();
    void enter();

private:
    Ui::Client *ui;
};

#endif // CLIENT_H
