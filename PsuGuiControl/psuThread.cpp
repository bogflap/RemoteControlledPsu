#include <QDebug>

#include "psuThread.h"
#include "psucontol.h"

PsuContol   gPsuControl;

PsuThread::PsuThread(QObject *parent)
    : QObject{parent}
{

}

qreal PsuThread::getMaxCurrent()
{
    qreal   maxMa;

    gPsuControl.getMaximumCurrentMa(maxMa);

    return maxMa;
}

qreal PsuThread::getMaxVoltage()
{
    qreal   maxMv;

    gPsuControl.getMaximumVoltageMv(maxMv);

    return maxMv;
}

void PsuThread::psuOpenPort(QString port)
{
    QString errorText = "";

    if (!gPsuControl.open(port))
    {
        gPsuControl.getError(errorText);
    }

    emit resultOpenPort(errorText);
}

void PsuThread::psuClosePort()
{
    QString errorText;

    if (gPsuControl.getPortOpen())
    {
        errorText = "";
        gPsuControl.close();
    }
    else
    {
        errorText = "Port not open";
    }

    emit resultClosePort(errorText);
}

void PsuThread::psuSetOutputCurrent(qreal current)
{
    QString error("");

    if (gPsuControl.setCurrent(current))
    {
        gPsuControl.getError(error);
    }

    emit resultSetOutputCurrent(error);
}

void PsuThread::psuGetOutputCurrent()
{
    qreal   current;

    QString error("");

    if (!gPsuControl.readCurrent(current))
    {
        gPsuControl.getError(error);
    }

    emit resultGetOutputCurrent(current, error);
}

void PsuThread::psuSetOutputVoltage(qreal voltage)
{
    Q_UNUSED(voltage);
}

void PsuThread::psuGetOutputVoltage()
{
    qreal   voltage;

    QString error("");

    if (!gPsuControl.readVoltage(voltage))
    {
        gPsuControl.getError(error);
    }

    emit resultGetOutputVoltage(voltage, error);
}

void PsuThread::psuGetActualOutputCurrent()
{
    qreal   current;

    QString error("");

    if (!gPsuControl.actualCurrent(current))
    {
        gPsuControl.getError(error);
    }

    emit resultGetActualOutputCurrent(current, error);
}

void PsuThread::psuGetActualOutputVoltage()
{
    qreal   voltage;

    QString error("");

    if (!gPsuControl.actualVoltage(voltage))
    {
        gPsuControl.getError(error);
    }

    emit resultGetActualOutputVoltage(voltage, error);
}

void PsuThread::psuSetOutputEnable(bool enable)
{
    Q_UNUSED(enable);
}

void PsuThread::psuGetStatus()
{
    QString status;

    QString error("");
    QTextStream tStatus(&status);

    if (!gPsuControl.getStatus(tStatus))
    {
        gPsuControl.getError(error);
    }

    emit resultGetStatus(status, error);
}

void PsuThread::psuGetIdentification()
{
    QString id("");
    QString error("");

    if (!gPsuControl.getVersion(id))
    {
        gPsuControl.getError(error);
    }

    emit resultGetIdentification(id, error);
}

void PsuThread::psuRecallPanelSetting(int number)
{
    Q_UNUSED(number);
}

void PsuThread::psuSavePanelSetting(int number)
{
    Q_UNUSED(number);
}

void PsuThread::psuSetOverCurrentProtection(bool enable)
{
    Q_UNUSED(enable);
}

void PsuThread::psuSetKeyboardLock(bool enable)
{
    Q_UNUSED(enable);
}

void PsuThread::psuIsConstantCurrent()
{
    bool    isCC = false;
    QString error("");

    if (!gPsuControl.isConstantCurrent(isCC))
    {
        gPsuControl.getError(error);
    }

    emit resultIsConstantCurrent(isCC, error);
}

void PsuThread::psuIsConstantVoltage()
{
    bool    isCV = false;
    QString error("");

    gPsuControl.isConstantVoltage(isCV);
    gPsuControl.getError(error);

    emit resultIsConstantCurrent(isCV, error);
}

void PsuThread::psuIsOutputEnabled()
{
    bool    isOE = false;
    QString error("");

    if (!gPsuControl.isOutputEnabled(isOE))
    {
        gPsuControl.getError(error);
    }

    emit resultIsOutputEnabled(isOE, error);
}
