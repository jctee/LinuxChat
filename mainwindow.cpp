#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButtonConnect, &QPushButton::clicked, this, &MainWindow::connectToServer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
        c->show();
    }

}

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
