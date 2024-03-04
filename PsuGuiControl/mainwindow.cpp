#include <QSerialPortInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "psuThread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    PsuThread*  psut;

    ui->setupUi(this);

    psut = new PsuThread();
    psut->moveToThread(&psuThread);
    connect(&psuThread, &QThread::finished, psut, &QObject::deleteLater);

    makeConnections();
    getSerialPorts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::closeClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::getSerialPorts()
{
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        ui->serialPortComboBox->addItem(port.systemLocation());
    }

    if (ui->serialPortComboBox->count())
    {
        ui->openButton->setEnabled(true);
    }
}

void MainWindow::makeConnections()
{

}
