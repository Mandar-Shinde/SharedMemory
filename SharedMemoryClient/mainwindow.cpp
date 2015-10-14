#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBuffer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sharedMemory = new QSharedMemory("MSTOKEN");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buRead_clicked()
{
    if(!sharedMemory->isAttached())
    {
        if (!sharedMemory->attach()) {
            ui->leOutput->appendPlainText(tr("Cannot attach Shared Memory for MSTOKEN. (Check if server part is running) "));
            return;
        }
    }

    QBuffer buffer;
    QDataStream in(&buffer);
    char *data;

    sharedMemory->lock();
    buffer.setData((char*)sharedMemory->constData(), sharedMemory->size());
    buffer.open(QBuffer::ReadOnly);
    in >> data;
    sharedMemory->unlock();
    ui->leOutput->appendPlainText("Reading from memory : "+QString::fromLatin1(data));
}
