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
    void    getMaximumVoltageMv(qreal& voltageMv);
    void    getMaximumCurrentMa(qreal& currentMa);

    bool    readCurrent(qreal& current);
    bool    setCurrent(qreal mCurrent);
    bool    actualCurrent(qreal& current);
    bool    setOverCurrentProtection(bool& enable);

    bool    readVoltage(qreal& voltage);
    bool    setVoltage(qreal mVoltage);
    bool    rampUpVoltage(qreal &startVoltage,
                          qreal &endVoltage,
                          qreal &rampTimeMs,
                          unsigned int &steps);
    bool    actualVoltage(qreal& voltage);

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

    bool    getPortOpen() const;

private:
    bool    getRawStatus(unsigned char& status);
    bool    waitForActualVoltage(qreal &voltage,
                                 int &timeoutMs,
                                 int &actualTimeMs);

    bool    sendCommand(QByteArray& command);
    bool    receiveResponse(QByteArray& response);

    bool    checkChannel(int channel);
    bool    checkVoltage(float_t voltage);
    bool    checkCurrent(float_t current);

private:
    QString lastError;
};

#endif // PSUCONTOL_H
