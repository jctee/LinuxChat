/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.cpp - An application that is the main menu to allow the user to connect
--
-- PROGRAM: inotd
--
-- FUNCTIONS:
-- void connectToServer()
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
-- This program has a button that starts a new connection to a server after providing the ip address,
-- port number, and a username. After entering all the input, mainwindow instantiates a new client window that
-- connects to the server corresponding to the entered ip address and port number as the username provided.
----------------------------------------------------------------------------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MainWindow Constructor
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: MainWindow()
--
-- RETURNS: N/A
--
-- NOTES:
-- Automatically call this function from main to open the main window into the program.
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButtonConnect, &QPushButton::clicked, this, &MainWindow::connectToServer);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MainWindow Destructor
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: ~MainWindow()
--
-- RETURNS: N/A
--
-- NOTES:
-- Automatically call this function when MainWindow is destroyed and deletes the ui.
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: connectToServer
--
-- DATE: April 4 2018
--
-- REVISIONS: None
--
-- DESIGNER: John Tee & Li Tong
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: void connectToServer (void)
--
-- RETURNS: void.
--
-- NOTES:
-- Call this function when the user presses "connect". Asks the user for ip address, port number, and username.
-- if the user presses cancel on any of them, doesn't try to connect at all. If the user presses ok on all of them,
-- the program instantiates a client that tries to connect to the server.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::connectToServer()
{
    bool ok;
    ip = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("IP Address:"), QLineEdit::Normal, "192.168.0.13", &ok);

    bool ok2;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("Port number:"), QLineEdit::Normal, "9002", &ok2);
    if (ok2 && !text.isEmpty())
    {
        port = text.toInt();
    }

    bool ok3;
    userName = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("User name:"), QLineEdit::Normal, QDir::home().dirName(), &ok3);

    if(ok && ok2 && ok3)
    {
        Client* c = new Client(this, ip, port, userName);
    }

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
void MainWindow::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, NULL, tr("Are you sure?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes)
        {
            event->ignore();
        }
        else
        {
            event->accept();
        }
}
