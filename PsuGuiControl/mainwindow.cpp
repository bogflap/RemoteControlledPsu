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

void MainWindow::quitClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::enableOutputClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::serialPortActivated(int index)
{
    Q_UNUSED(index);
}

void MainWindow::constantCurrentToggled(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsTensUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsTensDownClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsOnesUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsOnesDownClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsTenthsUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsTenthsDownClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsHundrethsUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsHundrethsDownClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsThousandthsUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsThousandthsDownClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsApplyClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::ampsResetClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::constantVoltageToggled(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsApplyClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsResetClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsTensUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsTensDownClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsOnesUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsOnesDownClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsTenthsUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsTenthsDownClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsHundrethsUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsHundrethsDownClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsThousandthsUpClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsThousandthsDownClicked(bool checked)
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
        ui->serialPort->addItem(port.systemLocation());
    }

    if (ui->serialPort->count())
    {
        ui->openButton->setEnabled(true);
    }
}

void MainWindow::makeConnections()
{
    connect(ui->openButton, SIGNAL(QPushButton::clicked(bool)), this, SLOT(openClicked(bool)));
    connect(ui->closeButton, SIGNAL(QPushButton::clicked(bool)), this, SLOT(closeClicked(bool)));
    connect(ui->quitButton, SIGNAL(QPushButton::clicked(bool)), this, SLOT(quitClicked(bool)));

    connect(ui->enableOutput, SIGNAL(QPushButton::clicked(bool)), this, SLOT(enableOutputClicked(bool)));

    connect(ui->serialPort, SIGNAL(activated(int)), this, SLOT(serialPortActivated(int)));

    connect(ui->constantCurrent, SIGNAL(toggled(bool)), this, SLOT(constantCurrentToggled(bool)));
    connect(ui->ampsTensUp, SIGNAL(clicked(bool)), this, SLOT(ampsTensUpClicked(bool)));
    connect(ui->ampsTensDown, SIGNAL(clicked(bool)), this, SLOT(ampsTensDownClicked(bool)));
}
