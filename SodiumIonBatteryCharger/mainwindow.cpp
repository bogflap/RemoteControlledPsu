#include <QSerialPortInfo>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , confData(parent)
{
    ui->setupUi(this);

    psuThread.moveToThread(&tPsuThread);
    tPsuThread.start();

    timer.setSingleShot(false);

    makeConnections();
    getSerialPorts();
}

void MainWindow::getSerialPorts()
{
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        ui->serialPort->addItem(port.systemLocation());
    }

    if (ui->serialPort->count())
    {
        ui->openSerialPort->setEnabled(true);
    }
}

void MainWindow::displayErrorDialog(QString &error)
{
    QString title("Error");

    QMessageBox::information(this, title, error);
}

void MainWindow::displayConfigurationId(QString &cId)
{
    QString preAmble("Configuration Id: ");

    preAmble.append(cId);
    ui->iniIdentifier->setText(preAmble);
}

MainWindow::~MainWindow()
{
    tPsuThread.quit();
    tPsuThread.wait();
    delete ui;
}

void MainWindow::selectIniFileClicked(bool checked)
{
    Q_UNUSED(checked);

    confFilePath = QFileDialog::getOpenFileName(this,
                                                "Open Initialisation File",
                                                "",
                                                "Ini File (*.ini)",
                                                nullptr,
                                                QFileDialog::ReadOnly);
    ui->iniFile->setText(confFilePath);
}

void MainWindow::openIniFileClicked(bool checked)
{
    Q_UNUSED(checked);

    if (!confData.open(confFilePath))
    {
        QString error("ERROR: Invalid Configuration File");
        displayErrorDialog(error);
    }
    else
    {
        ui->openSerialPort->setEnabled(true);

        QString configId = confData.getConfigurationId();
        displayConfigurationId(configId);
    }
}

void MainWindow::openSerialPortClicked(bool checked)
{
    Q_UNUSED(checked);
    QString port;

    port = ui->serialPort->currentText();

    emit psuOpenPort(port);
}

void MainWindow::startClicked(bool checked)
{
    Q_UNUSED(checked);

    ui->start->setEnabled(false);

    qreal   current = confData.getMaxChargeCurrent();
    current /= 1000.0;
    emit psuSetOutputCurrent(current);
}

void MainWindow::pauseClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::stopClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::quitClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::timerExpired()
{

}

void MainWindow::resultOpenPort(QString error)
{
    if(error != "")
    {
        displayErrorDialog(error);
    }
    else
    {
        emit psuGetIdentification();
    }
}

void MainWindow::resultClosePort(QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultSetOutputCurrent(QString error)
{
    if (error != "")
    {
        displayErrorDialog(error);
        ui->start->setEnabled(true);
    }
    else
    {
        qreal   voltage = confData.getMaxAppliedVolts();
        voltage /= 1000.0;
        emit psuSetOutputVoltage(voltage);
    }
}

void MainWindow::resultGetOutputCurrent(qreal current,QString error)
{
    Q_UNUSED(error);
    Q_UNUSED(current);
}

void MainWindow::resultSetOutputVoltage(QString error)
{
    if (error != "")
    {
        displayErrorDialog(error);
        ui->start->setEnabled(true);
    }
    else
    {
        emit psuSetOutputEnable(true);
    }
}

void MainWindow::resultGetOutputVoltage(qreal, QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultGetActualOutputCurrent(qreal, QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultGetActualOutputVoltage(qreal, QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultGetOutputEnable(QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultGetStatus(QString status, QString error)
{
    Q_UNUSED(error);
    Q_UNUSED(status);
}

void MainWindow::resultGetIdentification(QString identification, QString error)
{
    if (error != "")
    {
        displayErrorDialog(error);
    }
    else
    {
        ui->psuIdentifier->setText(identification);
    }
}

void MainWindow::resultRecallPanelSetting(int, QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultSavePanelSetting(QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultSetOutputEnable(QString error)
{
    if (error != "")
    {
        displayErrorDialog(error);
        ui->start->setEnabled(true);
    }
    else
    {
        ui->pause->setEnabled(true);
        ui->stop->setEnabled(true);

        qreal updatePeriod = confData.getUpdatePeriod();
        timer.start(updatePeriod);
    }
}

void MainWindow::resultSetOverCurrentProtection(QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultSetKeyboardLock(QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultIsConstantCurrent(bool, QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultIsConstantVoltage(bool, QString error)
{
    Q_UNUSED(error);
}

void MainWindow::resultIsOutputEnabled(bool, QString error)
{
    Q_UNUSED(error);
}

void MainWindow::makeConnections()
{
    connect(ui->selectIniFile, SIGNAL(clicked(bool)), this, SLOT(selectIniFileClicked(bool)));
    connect(ui->openIniFile, SIGNAL(clicked(bool)), this, SLOT(openIniFileClicked(bool)));

    connect(this, SIGNAL(psuOpenPort(QString)), &psuThread, SLOT(psuOpenPort(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuClosePort()), &psuThread, SLOT(psuClosePort()), Qt::QueuedConnection);

    connect(&psuThread, SIGNAL(resultOpenPort(QString)), this, SLOT(resultOpenPort(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultClosePort(QString)), this, SLOT(resultClosePort(QString)), Qt::QueuedConnection);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timerExpired()), Qt::QueuedConnection);
}
