#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QDateTime>

#include "configurationdata.h"
#include "psuThread.h"

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

    // Signals to PSU thread
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
private slots:
    void    resultOpenPort(QString);
    void    resultClosePort(QString);
    void    resultSetOutputCurrent(QString error);
    void    resultGetOutputCurrent(qreal current, QString error);
    void    resultSetOutputVoltage(QString);
    void    resultGetOutputVoltage(qreal, QString);
    void    resultGetActualOutputCurrent(qreal current, QString);
    void    resultGetActualOutputVoltage(qreal voltage, QString);
    void    resultGetOutputEnable(QString);
    void    resultGetStatus(QString status, QString);
    void    resultGetIdentification(QString identification, QString);
    void    resultRecallPanelSetting(int, QString);
    void    resultSavePanelSetting(QString);
    void    resultSetOutputEnable(QString);
    void    resultSetOverCurrentProtection(QString);
    void    resultSetKeyboardLock(QString);
    void    resultIsConstantCurrent(bool, QString);
    void    resultIsConstantVoltage(bool, QString);
    void    resultIsOutputEnabled(bool, QString);

    // Slots for UI controls
private slots:
    void    selectIniFileClicked(bool checked);
    void    openIniFileClicked(bool checked);
    void    openSerialPortClicked(bool checked);
    void    startClicked(bool checked);
    void    pauseClicked(bool checked);
    void    stopClicked(bool checked);
    void    quitClicked(bool checked);
    void    timerExpired();

// GUI state variables
private:
    bool                quitting;
    enum    eChargingState
    {
        eNO_STATE,
        eINIT_PORT_OPENED,
        eSERIAL_PORT_OPENED,
        eCHARGE_NOT_STARTED,
        eCHARGE_STARTED,
        eCHARGE_PAUSED,
        eCHARGE_STOPPING
    };

    eChargingState      currentState;

    // Utility functions
private:
    void    makeConnections();
    void    getSerialPorts();
    void    setCurrentState(eChargingState newState);
    void    displayErrorDialog(QString &error);
    void    displayConfigurationId(QString &cId);
    void    displayPsuId(QString &pId);
    void    displayActualCurrent(qreal current);
    void    displayActualVoltage(qreal voltage);
    void    stopCharging(QString &reason);
    void    floatToDigits(  qreal value,
                            int &tens,
                            int &ones,
                            int &tenths,
                            int &hundreths,
                            int &thousandths);
    void    avpInitialise();
    void    avpConfigure();
    void    avpAddData(qreal &time, qreal &current, qreal &voltage);
    void    avpClearData();

private:
    Ui::MainWindow *ui;
    ConfigurationData   confData;
    PsuThread           psuThread;
    QThread             tPsuThread;
    QString             confFilePath;
    QTimer              timer;
    QDateTime           endTime;
    QDateTime           startTime;
    qreal               lastVoltage;
    qreal               lastCurrent;
    qreal               chargeCompleteCurrent;
    qreal               minAppliedVoltage;
};
#endif // MAINWINDOW_H
