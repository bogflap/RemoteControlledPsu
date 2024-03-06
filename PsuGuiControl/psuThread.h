#ifndef PSUTHREAD_H
#define PSUTHREAD_H

#include <QObject>

class PsuThread : public QObject
{
    Q_OBJECT

public:
    explicit PsuThread(QObject *parent = nullptr);

public slots:
    void    psuOpenPort(QString);
    void    psuClosePort();
    void    psuSetOutputCurrent(double_t);
    void    psuGetOutputCurrent();
    void    psuSetOutputVoltage(double_t);
    void    psuGetOutputVoltage();
    void    psuGetActualOutputCurrent();
    void    psuGetActualOutputVoltage();
    void    psuSetOutputEnable(bool);
    void    psuGetStatus();
    void    psuGetIdentification();
    void    psuRecallPanelSetting(int);
    void    psuSavePanelSetting(int);
    void    psuSetOverCurrentPrtotection(bool);
    void    psuSetKeyboardLock(bool);

signals:
    void    resultOpenPort(QString);
    void    resultClosePort(QString);
    void    resultGetOutputCurrent(double_t,QString);
    void    resultSetOutputVoltage(QString);
    void    resultGetOutputVoltage(double_t, QString);
    void    resultGetActualOutputCurrent(double_t, QString);
    void    resultGetActualOutputVoltage(double_t, QString);
    void    resultGetOutputEnable(QString);
    void    resultGetStatus(QString status, QString);
    void    resultGetIdentification(QString, QString);
    void    resultRecallPanelSetting(int, QString);
    void    resultSavePanelSetting(QString);
    void    resultSetOverCurrentPrtotection(QString);
    void    resultSetKeyboardLock(QString);
};

#endif // PSUTHREAD_H
