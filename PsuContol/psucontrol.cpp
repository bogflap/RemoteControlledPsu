#include <QSerialPort>
#include <QTextStream>
#include <QTime>

#include "psucontrol.h"
#include "Params_72_2710.h"

Q_GLOBAL_STATIC(QSerialPort, serialPort);
Q_GLOBAL_STATIC(Params_72_2710, psuParams);

// Need this to prevent high levels from output commands to quickly for the PSU
// to handle
#define INTER_COMMAND_DELAY         50      // milliseconds
#define INTER_COMMAND_LONG_DELAY    100     // milliseconds

PsuContol::PsuContol()
{
    lastError = "";
}

bool PsuContol::open(const QString &portName)
{
    QString port("");
    bool    result = false;

    if (portName == "")
        port = "/dev/ttyACM0";
    else
    {
        port = portName;
    }

    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setPortName(port);

    result = serialPort->open(QIODevice::ReadWrite);
    lastError = "";
    if (!result)
    {
        lastError = serialPort->errorString();
    }

    return result;
}

bool PsuContol::close()
{
    bool    result = true;

    serialPort->close();

    return result;
}

bool PsuContol::getVersion(QString &version)
{
    bool        bResult = true;
    QByteArray  byteCommand;
    QByteArray  byteVersion;

    version.clear();

    while(true)
    {
        psuParams->getVersionCommand(byteCommand);

        if (!sendCommand(byteCommand))
        {
            bResult = false;
            break;
        }

        // Do not know length of response yet so leave it at the default
        if (!receiveResponse(byteVersion))
        {
            bResult = false;
            break;
        }

        version = QString::fromUtf8(byteVersion);

        break;
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return bResult;
}

bool PsuContol::getStatus(QTextStream &status)
{
    bool                result = true;
    unsigned char       uCharStatus = 0;;
    QByteArray          byteCommand;
    QByteArray          byteStatus;

    while (true)
    {
        psuParams->getStatusCommand(byteCommand);

        if (!sendCommand(byteCommand))
        {
            result = false;
            break;
        }

        if (!receiveResponse(byteStatus, 1))
        {
            result = false;
            break;
        }

        uCharStatus = *byteStatus.constData();

        if( uCharStatus & 0x01)
        {
            status << "CV mode" << Qt::endl;
        }
        else
        {
            status << "CC mode" << Qt::endl;
        }

        if ( uCharStatus & 0x40)
        {
            status << "Output ON" << Qt::endl;
        }
        else
        {
            status << "Output OFF" << Qt::endl;
        }

        break;
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

void PsuContol::getMaximumVoltageMv(qreal &voltageMv)
{
    voltageMv = psuParams->getMaxVoltageMv();
}

void PsuContol::getMaximumCurrentMa(qreal &currentMa)
{
    currentMa = psuParams->getMaxCurrentMa();
}

bool PsuContol::readCurrent(qreal &current)
{
    Q_UNUSED(current);
    bool    result = true;

    lastError = "Not implemented";

    return result;
}

bool PsuContol::setCurrent(qreal mCurrent)
{
    bool    result = true;

    if ((mCurrent * 1000.0) > psuParams->getMaxCurrentMa())
    {
        QTextStream ts(&lastError);

        ts  << "Maximum current exceeded (Requested "
            << mCurrent
            << " Maximum "
            << psuParams->getMaxCurrentMa()
            << "mA)"
            << Qt::endl;
    }
    else
    {
        QByteArray  command;

        psuParams->getSetCurrentCommand(command, mCurrent);
        result = sendCommand(command);
    }

    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

bool PsuContol::actualCurrent(qreal &current)
{
    QByteArray  command;
    QByteArray  readCurrent;
    QString     stringCurrent;

    bool        result = true;

    while(true)
    {
        psuParams->getActualCurrentCommand(command);
        if (!sendCommand(command))
        {
            result = false;
            break;
        }

        // Do not know length of response yet so leave it at the default
        if (!receiveResponse(readCurrent))
        {
            result = false;
            break;
        }

        stringCurrent = QString::fromUtf8(readCurrent);
        current = stringCurrent.toDouble();

        break;
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

bool PsuContol::setOverCurrentProtection(bool &enable)
{
    QByteArray  command;

    bool        result = true;

    psuParams->getOverCurrentProtectionCommand(command, enable);
    if (!sendCommand(command))
    {
        result = false;
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

bool PsuContol::readVoltage(qreal &voltage)
{
    QByteArray  command;
    QByteArray  readVoltage;
    QString     stringVoltage;

    bool        result = true;

    while(true)
    {
        psuParams->getReadVoltageCommand(command);
        if (!sendCommand(command))
        {
            result = false;
            break;
        }

        // Do not know length of response yet so leave it at the default
        if (!receiveResponse(readVoltage))
        {
            result = false;
            break;
        }

        stringVoltage = QString::fromUtf8(readVoltage);
        voltage = stringVoltage.toDouble();

        break;
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

bool PsuContol::setVoltage(qreal voltage)
{
    bool    result = false;

    if ((voltage * 1000.0) > psuParams->getMaxVoltageMv())
    {
        QTextStream ts(&lastError);

        ts << "Maximum voltage exceeded (Requested "
           << voltage
           << " Maxiumum "
           << psuParams->getMaxVoltageMv()
           << "mV)"
           << Qt::endl;
    }
    else
    {
        QByteArray  command;

        psuParams->getSetVoltageCommand(command, voltage);
        result = sendCommand(command);
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

bool PsuContol::rampUpVoltage(qreal &startVoltage,
                              qreal &endVoltage,
                              qreal &rampTimeMs,
                              unsigned int &steps)
{
    qreal    stepVoltage;
    qreal    thisStepVoltage;
    qreal    maxVoltage;
    qreal    stepTime;
    qreal    interCommandTime;
    int         iStepTimeMs;
    int         iRampTimeMs;

    int         iActualRampTimeMs = 0;
    bool        result  = false;
    bool        forResult = true;

    while(true)
    {
        stepVoltage = (endVoltage - startVoltage) / (float_t) steps;
        stepTime = rampTimeMs / (qreal) steps;
        interCommandTime = qreal(psuParams->getInterCommandTimeMs());
        iStepTimeMs = (int) stepTime;

        // Do some error checking first
        maxVoltage = psuParams->getMaxVoltageMv();
        if (startVoltage > maxVoltage)
        {
            QTextStream ts(&lastError);
            ts << "Start voltage exceeds maxium voltage (Requested "
               << startVoltage
               << " Maximum "
               << maxVoltage
               << "mV)"
               << Qt::endl;
            break;
        }
        if (endVoltage > maxVoltage)
        {
            QTextStream ts(&lastError);
            ts << "End voltage exceeds maxium voltage (Requested "
               << endVoltage
               << " Maximum "
               << maxVoltage
               << "mV)"
               << Qt::endl;
            break;
        }
        if (stepTime < interCommandTime)
        {
            QTextStream ts(&lastError);
            ts << "Step time less than minimum (Requested "
               << stepTime
               << " Minimum "
               << interCommandTime
               << "mS)"
               << Qt::endl;
            break;
        }
        if (steps == 0)
        {
            QTextStream ts(&lastError);
            ts << "Steps must be greater than zero"
               << Qt::endl;
            break;
        }

        // Start the voltage ramp
        thisStepVoltage = startVoltage;
        iRampTimeMs = psuParams->getRampTimeoutMs();
        for (unsigned int step = 0; step <= steps; step++)
        {
            // Set the requested voltage
            if (!setVoltage(thisStepVoltage))
            {
                // Some kind of error
                forResult = false;
                break;
            }

            // Wait for the actualvoltage to be achieved
            if (!waitForActualVoltage(thisStepVoltage,
                                      iRampTimeMs,
                                      iActualRampTimeMs))
            {
                // Did not achieve actual voltage in ramp timeout
                forResult = false;
                break;
            }

            thisStepVoltage += stepVoltage;
            waitForMilliSeconds(iStepTimeMs - iActualRampTimeMs);
        }

        if (!forResult)
        {
            break;
        }

        // Successful outcome
        result = true;
        break;
    }


    return result;
}

bool PsuContol::actualVoltage(qreal &voltage)
{
    QByteArray  command;
    QByteArray  readVoltage;
    QString     stringVoltage;

    bool        result = true;

    while(true)
    {
        psuParams->getActualVoltageCommand(command);
        if (!sendCommand(command))
        {
            result = false;
            break;
        }

        // Do not know length of response yet so leave it at the default
        if (!receiveResponse(readVoltage))
        {
            result = false;
            break;
        }

        stringVoltage = QString::fromUtf8(readVoltage);
        voltage = stringVoltage.toDouble();

        break;
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

bool PsuContol::setOverVoltageProtection(bool &enable)
{
    Q_UNUSED(enable);
    bool    result = true;

    waitForMilliSeconds(INTER_COMMAND_DELAY);

    lastError = "Not implemented";

    return result;
}

bool PsuContol::setBeep(bool &enable)
{
    Q_UNUSED(enable);
    bool    result = true;

    waitForMilliSeconds(INTER_COMMAND_DELAY);

    lastError = "Not implemented";

    return result;
}

bool PsuContol::setChannelOutput(bool &enable)
{
    QByteArray  command;

    bool    result = true;

    psuParams->setEnableOutCommand(command, enable);
    if (!sendCommand(command))
    {
        result = false;
    }

    // Long delay here to allow for possible OUTPUT Current Protection to work
    waitForMilliSeconds(INTER_COMMAND_LONG_DELAY);

    return result;
}

bool PsuContol::setPanelLock(bool &enable)
{
    Q_UNUSED(enable);
    bool    result = true;

    waitForMilliSeconds(INTER_COMMAND_DELAY);

    lastError = "Not implemented";

    return result;
}

bool PsuContol::saveConfiguration(int number)
{
    Q_UNUSED(number);
    bool    result = true;

    waitForMilliSeconds(INTER_COMMAND_DELAY);

    lastError = "Not implemented";

    return result;
}

bool PsuContol::recallConfiguration(int number)
{
    Q_UNUSED(number);
    bool    result = true;

    waitForMilliSeconds(INTER_COMMAND_DELAY);

    lastError = "Not implemented";

    return result;
}

void PsuContol::getError(QString &error)
{
    if (lastError == "")
    {
        error = serialPort->errorString();
    }
    else
    {
        error = lastError;
        lastError = "";
    }
}

bool PsuContol::isConstantCurrent(bool &constantCurrent)
{
    bool            result = true;
    unsigned char   status = 0;

    while(true)
    {
        if (!getRawStatus(status))
        {
            result = false;
            break;
        }

        constantCurrent = (status & 0x01) ? false : true;

        break;
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

bool PsuContol::isConstantVoltage(bool &constantVoltage)
{
    bool            result = true;
    unsigned char   status = 0;

    while(true)
    {
        if (!getRawStatus(status))
        {
            result = false;
            break;
        }

        constantVoltage = (status & 0x01) ? true : false;

        break;
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

bool PsuContol::isOutputEnabled(bool &outputEnabled)
{
    bool            result = true;
    unsigned char   status = 0;

    while(true)
    {
        if (!getRawStatus(status))
        {
            result = false;
            break;
        }

        outputEnabled = (status & 0x40) ? true : false;

        break;
    }

    // Avoid overloading the PSU interface
    waitForMilliSeconds(INTER_COMMAND_DELAY);

    return result;
}

bool PsuContol::getRawStatus(unsigned char &status)
{
    bool                result = true;
    QByteArray          byteCommand;
    QByteArray          byteStatus;

    while (true)
    {
        psuParams->getStatusCommand(byteCommand);

        if (!sendCommand(byteCommand))
        {
            result = false;
            break;
        }

        if (!receiveResponse(byteStatus, 1))
        {
            result = false;
            break;
        }

        status = *byteStatus.constData();

        break;
    }

    return result;
}

bool PsuContol::waitForActualVoltage(qreal &voltage, int &timeoutMs, int &actualTimeMs)
{
    QTime       endTime;
    QTime       thisTime;

    bool        result = false;

    QTime       startTime = QTime::currentTime();
    int         interCommandTime = psuParams->getInterCommandTimeMs();
    qreal    nowVoltage = 0.0;

    endTime = startTime.addMSecs(timeoutMs);

    bool    innerResult = false;
    while(true)
    {
        // Wait before sending command as the Psu cannot handle to much at once
        waitForMilliSeconds(interCommandTime);

        innerResult = actualVoltage(nowVoltage);
        if (!innerResult)
        {
            // some kind of error occurred
            break;
        }

        if (nowVoltage == voltage)
        {
            // Voltage has settled
            actualTimeMs = startTime.msec() - QTime::currentTime().msec();
            result = true;
            break;
        }

        thisTime = QTime::currentTime();
        if (endTime <= thisTime)
        {
            // Timed out
            lastError = "Timed out waiting for voltage to settle";
            break;
        }

        // Loop round waiting for success
    }

    return result;
}

bool PsuContol::sendCommand(QByteArray &command)
{
    bool    result = true;

    serialPort->write(command);
    if (!serialPort->waitForBytesWritten())
    {
        result = false;
    }

    return result;
}

bool PsuContol::receiveResponse(QByteArray &response, int expected)
{
    bool    result = true;

    while (true)
    {
        if (!serialPort->waitForReadyRead())
        {
            result = false;
            break;
        }

        response = serialPort->readAll();
        if (expected > 0)
        {
            // A length is expected so check it
            if (response.length() != expected)
            {
                result = false;
            }
        }
        else
        {
            result = true;
        }

        break;
    }

    return result;
}

bool PsuContol::checkChannel(int channel)
{
    bool    result = true;

    if (channel > psuParams->getMaxChannels())
    {
        result = false;
    }

    return result;
}

bool PsuContol::checkVoltage(float_t voltage)
{
    bool    result = true;

    if (voltage > psuParams->getMaxVoltageMv())
    {
        result = false;
    }

    return result;
}

bool PsuContol::checkCurrent(float_t current)
{
    bool    result = true;

    if (current > psuParams->getMaxCurrentMa())
    {
        result = false;
    }

    return result;
}

bool PsuContol::getPortOpen() const
{
    return serialPort->isOpen();
}

void PsuContol::waitForMilliSeconds(int milliSeconds)
{
    long    longMilliSeconds = milliSeconds;

    struct timespec ts =
    {
        (longMilliSeconds / 1000),  // Seconds
        (longMilliSeconds % 1000) * 1000 * 1000 // nano seconds
    };

    nanosleep(&ts, NULL);
}
