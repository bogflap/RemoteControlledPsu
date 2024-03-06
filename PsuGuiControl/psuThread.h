#ifndef PSUTHREAD_H
#define PSUTHREAD_H

#include <QObject>

class PsuThread : public QObject
{
    Q_OBJECT
public:
    explicit PsuThread(QObject *parent = nullptr);

private slots:
    void    openPort(QString port);
    void    closePort();
    void    setOutputCurrent(double_t current);
    void    getOutputCurrent();
    void    setOutputVoltage(double_t voltage);
    void    getOutputVoltage();
    void    getActualOutputCurrent();
    void    getActualOutputVoltage();
    void    setOutputEnable(bool enable);
    void    getStatus();
    void    getIdentification();
    void    recallPanelSetting(int number);
    void    savePanelSetting(int number);
    void    setOverCurrentPrtotection(bool enable);
    void    setKeyboardLock(bool enable);

signals:
    void    resultOpenPort(QString errorString);
    void    resultClosePort(QString errorString);
    void    resultSetOutputCurrent(QString errorString);
    void    resultGetOutputCurrent(double_t current,QString errorString);
    void    resultSetOutputVoltage(QString errorString);
    void    resultGetOutputVoltage(double_t voltage, QString errorString);
    void    resultGetActualOutputCurrent(double_t current, QString errorString);
    void    resultGetActualOutputVoltage(double_t voltage, QString errorString);
    void    resultGetOutputEnable(QString errorString);
    void    resultGetStatus(QString status, QString errorString);
    void    resultGetIdentification(QString identification, QString errorString);
    void    resultRecallPanelSetting(int number, QString errorString);
    void    resultSavePanelSetting(QString errorString);
    void    resultSetOverCurrentPrtotection(QString errorString);
    void    resultSetKeyboardLock(QString errorString);
};

#endif // PSUTHREAD_H
