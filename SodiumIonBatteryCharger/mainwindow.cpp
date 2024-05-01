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

    quitting = false;
    setCurrentState(eNO_STATE);

    makeConnections();
    getSerialPorts();
    avpInitialise();
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

void MainWindow::setCurrentState(eChargingState newState)
{
    currentState = newState;

    switch(currentState)
    {
    case eNO_STATE:
        ui->openSerialPort->setEnabled(false);
        ui->openIniFile->setEnabled(true);
        ui->start->setEnabled(false);
        ui->pause->setEnabled(false);
        ui->stop->setEnabled(false);
        ui->quit->setEnabled(true);
        break;
    case eINIT_PORT_OPENED:
        ui->openSerialPort->setEnabled(true);
        ui->openIniFile->setEnabled(true);
        ui->start->setEnabled(false);
        ui->pause->setEnabled(false);
        ui->stop->setEnabled(false);
        ui->quit->setEnabled(true);
        break;
    case eSERIAL_PORT_OPENED:
        ui->openSerialPort->setEnabled(true);
        ui->openIniFile->setEnabled(true);
        ui->start->setEnabled(false);
        ui->pause->setEnabled(false);
        ui->stop->setEnabled(false);
        ui->quit->setEnabled(true);
        break;
    case eCHARGE_NOT_STARTED:
        ui->openSerialPort->setEnabled(false);
        ui->openIniFile->setEnabled(false);
        ui->start->setEnabled(true);
        ui->pause->setEnabled(false);
        ui->stop->setEnabled(false);
        ui->quit->setEnabled(true);
        break;
    case eCHARGE_STARTED:
        ui->openSerialPort->setEnabled(false);
        ui->openIniFile->setEnabled(false);
        ui->start->setEnabled(false);
        ui->pause->setEnabled(true);
        ui->stop->setEnabled(true);
        ui->quit->setEnabled(false);
        break;
    case eCHARGE_PAUSED:
        ui->openSerialPort->setEnabled(false);
        ui->openIniFile->setEnabled(true);
        ui->start->setEnabled(false);
        ui->pause->setEnabled(true);
        ui->stop->setEnabled(false);
        ui->quit->setEnabled(true);
        break;
    case eCHARGE_STOPPING:
        ui->openSerialPort->setEnabled(false);
        ui->openIniFile->setEnabled(false);
        ui->start->setEnabled(false);
        ui->pause->setEnabled(false);
        ui->stop->setEnabled(false);
        ui->quit->setEnabled(true);
        break;
    default:
        QString error("Unknown state");
        displayErrorDialog(error);
        break;
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

void MainWindow::displayPsuId(QString &pId)
{
    QString preAmble("Power Supply Id: ");

    preAmble.append(pId);
    ui->psuIdentifier->setText(preAmble);
}

void MainWindow::displayActualCurrent(qreal current)
{
    int tens;
    int ones;
    int tenths;
    int hundreths;
    int thousandths;

    floatToDigits(current, tens, ones, tenths, hundreths, thousandths);

    ui->actualAmpsTens->display(tens);
    ui->actualAmpsOnes->display(ones);
    ui->actualAmpsTenths->display(tenths);
    ui->actualAmpsHundreths->display(hundreths);
    ui->actualAmpsThousandths->display(thousandths);
}

void MainWindow::displayActualVoltage(qreal voltage)
{
    int tens;
    int ones;
    int tenths;
    int hundreths;
    int thousandths;

    floatToDigits(voltage, tens, ones, tenths, hundreths, thousandths);

    ui->actualVoltsTens->display(tens);
    ui->actualVoltsOnes->display(ones);
    ui->actualVoltsTenths->display(tenths);
    ui->actualVoltsHundreths->display(hundreths);
    ui->actualVoltsThousandths->display(thousandths);
}

void MainWindow::stopCharging(QString &reason)
{
    emit psuSetOutputEnable(false);

    // The order of these calls is important as signal/slot event loop continues
    // even with the dialog displayed
    timer.stop();
    setCurrentState(eCHARGE_STOPPING);
    displayErrorDialog(reason);
}

void MainWindow::floatToDigits(qreal value, int &tens, int &ones, int &tenths, int &hundreths, int &thousandths)
{
    QString     fValue;
    QByteArray  ba;
    qsizetype   st;

    QTextStream ts(&fValue);

    ts.setFieldWidth(6);
    ts.setPadChar('0');
    ts.setRealNumberPrecision(3);
    ts.setRealNumberNotation(QTextStream::FixedNotation);
    ts.setFieldAlignment(QTextStream::AlignCenter);

    ts << value;

    ba = fValue.toLocal8Bit();

    st = ba.indexOf(".");

    if (st == 1)
    {
        tens = 0;
        ones = ba[0] - 0x30;
        tenths = ba[2] - 0x30;
        hundreths = ba[3] - 0x30;
        thousandths = ba[4] - 0x30;
    }
    else
    {
        tens = ba[0] - 0x30;
        ones = ba[1] - 0x30;
        tenths = ba[3] - 0x30;
        hundreths = ba[4] - 0x30;
        thousandths = ba[5] - 0x30;
    }
}

void MainWindow::avpInitialise()
{
    // Cut down on the typing and make code more readable
    QCustomPlot *plot = ui->ampsVoltsPlot;

    // Graph zero (0) in blue will be amps display
    QPen        ampsColour(Qt::blue);

    // Graph one (1) in red will be the volts display
    QPen        voltsColour(Qt::red);

    // Use default key/value axis layout
    plot->addGraph();
    plot->graph(0)->setPen(ampsColour);
    plot->graph(0)->setName("Amps");

    // Key axis bottom, value axis right
    plot->addGraph(plot->xAxis, plot->yAxis2);
    plot->graph(1)->setPen(voltsColour);
    plot->graph(1)->setName("Volts");

    // Assume left axis shows current values
    // Assume right axis shows voltage values
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(true);

    // Set user interactions
    // Allow user to drag axis ranges with mouse and zoom with mouse wheel
    // plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::avpConfigure()
{
    // Cut down on the typing and make code more readable
    QCustomPlot *plot = ui->ampsVoltsPlot;

    // Graph zero (0) will be amps display
    qreal       minAmpsRange = 0.0;
    qreal       maxAmpsRange = confData.getMaxChargeCurrent();
    maxAmpsRange /=1000.0;
    QCPRange    ampsRange(minAmpsRange, maxAmpsRange);

    // Graph one (1) will be volts display
    qreal       minVoltsRange = 0.0;
    qreal       maxVoltsRange = confData.getMaxAppliedVolts();
    maxVoltsRange /=1000.0;
    QCPRange    voltsRange(minVoltsRange, maxVoltsRange);

    plot->yAxis->setRange(ampsRange);
    plot->yAxis2->setRange(voltsRange);

    plot->replot();
}

void MainWindow::avpAddData(qreal &time, qreal &current, qreal &voltage)
{
    // Cut down on the typing and make code more readable
    QCustomPlot *plot = ui->ampsVoltsPlot;

    // Graph zero (0) is the amps display
    plot->graph(0)->addData(time, current);
    plot->graph(0)->rescaleAxes();

    // Graph one (1) is the volts display
    plot->graph(1)->addData(time, voltage);
    plot->graph(1)->rescaleAxes();

    plot->replot();
}

void MainWindow::avpClearData()
{
    ui->ampsVoltsPlot->graph(0)->data()->clear();
    ui->ampsVoltsPlot->graph(1)->data()->clear();

    ui->ampsVoltsPlot->replot();
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
        QString configId = confData.getConfigurationId();
        displayConfigurationId(configId);

        chargeCompleteCurrent = confData.getCompletedCurrent();
        chargeCompleteCurrent /= 1000.0;
        minAppliedVoltage = confData.getMinAppliedVolts();
        minAppliedVoltage /= 1000.0;

        setCurrentState(eINIT_PORT_OPENED);
        avpConfigure();
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

    startTime = QDateTime::currentDateTime();
    endTime = QDateTime::currentDateTime();
    endTime = endTime.addSecs(maxChargePeriod);

    qreal   current = confData.getMaxConstantCurrent();
    current /= 1000.0;
    setCurrentState(eCHARGE_STARTED);

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
        setCurrentState(eSERIAL_PORT_OPENED);
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
        displayErrorDialog(error);
    }
    else
    {
        if (current <= (qreal) chargeCompleteCurrent)
        {
            QString reason("Charging complete");
            stopCharging(reason);
        }
        else
        {
            lastCurrent = current;

            // Update actual current LCD display
            displayActualCurrent(current);

            // Add data to the amps and volts plot
            qreal   elapsedTime;
            elapsedTime = startTime.secsTo(QDateTime::currentDateTime());
            avpAddData(elapsedTime, lastCurrent, lastVoltage);
        }
    }
}

void MainWindow::resultGetActualOutputVoltage(qreal voltage, QString error)
{
    if (error != "")
    {
        stopCharging(error);
        displayErrorDialog(error);
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
            lastVoltage = voltage;
            // Update actual voltage LCD display
            displayActualVoltage(voltage);
            // Chart update AFTER actual current is obtained
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
        displayPsuId(identification);
        setCurrentState(eCHARGE_NOT_STARTED);
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
        // Required test as SetOutputEnable is part of the start and end
        // sequence
        if (currentState != eCHARGE_STOPPING)
        {
            setCurrentState(eCHARGE_STARTED);
            qreal updatePeriod = confData.getUpdatePeriod();
            timer.start(updatePeriod);
        }
        else
        {
            setCurrentState(eCHARGE_NOT_STARTED);
        }
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
    connect(ui->openSerialPort, SIGNAL(clicked(bool)), this, SLOT(openSerialPortClicked(bool)));
    connect(ui->start, SIGNAL(clicked(bool)), this, SLOT(startClicked(bool)));
    connect(ui->pause, SIGNAL(clicked(bool)), this, SLOT(pauseClicked(bool)));
    connect(ui->stop, SIGNAL(clicked(bool)), this, SLOT(stopClicked(bool)));
    connect(ui->quit, SIGNAL(clicked(bool)), this, SLOT(quitClicked(bool)));

    connect(this, SIGNAL(psuOpenPort(QString)), &psuThread, SLOT(psuOpenPort(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuClosePort()), &psuThread, SLOT(psuClosePort()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetIdentification()), &psuThread, SLOT(psuGetIdentification()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetOutputEnable(bool)), &psuThread, SLOT(psuSetOutputEnable(bool)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetOutputCurrent(qreal)), &psuThread, SLOT(psuSetOutputCurrent(qreal)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetOutputVoltage(qreal)), &psuThread, SLOT(psuSetOutputVoltage(qreal)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetActualOutputCurrent()), &psuThread, SLOT(psuGetActualOutputCurrent()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetActualOutputVoltage()), &psuThread, SLOT(psuGetActualOutputVoltage()), Qt::QueuedConnection);

    connect(&psuThread, SIGNAL(resultOpenPort(QString)), this, SLOT(resultOpenPort(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultClosePort(QString)), this, SLOT(resultClosePort(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetIdentification(QString,QString)), this, SLOT(resultGetIdentification(QString,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetOutputEnable(QString)), this, SLOT(resultSetOutputEnable(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetOutputCurrent(QString)), this, SLOT(resultSetOutputCurrent(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetOutputVoltage(QString)), this, SLOT(resultSetOutputVoltage(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetActualOutputCurrent(qreal,QString)), this, SLOT(resultGetActualOutputCurrent(qreal,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetActualOutputVoltage(qreal,QString)), this, SLOT(resultGetActualOutputVoltage(qreal,QString)), Qt::QueuedConnection);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timerExpired()), Qt::QueuedConnection);
}
