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

private:
    void    makeConnections();
    void    getSerialPorts();
    void    displayErrorDialog(QString &error);
    void    displayConfigurationId(QString &cId);
    void    stopCharging(QString &reason);

private:
    Ui::MainWindow *ui;
    ConfigurationData   confData;
    PsuThread           psuThread;
    QThread             tPsuThread;
    QString             confFilePath;
    QTimer              timer;
    QDateTime           endTime;
    int                 chargeCompleteCurrent;
    int                 minAppliedVoltage;
    bool                quitting;
};
#endif // MAINWINDOW_H
