//#############################################################################
//
// This file is specfic to the TENMA 72-2710 DC Power Supply 30v 5A
//
//#############################################################################

#ifndef PSUCONTOL_H
#define PSUCONTOL_H

#include <QTextStream>

#include "PsuContol_global.h"

class PSUCONTOL_EXPORT PsuContol
{
public:
    PsuContol();

    bool    open(const QString& portName);
    bool    close();

    bool    getVersion(QString& version);
    bool    getStatus(QTextStream& status);
    void    getMaximumVoltageMv(double_t& voltageMv);
    void    getMaximumCurrentMa(double_t& currentMa);

    bool    readCurrent(double_t& current);
    bool    setCurrent(double_t mCurrent);
    bool    actualCurrent(double_t& current);
    bool    setOverCurrentProtection(bool& enable);

    bool    readVoltage(double_t& voltage);
    bool    setVoltage(double_t mVoltage);
    bool    rampUpVoltage(double_t &startVoltage,
                          double_t &endVoltage,
                          double_t &rampTimeMs,
                          unsigned int &steps);
    bool    actualVoltage(double_t& voltage);

    bool    setOverVoltageProtection(bool& enable);

    bool    setBeep(bool& enable);
    bool    setChannelOutput(bool& enable);
    bool    setPanelLock(bool& enable);
    bool    saveConfiguration(int number);
    bool    recallConfiguration(int number);
    void    getError(QString& error);

    bool    isConstantCurrent(bool& constantCurrent);
    bool    isConstantVoltage(bool& constantVoltage);
    bool    isOutputEnabled(bool& outputEnabled);

    // Utility functions
    void    waitForMilliSeconds(int milliSeconds);

private:
    bool    getRawStatus(unsigned char& status);
    bool    waitForActualVoltage(double_t &voltage,
                                 int &timeoutMs,
                                 int &actualTimeMs);

    bool    sendCommand(QByteArray& command);
    bool    receiveResponse(QByteArray& response);

    bool    checkChannel(int channel);
    bool    checkVoltage(float_t voltage);
    bool    checkCurrent(float_t current);

    QString lastError;

};

#endif // PSUCONTOL_H
