#include <QDebug>

#include "psuThread.h"

PsuThread::PsuThread(QObject *parent)
    : QObject{parent}
{

}

void PsuThread::psuOpenPort(QString port)
{
    Q_UNUSED(port);

    qDebug() << "psuOpenPort";
}

void PsuThread::psuClosePort()
{

}

void PsuThread::psuSetOutputCurrent(double_t current)
{
    Q_UNUSED(current);
}

void PsuThread::psuGetOutputCurrent()
{

}

void PsuThread::psuSetOutputVoltage(double_t voltage)
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
