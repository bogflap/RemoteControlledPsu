#include <QDebug>

#include "psuThread.h"
#include "psucontol.h"

PsuContol   gPsuControl;

PsuThread::PsuThread(QObject *parent)
    : QObject{parent}
{

}

void PsuThread::psuOpenPort(QString port)
{
    QString errorText = "";

    gPsuControl.open(port);
    gPsuControl.getError(errorText);

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
    Q_UNUSED(current);
}

void PsuThread::psuGetOutputCurrent()
{

}

void PsuThread::psuSetOutputVoltage(qreal voltage)
{
    Q_UNUSED(voltage);
}

void PsuThread::psuGetOutputVoltage()
{

}

void PsuThread::psuGetActualOutputCurrent()
{

}

void PsuThread::psuGetActualOutputVoltage()
{

}

void PsuThread::psuSetOutputEnable(bool enable)
{
    Q_UNUSED(enable);
}

void PsuThread::psuGetStatus()
{

}

void PsuThread::psuGetIdentification()
{

}

void PsuThread::psuRecallPanelSetting(int number)
{
    Q_UNUSED(number);
}

void PsuThread::psuSavePanelSetting(int number)
{
    Q_UNUSED(number);
}

void PsuThread::psuSetOverCurrentPrtotection(bool enable)
{
    Q_UNUSED(enable);
}

void PsuThread::psuSetKeyboardLock(bool enable)
{
    Q_UNUSED(enable);
}
