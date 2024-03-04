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

void MainWindow::resultSetOutputCurrent(QString errorString)
{
    Q_UNUSED(errorString);
}

void MainWindow::resultGetOutputCurrent(double_t current, QString errorString)
{
    Q_UNUSED(errorString);
    Q_UNUSED(current);
}

void MainWindow::resultSetOutputVoltage(QString errorString)
{
    Q_UNUSED(errorString);
}

void MainWindow::resultGetOutputVoltage(double_t voltage, QString errorString)
{
    Q_UNUSED(errorString);
    Q_UNUSED(voltage);
}

void MainWindow::resultGetActualOutputCurrent(double_t current, QString errorString)
{
    Q_UNUSED(errorString);
    Q_UNUSED(current);
}

void MainWindow::resultGetActualOutputVoltage(double_t voltage, QString errorString)
{
    Q_UNUSED(errorString);
    Q_UNUSED(voltage);
}

void MainWindow::resultGetOutputEnable(QString errorString)
{
    Q_UNUSED(errorString);
}

void MainWindow::resultGetStatus(QString status, QString errorString)
{
    Q_UNUSED(errorString);
    Q_UNUSED(status);
}

void MainWindow::resultGetIdentification(QString identification, QString errorString)
{
    Q_UNUSED(errorString);
    Q_UNUSED(identification);
}

void MainWindow::resultRecallPanelSetting(int number, QString errorString)
{
    Q_UNUSED(errorString);
    Q_UNUSED(number);
}

void MainWindow::resultSavePanelSetting(QString errorString)
{
    Q_UNUSED(errorString);
}

void MainWindow::resultSetOverCurrentPrtotection(QString errorString)
{
    Q_UNUSED(errorString);
}

void MainWindow::resultSetKeyboardLock(QString errorString)
{
    Q_UNUSED(errorString);
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
