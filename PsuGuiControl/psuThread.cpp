#include "psuThread.h"

PsuThread::PsuThread(QObject *parent)
    : QObject{parent}
{

}

void PsuThread::openPort(QString port)
{
    Q_UNUSED(port);
}

void PsuThread::closePort()
{

}

void PsuThread::setOutputCurrent(double_t current)
{
    Q_UNUSED(current);
}

void PsuThread::getOutputCurrent()
{

}

void PsuThread::setOutputVoltage(double_t voltage)
{
    Q_UNUSED(voltage);
}

void PsuThread::getOutputVoltage()
{

}

void PsuThread::getActualOutputCurrent()
{

}

void PsuThread::getActualOutputVoltage()
{

}

void PsuThread::setOutputEnable(bool enable)
{
    Q_UNUSED(enable);
}

void PsuThread::getStatus()
{

}

void PsuThread::getIdentification()
{

}

void PsuThread::recallPanelSetting(int number)
{
    Q_UNUSED(number);
}

void PsuThread::savePanelSetting(int number)
{
    Q_UNUSED(number);
}

void PsuThread::setOverCurrentPrtotection(bool enable)
{
    Q_UNUSED(enable);
}

void PsuThread::setKeyboardLock(bool enable)
{
    Q_UNUSED(enable);
}
