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
    void    psuSetOverCurrentPrtotection(bool);
    void    psuSetKeyboardLock(bool);
    void    psuIsConstantCurrent();
    void    psuIsConstantVoltage();
    void    psuIsOutputEnabled();

signals:
    void    resultOpenPort(QString);
    void    resultClosePort(QString);
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
    void    resultSetOverCurrentPrtotection(QString);
    void    resultSetKeyboardLock(QString);
    void    resultIsConstantCurrent(bool, QString);
    void    resultIsConstantVoltage(bool, QString);
    void    resultIsOutputEnabled(bool, QString);
};

#endif // PSUTHREAD_H
