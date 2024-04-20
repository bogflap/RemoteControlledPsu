#include <QSerialPortInfo>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , confData(parent)
    , quitting(false)
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

void MainWindow::stopCharging(QString &reason)
{
    emit psuSetOutputEnable(false);

    displayErrorDialog(reason);

    ui->start->setEnabled(false);
    ui->pause->setEnabled(false);
    ui->stop->setEnabled(false);

    timer.stop();

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

        chargeCompleteCurrent = confData.getCompletedCurrent();
        minAppliedVoltage = confData.getMinAppliedVolts();
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

    int maxChargePeriod = confData.getMaxChargePeriod() / 1000;

    ui->start->setEnabled(false);

    endTime = QDateTime::currentDateTime();
    endTime = endTime.addSecs(maxChargePeriod);

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

    timer.stop();

    quitting = true;

    emit psuClosePort();
}

void MainWindow::timerExpired()
{
    int elapsedSeconds = QDateTime::currentDateTime().secsTo(endTime);

    if (elapsedSeconds < 0)
    {
        QString error("Maximum charge time exceeded");
        stopCharging(error);
    }
    else
    {
        emit psuGetActualOutputVoltage();
    }
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

    if (quitting)
    {
        exit(0);
    }
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

void MainWindow::resultGetActualOutputCurrent(qreal current, QString error)
{
    if (error != "")
    {
        stopCharging(error);
    }
    else
    {
        if (current <= (qreal) chargeCompleteCurrent)
        {
            QString reason("Charging complete");
            stopCharging(reason);
        }
        // else do nothing
    }
}

void MainWindow::resultGetActualOutputVoltage(qreal voltage, QString error)
{
    if (error != "")
    {
        stopCharging(error);
    }
    else
    {
        if (voltage < (qreal) minAppliedVoltage)
        {
            QString reason("Minimum voltage detected");
            stopCharging(reason);
        }
        else
        {
            emit psuGetActualOutputCurrent();
        }
    }
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
    connect(this, SIGNAL(psuSetOutputEnable(bool)), &psuThread, SLOT(psuSetOutputEnable(bool)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetOutputCurrent(qreal)), &psuThread, SLOT(psuSetOutputCurrent(qreal)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetOutputVoltage(qreal)), &psuThread, SLOT(psuSetOutputVoltage(qreal)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetActualOutputCurrent()), &psuThread, SLOT(psuGetActualOutputCurrent()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetActualOutputVoltage()), &psuThread, SLOT(psuGetActualOutputVoltage()), Qt::QueuedConnection);

    connect(&psuThread, SIGNAL(resultOpenPort(QString)), this, SLOT(resultOpenPort(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultClosePort(QString)), this, SLOT(resultClosePort(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetOutputEnable(QString)), this, SLOT(resultSetOutputEnable(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetOutputCurrent(QString)), this, SLOT(resultSetOutputCurrent(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetOutputVoltage(QString)), this, SLOT(resultSetOutputVoltage(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetActualOutputCurrent(qreal,QString)), this, SLOT(resultGetActualOutputCurrent(qreal,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetActualOutputVoltage(qreal,QString)), this, SLOT(resultGetActualOutputVoltage(qreal,QString)), Qt::QueuedConnection);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timerExpired()), Qt::QueuedConnection);
}
