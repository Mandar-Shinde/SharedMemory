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

void MainWindow::on_buSave_clicked()
{
    // Load into shared memory
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);

    out <<  ui->leInput->toPlainText().toUtf8().constData();
    int size = buffer.size();
    if(!sharedMemory->create(size))
    {
        switch(sharedMemory->error())
        {
        case 0:	 ui->leSaved->appendPlainText(tr("No error occurred.")); break;
        case 1:	 ui->leSaved->appendPlainText(tr("The operation failed because the caller didn't have the required permissions.")); break;
        case 2:	 ui->leSaved->appendPlainText(tr("A create operation failed because the requested size was invalid.")); break;
        case 3:	 ui->leSaved->appendPlainText(tr("The operation failed because of an invalid key.")); break;
        case 4:	 ui->leSaved->appendPlainText(tr("Shared memory segment with the specified key already existed, So overriting with new values"));
                    sharedMemory->attach();
                    break;
        case 5:	 ui->leSaved->appendPlainText(tr("An attach() failed because a shared memory segment with the specified key could not be found."));break;
        case 6:	 ui->leSaved->appendPlainText(tr("The attempt to lock() the shared memory segment failed because create() or attach() failed and returned false, or because a system error occurred in QSystemSemaphore::acquire()."));break;
        case 7:	 ui->leSaved->appendPlainText(tr("A create() operation failed because there was not enough memory available to fill the request."));break;
        case 8:	 ui->leSaved->appendPlainText(tr("Something else happened and it was bad."));break;
        }
    }

    // Copy data to shared memory
    sharedMemory->lock();
    char *to = (char*)sharedMemory->data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(sharedMemory->size(), size));
    sharedMemory->unlock();
    ui->leSaved->appendPlainText("Saving: "+ui->leInput->toPlainText());

}
