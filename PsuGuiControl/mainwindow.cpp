#include <QSerialPortInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    psuThread.moveToThread(&tPsuThread);
    connect(    &tPsuThread,
                &QThread::finished,
                &psuThread,
                &QObject::deleteLater);
    tPsuThread.start();

    makeConnections();
    getSerialPorts();
}

MainWindow::~MainWindow()
{
    tPsuThread.quit();
    tPsuThread.wait();
    delete ui;
}

void MainWindow::openClicked(bool checked)
{
    Q_UNUSED(checked);

    QString port;

    port = ui->serialPort->currentText();

    emit psuOpenPort(port);
}

void MainWindow::closeClicked(bool checked)
{
    Q_UNUSED(checked);

    emit psuClosePort();
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

void MainWindow::resultOpenPort(QString errorString)
{
    showErrorText(errorString);
}

void MainWindow::resultClosePort(QString errorString)
{
    Q_UNUSED(errorString);
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

void MainWindow::showErrorText(QString text)
{
    if (text != "")
    {
        ui->statusbar->showMessage(text);
    }
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
    connect(ui->openButton, SIGNAL(clicked(bool)), this, SLOT(openClicked(bool)));
    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(closeClicked(bool)));
    connect(ui->quitButton, SIGNAL(clicked(bool)), this, SLOT(quitClicked(bool)));

    connect(ui->enableOutput, SIGNAL(clicked(bool)), this, SLOT(enableOutputClicked(bool)));

    connect(ui->serialPort, SIGNAL(activated(int)), this, SLOT(serialPortActivated(int)));

    connect(ui->constantCurrent, SIGNAL(toggled(bool)), this, SLOT(constantCurrentToggled(bool)));
    connect(ui->ampsTensUp, SIGNAL(clicked(bool)), this, SLOT(ampsTensUpClicked(bool)));
    connect(ui->ampsTensDown, SIGNAL(clicked(bool)), this, SLOT(ampsTensDownClicked(bool)));
    connect(ui->ampsOnesUp, SIGNAL(clicked(bool)), this, SLOT(ampsOnesUpClicked(bool)));
    connect(ui->ampsOnesDown, SIGNAL(clicked(bool)), this, SLOT(ampsOnesDownClicked(bool)));
    connect(ui->ampsTenthsUp, SIGNAL(clicked(bool)), this, SLOT(ampsTenthsUpClicked(bool)));
    connect(ui->ampsTenthsDown, SIGNAL(clicked(bool)), this, SLOT(ampsTenthsDownClicked(bool)));
    connect(ui->ampsHundrethsUp, SIGNAL(clicked(bool)), this, SLOT(ampsHundrethsUpClicked(bool)));
    connect(ui->ampsHundrethsDown, SIGNAL(clicked(bool)), this, SLOT(ampsHundrethsDownClicked(bool)));
    connect(ui->ampsThousandthsUp, SIGNAL(clicked(bool)), this, SLOT(ampsThousandthsUpClicked(bool)));
    connect(ui->ampsThousandthsDown, SIGNAL(clicked(bool)), this, SLOT(ampsThousandthsDownClicked(bool)));

    connect(ui->constantVoltage, SIGNAL(toggled(bool)), this, SLOT(constantVoltageToggled(bool)));
    connect(ui->voltsTensUp, SIGNAL(clicked(bool)), this, SLOT(voltsTensUpClicked(bool)));
    connect(ui->voltsTensDown, SIGNAL(clicked(bool)), this, SLOT(voltsTensDownClicked(bool)));
    connect(ui->voltsOnesUp, SIGNAL(clicked(bool)), this, SLOT(voltsOnesUpClicked(bool)));
    connect(ui->voltsOnesDown, SIGNAL(clicked(bool)), this, SLOT(voltsOnesDownClicked(bool)));
    connect(ui->voltsTenthsUp, SIGNAL(clicked(bool)), this, SLOT(voltsTenthsUpClicked(bool)));
    connect(ui->voltsTenthsDown, SIGNAL(clicked(bool)), this, SLOT(voltsTenthsDownClicked(bool)));
    connect(ui->voltsHundrethsUp, SIGNAL(clicked(bool)), this, SLOT(voltsHundrethsUpClicked(bool)));
    connect(ui->voltsHundrethsDown, SIGNAL(clicked(bool)), this, SLOT(voltsHundrethsDownClicked(bool)));
    connect(ui->voltsThousandthsUp, SIGNAL(clicked(bool)), this, SLOT(voltsThousandthsUpClicked(bool)));
    connect(ui->voltsThousandthsDown, SIGNAL(clicked(bool)), this, SLOT(voltsThousandthsDownClicked(bool)));

//    QObject::connect(this, &MainWindow::psuOpenPort, &psuThread, &PsuThread::psuOpenPort);
    connect(this, SIGNAL(psuOpenPort(QString)), &psuThread, SLOT(psuOpenPort(QString)));
    connect(this, SIGNAL(psuClosePort()), &psuThread, SLOT(psuClosePort()));
    connect(this, SIGNAL(psuGetOutputCurrent()), &psuThread, SLOT(psuGetOutputCurrent()));
    connect(this, SIGNAL(psuSetOutputVoltage(double_t)), &psuThread, SLOT(psuSetOutputVoltage(double_t)));
    connect(this, SIGNAL(psuGetOutputVoltage()), &psuThread, SLOT(psuGetOutputVoltage()));
    connect(this, SIGNAL(psuGetActualOutputCurrent()), &psuThread, SLOT(psuGetActualOutputCurrent()));
    connect(this, SIGNAL(psuGetActualOutputVoltage()), &psuThread, SLOT(psuGetActualOutputVoltage()));
    connect(this, SIGNAL(psuSetOutputEnable(bool)), &psuThread, SLOT(psuSetOutputEnable(bool)));
    connect(this, SIGNAL(psuGetStatus()), &psuThread, SLOT(psuGetStatus()));
    connect(this, SIGNAL(psuGetIdentification()), &psuThread, SLOT(psuGetIdentification()));
    connect(this, SIGNAL(psuRecallPanelSetting(int)), &psuThread, SLOT(psuRecallPanelSetting(int)));
    connect(this, SIGNAL(psuSavePanelSetting(int)), &psuThread, SLOT(psuSavePanelSetting(int)));
    connect(this, SIGNAL(psuSetOverCurrentPrtotection(bool)), &psuThread, SLOT(psuSetOverCurrentPrtotection(bool)));
    connect(this, SIGNAL(psuSetKeyboardLock(bool)), &psuThread, SLOT(psuSetKeyboardLock(bool)));

    connect(&psuThread, SIGNAL(resultOpenPort(QString)), this, SLOT(resultOpenPort(QString)));
    connect(&psuThread, SIGNAL(resultClosePort(QString)), this, SLOT(resultClosePort(QString)));
    connect(&psuThread, SIGNAL(resultSetOutputVoltage(QString)), this, SLOT(resultSetOutputVoltage(QString)));
    connect(&psuThread, SIGNAL(resultGetOutputCurrent(double_t,QString)), this, SLOT(resultGetOutputCurrent(double_t,QString)));
    connect(&psuThread, SIGNAL(resultGetActualOutputCurrent(double_t,QString)), this, SLOT(resultGetActualOutputCurrent(double_t,QString)));
    connect(&psuThread, SIGNAL(resultGetActualOutputVoltage(double_t,QString)), this, SLOT(resultGetActualOutputVoltage(double_t,QString)));
    connect(&psuThread, SIGNAL(resultGetOutputEnable(QString)), this, SLOT(resultGetOutputEnable(QString)));
    connect(&psuThread, SIGNAL(resultGetStatus(QString,QString)), this, SLOT(resultGetStatus(QString,QString)));
    connect(&psuThread, SIGNAL(resultGetIdentification(QString,QString)), this, SLOT(resultGetIdentification(QString,QString)));
    connect(&psuThread, SIGNAL(resultRecallPanelSetting(int,QString)), this, SLOT(resultRecallPanelSetting(int,QString)));
    connect(&psuThread, SIGNAL(resultSavePanelSetting(QString)), this, SLOT(resultSavePanelSetting(QString)));
    connect(&psuThread, SIGNAL(resultSetOverCurrentPrtotection(QString)), this, SLOT(resultSetOverCurrentPrtotection(QString)));
    connect(&psuThread, SIGNAL(resultSetKeyboardLock(QString)), this, SLOT(resultSetKeyboardLock(QString)));
}
