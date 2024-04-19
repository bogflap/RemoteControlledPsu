#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QLabel>
#include <QTimer>

#include "psuThread.h"
#include "lcdcontroller.h"
#include "lcdfloattodigits.h"
#include "lcddigitstofloat.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// PSU thread commands
signals:
    void    psuOpenPort(QString);
    void    psuClosePort();
    void    psuSetOutputCurrent(qreal);
    void    psuGetOutputCurrent();
    void    psuSetOutputVoltage(qreal);
    void    psuGetOutputVoltage();
    void    psuGetActualOutputCurrent();
    void    psuGetActualOutputVoltage();
    void    psuSetOutputEnable(bool);
    void    psuGetStatus();
    void    psuGetIdentification();
    void    psuRecallPanelSetting(int);
    void    psuSavePanelSetting(int);
    void    psuSetOverCurrentProtection(bool);
    void    psuSetKeyboardLock(bool);
    void    psuIsConstantCurrent();
    void    psuIsConstantVoltage();
    void    psuIsOutputEnabled();

    // Slots for PSU thread results
public slots:
    void    resultOpenPort(QString);
    void    resultClosePort(QString);
    void    resultSetOutputCurrent(QString);
    void    resultGetOutputCurrent(qreal,QString);
    void    resultSetOutputVoltage(QString);
    void    resultGetOutputVoltage(qreal, QString);
    void    resultGetActualOutputCurrent(qreal, QString);
    void    resultGetActualOutputVoltage(qreal, QString);
    void    resultGetOutputEnable(QString);
    void    resultGetStatus(QString status, QString);
    void    resultGetIdentification(QString, QString);
    void    resultRecallPanelSetting(int, QString);
    void    resultSavePanelSetting(QString);
    void    resultSetOutputEnable(QString);
    void    resultSetOverCurrentProtection(QString);
    void    resultSetKeyboardLock(QString);
    void    resultIsConstantCurrent(bool, QString);
    void    resultIsConstantVoltage(bool, QString);
    void    resultIsOutputEnabled(bool, QString);

private:
    void    getSerialPorts();
    void    makeConnections();

// GUI slots
private slots:
    void    openClicked(bool);
    void    closeClicked(bool);
    void    quitClicked(bool);

    void    enableOutputClicked(bool);

    void    serialPortActivated(int);

    void    ampsTensUpClicked(bool);
    void    ampsTensDownClicked(bool);
    void    ampsOnesUpClicked(bool);
    void    ampsOnesDownClicked(bool);
    void    ampsTenthsUpClicked(bool);
    void    ampsTenthsDownClicked(bool);
    void    ampsHundrethsUpClicked(bool);
    void    ampsHundrethsDownClicked(bool);
    void    ampsThousandthsUpClicked(bool);
    void    ampsThousandthsDownClicked(bool);

    void    ampsApplyClicked(bool);
    void    ampsResetClicked(bool);

    void    voltsApplyClicked(bool);
    void    voltsResetClicked(bool);
    void    voltsTensUpClicked(bool);
    void    voltsTensDownClicked(bool);
    void    voltsOnesUpClicked(bool);
    void    voltsOnesDownClicked(bool);
    void    voltsTenthsUpClicked(bool);
    void    voltsTenthsDownClicked(bool);
    void    voltsHundrethsUpClicked(bool);
    void    voltsHundrethsDownClicked(bool);
    void    voltsThousandthsUpClicked(bool);
    void    voltsThousandthsDownClicked(bool);

    // General slots
private slots:
    void    ampsVoltsTimeout();

private:
    void    showStatusText(QString text);
    void    setupLcds();
    void    AmpsVoltsEnable(bool enable);
    void    setActualAmps(qreal value);
    void    setActualVolts(qreal value);
    void    setOutputVolts(qreal value);
    void    setOutputAmps(qreal value);
    void    setEnableOutputColour(bool bOutputEnabled);

private:
    Ui::MainWindow      *ui;
    PsuThread           psuThread;
    QThread             tPsuThread;
    QTimer              ampsVoltsTimer;
    bool                quitting;
    lcdController       ampsLcd;
    lcdController       voltsLcd;
    lcdFloatToDigits    floatToDigits;
    lcdDigitsToFloat    digitsToFloat;
    QString             outputEnabledStyle;
    QString             outputDisabledStyle;
};
#endif // MAINWINDOW_H
