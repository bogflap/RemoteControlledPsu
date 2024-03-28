#ifndef PARAMS_72_2710_H
#define PARAMS_72_2710_H

#include <QByteArray>
#include <QString>

class Params_72_2710
{
public:
    // Maximum values
    int         getMaxChannels();
    double_t    getMaxVoltageMv();
    double_t    getMaxCurrentMa();

    // Minimum times
    int         getInterCommandTimeMs();
    int         getRampTimeoutMs();


    // Identification and status commands
    void        getVersionCommand(QByteArray& command);
    void        getStatusCommand(QByteArray& command);

    // Voltage commands
    void        getSetVoltageCommand(QByteArray& command, double_t voltage);
    void        getReadVoltageCommand(QByteArray& command);
    void        getActualVoltageCommand(QByteArray& command);

    // Current commands
    void        getSetCurrentCommand(QByteArray& command, double_t current);
    void        getReadCurrentCommand(QByteArray& command);
    void        getActualCurrentCommand(QByteArray& command);
    void        getOverCurrentProtectionCommand(QByteArray& command, bool& enable);

    // Enable output
    void        setEnableOutCommand(QByteArray& command, bool& enable);

    // Panel setting commands
    void        setRecallPanelSettingsCommand(QByteArray& command, int& setting);
    void        setSavePanelSettingsCommand(QByteArray& command, int& setting);

    // Keyboard lock command
    void        setKeyboardLockCommand(QByteArray& command, bool& enable);
};

#endif // PARAMS_72_2710_H
