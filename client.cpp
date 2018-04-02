#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent, QString ip, int port, QString userName) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    this->userName = userName;
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress(ip), port);
    QString hi = userName + " has connected";
    tcpSocket->write(hi.toStdString().c_str());
    qDebug() << port;
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    ui->lineEdit->setMaxLength(255 - 13 - userName.length());
    connect(ui->pushButton, &QPushButton::clicked, this, &Client::enter);
    //connect(ui->actionConnect, &QAction::triggered, this, &Client::connectToServer);
    connect(ui->actionDisconnect, &QAction::triggered, this, &Client::disconnectClient);
    connect(ui->actionSaveChat, &QAction::triggered, this, &Client::saveChat);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &Client::enter);
    //connect(ui->pushButtonConnect, &QPushButton::clicked, this, &MainWindow::disconnectClient);
}

void Client::disconnectClient()
{
    tcpSocket->disconnectFromHost();
}

void Client::saveChat()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text file(*.txt"));
    outputFile.open(fileName.toStdString());
    outputFile << ui->textBrowser->toPlainText().toStdString();
    outputFile.close();
}

void Client::enter()
{
    //ui->textBrowser->append(ui->textEdit->toPlainText());
    QString msg = ui->lineEdit->displayText();
    if (msg != "")
    {
    msg.prepend(userName + ": ");
    msg.prepend(QTime::currentTime().toString() + " - ");
    qDebug() << msg;
    //char test2[255];
    //memset(test2, '\0', 255);
    //memcpy(test2, msg.toStdString().c_str() ,msg.size());
    //qDebug() << test2;
    qDebug() << tcpSocket->write(msg.toStdString().c_str());
    tcpSocket->flush();
    ui->lineEdit->setText("");
    }
}

void Client::readyRead()
{
    //qDebug() << tcpSocket->readAll();
    ui->textBrowser->append(tcpSocket->readAll());
}

void Client::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, NULL,
                                                                    tr("Are you sure?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
        } else {
            disconnectClient();
            QString txt = userName + " has disconnected";
            qDebug() << tcpSocket->write(txt.toStdString().c_str());
            event->accept();
        }
}

Client::~Client()
{
    delete ui;
}
