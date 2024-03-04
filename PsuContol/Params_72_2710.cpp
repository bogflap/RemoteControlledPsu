
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
    Q_UNUSED(command);
    Q_UNUSED(current);
}

void Params_72_2710::getReadCurrentCommand(QByteArray &command)
{
    Q_UNUSED(command);
}

void Params_72_2710::getActualCurrentCommand(QByteArray &command)
{
    Q_UNUSED(command);
}

void Params_72_2710::getOverCurrentProtectionCommand(QByteArray &command, bool &enable)
{
    Q_UNUSED(command);
    Q_UNUSED(enable);
}

void Params_72_2710::getEnableOutCommand(QByteArray &command, bool &enable)
{
    Q_UNUSED(command);
    Q_UNUSED(enable);

}

void Params_72_2710::getRecallPanelSettingsCommand(QByteArray &command, int &setting)
{
    Q_UNUSED(command);
    Q_UNUSED(setting);
}

void Params_72_2710::getSavePanelSettingsCommand(QByteArray &command, int &setting)
{
    Q_UNUSED(command);
    Q_UNUSED(setting);
}

void Params_72_2710::getKeyboardLockCommand(QByteArray &command, bool &enable)
{

    Q_UNUSED(command);
    Q_UNUSED(enable);
}
