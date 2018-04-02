#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionDisconnect->setEnabled(false);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::enter);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::connectToServer);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::disconnectClient);
    //connect(ui->pushButtonConnect, &QPushButton::clicked, this, &MainWindow::disconnectClient);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::disconnectClient()
{
    ui->actionDisconnect->setEnabled(false);
    ui->actionConnect->setEnabled(true);

    tcpSocket->disconnectFromHost();
}

void MainWindow::enter()
{
    //ui->textBrowser->append(ui->textEdit->toPlainText());
    QString msg = ui->textEdit->toPlainText();
    if (msg != "")
    {
    msg.prepend(userName + ": ");
    msg.prepend(QTime::currentTime().toString() + "     ");
    qDebug() << msg;
    //char test2[255];
    //memset(test2, '\0', 255);
    //memcpy(test2, msg.toStdString().c_str() ,msg.size());
    //qDebug() << test2;
    qDebug() << tcpSocket->write(msg.toStdString().c_str());
    tcpSocket->flush();
    ui->textEdit->setText("");
    }

}

void MainWindow::readyRead()
{
    //qDebug() << tcpSocket->readAll();
    ui->textBrowser->append(tcpSocket->readAll());
}

void MainWindow::connectToServer()
{
    ui->actionDisconnect->setEnabled(true);
    ui->actionConnect->setEnabled(false);

    bool ok;
    ip = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("IP Address:"), QLineEdit::Normal, "192.168.0.13", &ok);
    if (ok && !ip.isEmpty())
    {
        //ui->textBrowser->append(ip);
    }

    bool ok2;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("Port number:"), QLineEdit::Normal, "9002", &ok2);
    if (ok2 && !text.isEmpty())
    {
        port = text.toInt();
        //ui->textBrowser->append(port);
    }

    bool ok3;
    userName = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("User name:"), QLineEdit::Normal, QDir::home().dirName(), &ok3);
    if (ok3 && !userName.isEmpty())
    {
        //ui->textBrowser->append(userName);
    }

    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress(ip), port);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QString hi = userName + " has connected";
    tcpSocket->write(hi.toStdString().c_str());
    qDebug() << port;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, NULL,
                                                                    tr("Are you sure?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
        } else {
            QString txt = userName + " has disconnected";
            qDebug() << tcpSocket->write(txt.toStdString().c_str());
            event->accept();
        }
}
