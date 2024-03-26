#include <QSerialPortInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ampsVoltsTimer(parent)
    , outputEnabledStyle(QString::fromUtf8("background-color: rgb(255, 0, 0);"))
    , outputDisabledStyle(QString::fromUtf8("background-color: rgb(0, 255, 0);"))
{
    ui->setupUi(this);

    psuThread.moveToThread(&tPsuThread);
    tPsuThread.start();

    quitting = false;

    setupLcds();
    makeConnections();
    getSerialPorts();
    setEnableOutputColour(true);
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

    ampsVoltsTimer.stop();
    setEnableOutputColour(true);

    emit psuClosePort();
}

void MainWindow::quitClicked(bool checked)
{
    Q_UNUSED(checked);

    quitting = true;

    emit psuClosePort();}

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

    ampsLcd.upTens();
}

void MainWindow::ampsTensDownClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.downTens();
}

void MainWindow::ampsOnesUpClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.upOnes();
}

void MainWindow::ampsOnesDownClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.downOnes();
}

void MainWindow::ampsTenthsUpClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.upTenths();
}

void MainWindow::ampsTenthsDownClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.downTenths();
}

void MainWindow::ampsHundrethsUpClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.upHundreths();
}

void MainWindow::ampsHundrethsDownClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.downHundreths();
}

void MainWindow::ampsThousandthsUpClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.upThousandths();
}

void MainWindow::ampsThousandthsDownClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.downThousandths();
}

void MainWindow::ampsApplyClicked(bool checked)
{
    Q_UNUSED(checked);

    qreal   currentMa;

    int     tens        = ui->ampsTens->intValue();
    int     ones        = ui->ampsOnes->intValue();
    int     tenths      = ui->ampsTenths->intValue();
    int     hundreths   = ui->ampsHundreths->intValue();
    int     thousandths = ui->ampsThousandths->intValue();

    currentMa = digitsToFloat.digitsToFloat(tens,
                                            ones,
                                            tenths,
                                            hundreths,
                                            thousandths);

    currentMa /= 1000.0;

    emit psuSetOutputCurrent(currentMa);
}

void MainWindow::ampsResetClicked(bool checked)
{
    Q_UNUSED(checked);

    ampsLcd.setValues(0, 0, 0, 0, 0);
}

void MainWindow::constantVoltageToggled(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::voltsApplyClicked(bool checked)
{
    Q_UNUSED(checked);

    qreal   voltsMv;

    int     tens        = ui->voltsTens->intValue();
    int     ones        = ui->voltsOnes->intValue();
    int     tenths      = ui->voltsTenths->intValue();
    int     hundreths   = ui->voltsHundreths->intValue();
    int     thousandths = ui->voltsThousandths->intValue();

    voltsMv = digitsToFloat.digitsToFloat(tens,
                                            ones,
                                            tenths,
                                            hundreths,
                                            thousandths);

    voltsMv /= 1000.0;

    emit psuSetOutputVoltage(voltsMv);
}

void MainWindow::voltsResetClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.setValues(0, 0, 0, 0, 0);
}

void MainWindow::voltsTensUpClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.upTens();
}

void MainWindow::voltsTensDownClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.downTens();
}

void MainWindow::voltsOnesUpClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.upOnes();
}

void MainWindow::voltsOnesDownClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.downOnes();
}

void MainWindow::voltsTenthsUpClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.upTenths();
}

void MainWindow::voltsTenthsDownClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.downTenths();
}

void MainWindow::voltsHundrethsUpClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.upHundreths();
}

void MainWindow::voltsHundrethsDownClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.downHundreths();
}

void MainWindow::voltsThousandthsUpClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.upThousandths();
}

void MainWindow::voltsThousandthsDownClicked(bool checked)
{
    Q_UNUSED(checked);

    voltsLcd.downThousandths();
}

void MainWindow::ampsVoltsTimeout()
{
    emit psuGetActualOutputCurrent();
    emit psuGetActualOutputVoltage();
}

void MainWindow::resultOpenPort(QString errorString)
{
    if (!errorString.isEmpty())
    {
        showStatusText(errorString);
    }
    else
    {
        QString text;
        QTextStream ts(&text);

        ts << "Opened port " << ui->serialPort->currentText();
        showStatusText(text);

        ui->openButton->setEnabled(false);
        ui->closeButton->setEnabled(true);
        ui->serialPort->setEnabled(false);

        // Opened device so get some information about it
        emit psuGetIdentification();
        emit psuGetStatus();
        emit psuGetOutputCurrent();
        emit psuGetOutputVoltage();
    }
}

void MainWindow::resultClosePort(QString errorString)
{
    showStatusText(errorString);

    QString text;
    QTextStream ts(&text);

    ts << "Closed port " << ui->serialPort->currentText();
    showStatusText(text);

    ui->openButton->setEnabled(true);
    ui->closeButton->setEnabled(false);
    ui->serialPort->setEnabled(true);

    if (quitting)
    {
        exit(0);
    }
}

void MainWindow::resultSetOutputCurrent(QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
}

void MainWindow::resultGetOutputCurrent(qreal current, QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
    else
    {
        setOutputAmps(current);
    }
}

void MainWindow::resultSetOutputVoltage(QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
}

void MainWindow::resultGetOutputVoltage(qreal voltage, QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
    else
    {
        setOutputVolts(voltage);
    }
}

void MainWindow::resultGetActualOutputCurrent(qreal current, QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
    else
    {
        setActualAmps(current);
    }
}

void MainWindow::resultGetActualOutputVoltage(qreal voltage, QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
    else
    {
        setActualVolts(voltage);
    }
}

void MainWindow::resultGetOutputEnable(QString errorString)
{
    Q_UNUSED(errorString);
}

void MainWindow::resultGetStatus(QString status, QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
    else
    {
        // Display status results and then get constant current/voltage and
        // output enabled settings.
        // After that it is up to the user to do something
        QStringList stats = status.split("\n");
        ui->mode->setText(stats[0]);
        ui->outputEnabled->setText(stats[1]);

        emit psuIsConstantCurrent();
        emit psuIsOutputEnabled();
    }
}

void MainWindow::resultGetIdentification(QString identification, QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
    else
    {
        QString idText;
        QTextStream ts(&idText);

        ts  <<  "Identifier = "
            <<   identification;
        ui->identification->setText(idText);
    }
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

void MainWindow::resultSetOverCurrentProtection(QString errorString)
{
    Q_UNUSED(errorString);
}

void MainWindow::resultSetKeyboardLock(QString errorString)
{
    Q_UNUSED(errorString);
}

void MainWindow::resultIsConstantCurrent(bool result, QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
    else
    {
        ui->constantCurrent->setEnabled(true);
        ui->constantVoltage->setEnabled(true);
        setConstantCurrent(result);
    }
}

void MainWindow::resultIsConstantVoltage(bool result, QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
    else
    {
        ui->constantCurrent->setEnabled(true);
        ui->constantVoltage->setEnabled(true);
        setConstantCurrent(!result);
    }
}

void MainWindow::resultIsOutputEnabled(bool result, QString errorString)
{
    if (errorString != "")
    {
        showStatusText(errorString);
    }
    else
    {
        setEnableOutputColour(!result);
        ui->enableOutput->setEnabled(result);
        ampsVoltsTimer.start(1000);
    }
}

void MainWindow::showStatusText(QString text)
{
    if (text != "")
    {
        ui->statusbar->showMessage(text);
    }
}

void MainWindow::setConstantCurrent(bool constantCurrent)
{
    // It is either constant current or constant voltage
    ui->constantCurrent->setChecked(constantCurrent);
    ui->constantVoltage->setChecked(!constantCurrent);
    ui->ampsApply->setEnabled(constantCurrent);
    ui->voltsApply->setEnabled(!constantCurrent);

    ui->ampsTensUp->setEnabled(constantCurrent);
    ui->ampsTensDown->setEnabled(constantCurrent);
    ui->ampsOnesUp->setEnabled(constantCurrent);
    ui->ampsOnesDown->setEnabled(constantCurrent);
    ui->ampsTenthsUp->setEnabled(constantCurrent);
    ui->ampsTenthsDown->setEnabled(constantCurrent);
    ui->ampsHundrethsUp->setEnabled(constantCurrent);
    ui->ampsHundrethsDown->setEnabled(constantCurrent);
    ui->ampsThousandthsUp->setEnabled(constantCurrent);
    ui->ampsThousandthsDown->setEnabled(constantCurrent);
    ui->ampsReset->setEnabled(constantCurrent);

    ui->voltsTensUp->setEnabled(!constantCurrent);
    ui->voltsTensDown->setEnabled(!constantCurrent);
    ui->voltsOnesUp->setEnabled(!constantCurrent);
    ui->voltsOnesDown->setEnabled(!constantCurrent);
    ui->voltsTenthsUp->setEnabled(!constantCurrent);
    ui->voltsTenthsDown->setEnabled(!constantCurrent);
    ui->voltsHundrethsUp->setEnabled(!constantCurrent);
    ui->voltsHundrethsDown->setEnabled(!constantCurrent);
    ui->voltsThousandthsUp->setEnabled(!constantCurrent);
    ui->voltsThousandthsDown->setEnabled(!constantCurrent);
    ui->voltsReset->setEnabled(!constantCurrent);
}

void MainWindow::setupLcds()
{
    qreal   maxMa = psuThread.getMaxCurrent();
    qreal   maxMv = psuThread.getMaxVoltage();

    ampsLcd.setDigits(ui->ampsTens,
                      ui->ampsOnes,
                      ui->ampsTenths,
                      ui->ampsHundreths,
                      ui->ampsThousandths,
                      maxMa);
    ampsLcd.setValues(0, 0, 0, 0, 0);

    voltsLcd.setDigits(ui->voltsTens,
                       ui->voltsOnes,
                       ui->voltsTenths,
                       ui->voltsHundreths,
                       ui->voltsThousandths,
                       maxMv);
    voltsLcd.setValues(0, 0, 0, 0, 0);

    setActualAmps(0.0);
    setActualVolts(0.0);
}

void MainWindow::setActualAmps(qreal value)
{
    int     tens;
    int     ones;
    int     tenths;
    int     hundreths;
    int     thousandths;

    floatToDigits.floatToDigits(value, tens, ones, tenths, hundreths, thousandths);

    ui->actualAmpsTens->display(tens);
    ui->actualAmpsOnes->display(ones);
    ui->actualAmpsTenths->display(tenths);
    ui->actualAmpsHundreths->display(hundreths);
    ui->actualAmpsThousandths->display(thousandths);
}

void MainWindow::setActualVolts(qreal value)
{
    int     tens;
    int     ones;
    int     tenths;
    int     hundreths;
    int     thousandths;

    floatToDigits.floatToDigits(value, tens, ones, tenths, hundreths, thousandths);

    ui->actualVoltsTens->display(tens);
    ui->actualVoltsOnes->display(ones);
    ui->actualVoltsTenths->display(tenths);
    ui->actualVoltsHundreths->display(hundreths);
    ui->actualVoltsThousandths->display(thousandths);
}

void MainWindow::setOutputVolts(qreal value)
{
    int     tens;
    int     ones;
    int     tenths;
    int     hundreths;
    int     thousandths;

    floatToDigits.floatToDigits(value, tens, ones, tenths, hundreths, thousandths);
    voltsLcd.setValues(tens, ones, tenths, hundreths, thousandths);
}

void MainWindow::setOutputAmps(qreal value)
{
    int     tens;
    int     ones;
    int     tenths;
    int     hundreths;
    int     thousandths;

    floatToDigits.floatToDigits(value, tens, ones, tenths, hundreths, thousandths);
    ampsLcd.setValues(tens, ones, tenths, hundreths, thousandths);
}

void MainWindow::setEnableOutputColour(bool bOutputEnabled)
{
    if (bOutputEnabled)
    {
        ui->enableOutput->setStyleSheet(outputDisabledStyle);
    }
    else
    {
        ui->enableOutput->setStyleSheet(outputEnabledStyle);
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
    connect(ui->ampsApply, SIGNAL(clicked(bool)), this, SLOT(ampsApplyClicked(bool)));
    connect(ui->ampsReset, SIGNAL(clicked(bool)), this, SLOT(ampsResetClicked(bool)));

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
    connect(ui->voltsApply, SIGNAL(clicked(bool)), this, SLOT(voltsApplyClicked(bool)));
    connect(ui->voltsReset, SIGNAL(clicked(bool)), this, SLOT(voltsResetClicked(bool)));

    connect(this, SIGNAL(psuOpenPort(QString)), &psuThread, SLOT(psuOpenPort(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuClosePort()), &psuThread, SLOT(psuClosePort()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetOutputCurrent(qreal)), &psuThread, SLOT(psuSetOutputCurrent(qreal)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetOutputCurrent()), &psuThread, SLOT(psuGetOutputCurrent()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetOutputVoltage(qreal)), &psuThread, SLOT(psuSetOutputVoltage(qreal)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetOutputVoltage()), &psuThread, SLOT(psuGetOutputVoltage()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetActualOutputCurrent()), &psuThread, SLOT(psuGetActualOutputCurrent()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetActualOutputVoltage()), &psuThread, SLOT(psuGetActualOutputVoltage()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetOutputEnable(bool)), &psuThread, SLOT(psuSetOutputEnable(bool)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetStatus()), &psuThread, SLOT(psuGetStatus()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuGetIdentification()), &psuThread, SLOT(psuGetIdentification()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuRecallPanelSetting(int)), &psuThread, SLOT(psuRecallPanelSetting(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSavePanelSetting(int)), &psuThread, SLOT(psuSavePanelSetting(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetOverCurrentProtection(bool)), &psuThread, SLOT(psuSetOverCurrentProtection(bool)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuSetKeyboardLock(bool)), &psuThread, SLOT(psuSetKeyboardLock(bool)), Qt::QueuedConnection);
    connect(this, SIGNAL(psuIsConstantCurrent()), &psuThread, SLOT(psuIsConstantCurrent()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuIsConstantVoltage()), &psuThread, SLOT(psuIsConstantVoltage()), Qt::QueuedConnection);
    connect(this, SIGNAL(psuIsOutputEnabled()), &psuThread, SLOT(psuIsOutputEnabled()), Qt::QueuedConnection);

    connect(&psuThread, SIGNAL(resultOpenPort(QString)), this, SLOT(resultOpenPort(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultClosePort(QString)), this, SLOT(resultClosePort(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetOutputCurrent(qreal,QString)), this, SLOT(resultGetOutputCurrent(qreal,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetOutputCurrent(QString)), this, SLOT(resultSetOutputCurrent(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetOutputVoltage(QString)), this, SLOT(resultSetOutputVoltage(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetOutputVoltage(qreal,QString)), this, SLOT(resultGetOutputVoltage(qreal,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetActualOutputCurrent(qreal,QString)), this, SLOT(resultGetActualOutputCurrent(qreal,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetActualOutputVoltage(qreal,QString)), this, SLOT(resultGetActualOutputVoltage(qreal,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetOutputEnable(QString)), this, SLOT(resultGetOutputEnable(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetStatus(QString,QString)), this, SLOT(resultGetStatus(QString,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultGetIdentification(QString,QString)), this, SLOT(resultGetIdentification(QString,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultRecallPanelSetting(int,QString)), this, SLOT(resultRecallPanelSetting(int,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSavePanelSetting(QString)), this, SLOT(resultSavePanelSetting(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetOverCurrentProtection(QString)), this, SLOT(resultSetOverCurrentProtection(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultSetKeyboardLock(QString)), this, SLOT(resultSetKeyboardLock(QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultIsConstantCurrent(bool,QString)), this, SLOT(resultIsConstantCurrent(bool,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultIsConstantVoltage(bool,QString)), this, SLOT(resultIsConstantVoltage(bool,QString)), Qt::QueuedConnection);
    connect(&psuThread, SIGNAL(resultIsOutputEnabled(bool,QString)), this, SLOT(resultIsOutputEnabled(bool,QString)), Qt::QueuedConnection);

    connect(&ampsVoltsTimer, SIGNAL(timeout()), this, SLOT(ampsVoltsTimeout()), Qt::QueuedConnection);
}
