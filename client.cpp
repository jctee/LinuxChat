#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent, QString ip, int port, QString userName) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    this->userName = userName;
    ui->lineEdit->setMaxLength(255 - 13 - userName.length());

    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress(ip), port);
    QString connectMessage = userName + " has connected";
    tcpSocket->write(connectMessage.toStdString().c_str());

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(ui->pushButton, &QPushButton::clicked, this, &Client::enter);
    connect(ui->actionDisconnect, &QAction::triggered, this, &Client::disconnectClient);
    connect(ui->actionSaveChat, &QAction::triggered, this, &Client::saveChat);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &Client::enter);
}

void Client::disconnectClient()
{
    tcpSocket->disconnectFromHost();
    ui->textBrowser->append(userName + " has disconnected");
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
    QString msg = ui->lineEdit->displayText();
    if (msg != "")
    {
        msg.prepend(userName + ": ");
        msg.prepend(QTime::currentTime().toString() + " - ");
        tcpSocket->write(msg.toStdString().c_str());
        tcpSocket->flush();
        ui->lineEdit->setText("");
    }
}

void Client::readyRead()
{
    ui->textBrowser->append(tcpSocket->readAll());
}

void Client::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, NULL, tr("Are you sure?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
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
