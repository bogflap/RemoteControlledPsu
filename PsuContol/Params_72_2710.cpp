
#include "Params_72_2710.h"


int Params_72_2710::getMaxChannels()
{
    return 5;
}

double_t Params_72_2710::getMaxVoltageMv()
{
    return 30000.0;
}

double_t Params_72_2710::getMaxCurrentMa()
{
    return 5000.0;
}

int Params_72_2710::getInterCommandTimeMs()
{
    return 50;
}

int Params_72_2710::getRampTimeoutMs()
{
    return 1000;
}

void Params_72_2710::getVersionCommand(QByteArray &command)
{
    command = QString("*IDN?").toUtf8();
}

void Params_72_2710::getStatusCommand(QByteArray &command)
{
    command = QString("STATUS?").toUtf8();
}

void Params_72_2710::getSetVoltageCommand(QByteArray& command, double_t voltage)
{
    QString localCommand = QString("VSET1:%1").arg(voltage, 0, 'f', 2);
    command = localCommand.toUtf8();
}

void Params_72_2710::getReadVoltageCommand(QByteArray &command)
{
    QString localCommand = QString("VSET1?");
    command = localCommand.toUtf8();
}

void Params_72_2710::getActualVoltageCommand(QByteArray &command)
{
    QString localCommand = QString("VOUT1?");
    command = localCommand.toUtf8();
}

void Params_72_2710::getSetCurrentCommand(QByteArray &command, double_t current)
{
    QString localCommand = QString("ISET1:%1").arg(current, 0, 'f', 2);
    command = localCommand.toUtf8();
}

void Params_72_2710::getReadCurrentCommand(QByteArray &command)
{
    QString localCommand = QString("ISET1?");
    command = localCommand.toUtf8();
}

void Params_72_2710::getActualCurrentCommand(QByteArray &command)
{
    QString localCommand = QString("IOUT1?");
    command = localCommand.toUtf8();
}

void Params_72_2710::getOverCurrentProtectionCommand(QByteArray &command, bool &enable)
{
    QString localCommand;

    if (enable)
    {
        localCommand = QString("OCP1");
    }
    else
    {
        localCommand = QString("OCP0");
    }
    command = localCommand.toUtf8();
}

void Params_72_2710::setEnableOutCommand(QByteArray &command, bool &enable)
{
    QString localCommand;

    if (enable)
    {
        localCommand = QString("OUT1");
    }
    else
    {
        localCommand = QString("OUT0");
    }
    command = localCommand.toUtf8();
 }

void Params_72_2710::setRecallPanelSettingsCommand(QByteArray &command, int &setting)
{
    QString localCommand = QString("RCL%1").arg(setting);
    command = localCommand.toUtf8();
}

void Params_72_2710::setSavePanelSettingsCommand(QByteArray &command, int &setting)
{
    QString localCommand = QString("SAV%1").arg(setting);
    command = localCommand.toUtf8();
}

void Params_72_2710::setKeyboardLockCommand(QByteArray &command, bool &enable)
{
    QString localCommand;

    if (enable)
    {
        localCommand = QString("LOCK1");
    }
    else
    {
        localCommand = QString("LOCK0");
    }
    command = localCommand.toUtf8();
}
