/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: client.cpp - Application that connects to the chat server.
--
-- PROGRAM: inotd
--
-- FUNCTIONS:
-- void disconnectClient()
-- void saveChat()
-- void enter()
-- void readyRead()
-- void closeEvent(QCloseEvent* event)
--
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- NOTES:
-- This program instantiates a tcp socket that connects to the given ip and port as the username given. Gives the program
-- for 1 second to connect to the server before timing out. After connecting to the server, shows other users who
-- connect to the chatroom and displays all messages received from the server along with who sent the message and
-- the time they sent it. Also allows the user to save the chat log into a file.
----------------------------------------------------------------------------------------------------------------------*/
#include "client.h"
#include "ui_client.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Client Constructor
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: Client(QString ip, int port, QString userName)
--
-- RETURNS: N/A
--
-- NOTES:
-- Call this function from MainWindow when user wants to connect to a server.
-- Connects to the server with the given ip, port, and userName.
-- Connects slots and signals of the buttons of the GUI.
----------------------------------------------------------------------------------------------------------------------*/
Client::Client(QWidget *parent, QString ip, int port, QString userName) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    this->userName = userName;
    ui->lineEdit->setMaxLength(255 - 13 - userName.length());

    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress(ip), port);
    if(tcpSocket->waitForConnected(1000))
    {
        QString connectMessage = userName + " has connected";
        tcpSocket->write(connectMessage.toStdString().c_str());
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        this->show();
    }
    else
    {
        QMessageBox::information(this, NULL, tr("Connection failed\n"));
    }
    connect(ui->pushButton, &QPushButton::clicked, this, &Client::enter);
    connect(ui->actionDisconnect, &QAction::triggered, this, &Client::disconnectClient);
    connect(ui->actionSaveChat, &QAction::triggered, this, &Client::saveChat);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &Client::enter);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: disconnectClient
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: void disconnectClient (void)
--
-- RETURNS: void.
--
-- NOTES:
-- Call this function when the user presses "disconnect" or when the program is closed and disconnects from the server.
----------------------------------------------------------------------------------------------------------------------*/
void Client::disconnectClient()
{
    QString txt = userName + " has disconnected";
    qDebug() << tcpSocket->write(txt.toStdString().c_str());
    tcpSocket->disconnectFromHost();
    ui->textBrowser->append(userName + " has disconnected");
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: saveChat
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: void saveChat (void)
--
-- RETURNS: void.
--
-- NOTES:
-- Call this function when the user presses "save file". Asks the user to enter in a file name to save the chat log to,
-- and saves the chat log into that file.
----------------------------------------------------------------------------------------------------------------------*/
void Client::saveChat()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text file(*.txt"));
    outputFile.open(fileName.toStdString());
    outputFile << ui->textBrowser->toPlainText().toStdString();
    outputFile.close();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: enter
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: void enter (void)
--
-- RETURNS: void.
--
-- NOTES:
-- Call this function when the user presses the "enter" key. Sends the message in the line if it's not empty and
-- clears the current line to allow the user to enter a new message.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: readyRead
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: void readyRead (void)
--
-- RETURNS: void.
--
-- NOTES:
-- Slot that receives a signal when there is something in the tcp socket buffer. This function automatically appends
-- the received message to the chat log.
----------------------------------------------------------------------------------------------------------------------*/
void Client::readyRead()
{
    ui->textBrowser->append(tcpSocket->readAll());
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: closeEvent
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: void closeEvent (QCloseEvent* event)
--
-- RETURNS: void.
--
-- NOTES:
-- Call this function when the user presses the X on the top right of a window to try and close the window.
-- Asks the user for confirmation, pressing Yes closes the window, keeps the window open otherwise.
----------------------------------------------------------------------------------------------------------------------*/
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
        event->accept();
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Client Destructor
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: ~Client()
--
-- RETURNS: N/A
--
-- NOTES:
-- Automatically call this function when Client is destroyed and deletes the ui.
----------------------------------------------------------------------------------------------------------------------*/
Client::~Client()
{
    delete ui;
}
