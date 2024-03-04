#include <QSerialPort>
#include <QTextStream>
#include <QTime>

#include "psucontol.h"
#include "Params_72_2710.h"

Q_GLOBAL_STATIC(QSerialPort, serialPort);
Q_GLOBAL_STATIC(Params_72_2710, psuParams);

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
    bool        bResult = false;
    QByteArray  byteCommand;
    QByteArray  byteVersion;

    version.clear();

    while(true)
    {
        psuParams->getVersionCommand(byteCommand);

        if (!sendCommand(byteCommand))
        {
            break;
        }

        if (!receiveResponse(byteVersion))
        {
            break;
        }

        version = QString::fromUtf8(byteVersion);

        bResult = true;
        break;
    }

    return bResult;
}

bool PsuContol::getStatus(QTextStream &status)
{
    bool                result = false;
    unsigned char       uCharStatus = 0;;
    QByteArray          byteCommand;
    QByteArray          byteStatus;

    while (true)
    {
        psuParams->getStatusCommand(byteCommand);

        if (!sendCommand(byteCommand))
        {
                break;
        }

        if (!receiveResponse(byteStatus))
        {
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

        result = true;
        break;
    }

    return result;
}

void PsuContol::getMaximumVoltageMv(double_t &voltageMv)
{
    voltageMv = psuParams->getMaxVoltageMv();
}

void PsuContol::getMaximumCurrentMa(double_t &currentMa)
{
    currentMa = psuParams->getMaxCurrentMa();
}

bool PsuContol::readCurrent(double_t &current)
{
    Q_UNUSED(current);
    bool    result = true;

    return result;
}

bool PsuContol::setCurrent(double_t mCurrent)
{
    bool    result = false;

    if (mCurrent > psuParams->getMaxCurrentMa())
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

    return result;
}

bool PsuContol::actualCurrent(double_t &current)
{
    Q_UNUSED(current);
    bool    result = true;

    return result;
}

bool PsuContol::setOverCurrentProtection(bool &enable)
{
    Q_UNUSED(enable);
    bool    result = true;

    return result;
}

bool PsuContol::readVoltage(double_t &voltage)
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

        if (!receiveResponse(readVoltage))
        {
            result = false;
            break;
        }

        stringVoltage = QString::fromUtf8(readVoltage);
        voltage = stringVoltage.toDouble();

        break;
    }

    return result;
}

bool PsuContol::setVoltage(double_t mVoltage)
{
    bool    result = false;

    if (mVoltage > psuParams->getMaxVoltageMv())
    {
        QTextStream ts(&lastError);

        ts << "Maximum voltage exceeded (Requested "
           << mVoltage
           << " Maxiumum "
           << psuParams->getMaxVoltageMv()
           << "mV)"
           << Qt::endl;
    }
    else
    {
        QByteArray  command;

        psuParams->getSetVoltageCommand(command, mVoltage);
        result = sendCommand(command);
    }

    return result;
}

bool PsuContol::rampUpVoltage(double_t &startVoltage,
                              double_t &endVoltage,
                              double_t &rampTimeMs,
                              unsigned int &steps)
{
    double_t    stepVoltage;
    double_t    thisStepVoltage;
    double_t    maxVoltage;
    double_t    stepTime;
    double_t    interCommandTime;
    int         iStepTimeMs;
    int         iRampTimeMs;

    int         iActualRampTimeMs = 0;
    bool        result  = false;
    bool        forResult = true;

    while(true)
    {
        stepVoltage = (endVoltage - startVoltage) / (float_t) steps;
        stepTime = rampTimeMs / (double_t) steps;
        interCommandTime = double_t(psuParams->getInterCommandTimeMs());
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

bool PsuContol::actualVoltage(double_t &voltage)
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

        if (!receiveResponse(readVoltage))
        {
            result = false;
            break;
        }

        stringVoltage = QString::fromUtf8(readVoltage);
        voltage = stringVoltage.toDouble();

        break;
    }

    return result;
}

bool PsuContol::setOverVoltageProtection(bool &enable)
{
    Q_UNUSED(enable);
    bool    result = true;

    return result;
}

bool PsuContol::setBeep(bool &enable)
{
    Q_UNUSED(enable);
    bool    result = true;

    return result;
}

bool PsuContol::setChannelOutput(bool &enable)
{
    Q_UNUSED(enable);
    bool    result = true;

    return result;
}

bool PsuContol::setPanelLock(bool &enable)
{
    Q_UNUSED(enable);
    bool    result = true;

    return result;
}

bool PsuContol::saveConfiguration(int number)
{
    Q_UNUSED(number);
    bool    result = true;

    return result;
}

bool PsuContol::recallConfiguration(int number)
{
    Q_UNUSED(number);
    bool    result = true;

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
    bool            result = false;
    unsigned char   status = 0;

    while(true)
    {
        if (!getRawStatus(status))
        {
            break;
        }

        constantCurrent = (status & 0x01) ? false : true;

        result = true;
        break;
    }

    return result;
}

bool PsuContol::isConstantVoltage(bool &constantVoltage)
{
    bool            result = false;
    unsigned char   status = 0;

    while(true)
    {
        if (!getRawStatus(status))
        {
            break;
        }

        constantVoltage = (status & 0x01) ? true : false;

        result = true;
        break;
    }

    return result;
}

bool PsuContol::isOutputEnabled(bool &outputEnabled)
{
    bool            result = false;
    unsigned char   status = 0;

    while(true)
    {
        if (!getRawStatus(status))
        {
            break;
        }

        outputEnabled = (status & 0x40) ? true : false;

        result = true;
        break;
    }

    return result;
}

bool PsuContol::getRawStatus(unsigned char &status)
{
    bool                result = false;
    QByteArray          byteCommand;
    QByteArray          byteStatus;

    while (true)
    {
        psuParams->getStatusCommand(byteCommand);

        if (!sendCommand(byteCommand))
        {
                break;
        }

        if (!receiveResponse(byteStatus))
        {
            break;
        }

        status = *byteStatus.constData();

        result = true;
        break;
    }

    return result;
}

bool PsuContol::waitForActualVoltage(double_t &voltage, int &timeoutMs, int &actualTimeMs)
{
    QTime       endTime;
    QTime       thisTime;

    bool        result = false;

    QTime       startTime = QTime::currentTime();
    int         interCommandTime = psuParams->getInterCommandTimeMs();
    double_t    nowVoltage = 0.0;

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

bool PsuContol::receiveResponse(QByteArray &response)
{
    bool    result = false;

    while (true)
    {
        if (!serialPort->waitForReadyRead())
        {
            break;
        }

        response = serialPort->readAll();

        result = true;
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
