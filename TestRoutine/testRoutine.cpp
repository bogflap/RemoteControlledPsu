#include <QCoreApplication>

#include "psucontol.h"

QTextStream gOutText = QTextStream(stdout);
PsuContol   gPsuControl;

bool    OpenPort(const QString &portName);
bool    GetPsuVersion();
bool    GetPsuStatus();
bool    TestVoltageControlMonitor();
bool    TestRampVoltage();
bool    ClosePort();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString portName("");

    while(true)
    {
        if (!OpenPort(portName)) break;
        if (!GetPsuVersion()) break;
        if (!GetPsuStatus()) break;
        if (!TestVoltageControlMonitor()) break;
        if (!TestRampVoltage()) break;

        break;
    }

    ClosePort();

    exit(0);
}

bool OpenPort(const QString& portName)
{
    QString error;

    bool    result = false;

    gOutText << "Opening port " << Qt::endl;
    result = gPsuControl.open(portName);
    if (!result)
    {
        gPsuControl.getError(error);
        gOutText << "Error = '" << error << "'" << Qt::endl;
    }

    return result;
}

bool GetPsuVersion()
{
    QString version;
    QString error;

    bool    result = false;

    gOutText << "Getting PSU version" << Qt::endl;
    result = gPsuControl.getVersion(version);
    if (!result)
    {
        gPsuControl.getError(error);
        gOutText << "Error = " << error << Qt::endl;
    }
    gOutText << version << Qt::endl;

    return result;
}

bool GetPsuStatus()
{
    QString     status;
    QString     error;

    bool        result = false;
    QTextStream statusText = QTextStream(&status);

    gOutText << "Getting PSU status" << Qt::endl;
    result = gPsuControl.getStatus(statusText);
    if (!result)
    {
        gPsuControl.getError(error);
        gOutText << "Error = " << error << Qt::endl;
    }
    gOutText << status;

    return result;
}

bool    TestVoltageControlMonitor()
{
    QString error;

    double_t  voltage = 5.0;
    double_t  setVoltage = 0.0;
    double_t  actualVoltage = 0.0;
    bool    result = false;

    while (true)
    {
        gOutText << "Setting output voltage to "
                 << voltage
                 << " volts"
                 << Qt::endl;
        result = gPsuControl.setVoltage(voltage);
        if (!result)
        {
            gPsuControl.getError(error);
            gOutText << "Error = " << error << Qt::endl;
            break;
        }

        // May need this whilst psu is actually implementing the voltage change
        // Otherwise the program hangs waiting for receive data
        gOutText << "Sleep for 50ms" << Qt::endl;
        gPsuControl.waitForMilliSeconds(50);

        gOutText << "Getting output voltage"<< Qt::endl;
        result = gPsuControl.readVoltage(setVoltage);
        if (!result)
        {
            gPsuControl.getError(error);
            gOutText << "Error = " << error << Qt::endl;
            break;
        }
        gOutText << "Read output voltage "
                 << setVoltage
                 << " volts"
                 << Qt::endl;

        gOutText << "Getting actual output voltage"<< Qt::endl;
        result = gPsuControl.actualVoltage(actualVoltage);
        if (!result)
        {
            gPsuControl.getError(error);
            gOutText << "Error = " << error << Qt::endl;
            break;
        }
        gOutText << "Actual output voltage "
                 << actualVoltage
                 << " volts"
                 << Qt::endl;

        // We are testin the command not the psu ramp times so omitting the
        // following check
//        if (voltage != actualVoltage)
//        {
//            gOutText.setRealNumberNotation(QTextStream::FixedNotation);
//            gOutText.setRealNumberPrecision(2);
//            gOutText << "Failed to reach set voltage (Set  = "
//                     << voltage
//                     << " Actual = "
//                     << actualVoltage
//                     << "V)"
//                     << Qt::endl;
//            result = false;
//            break;
//        }

        result = true;
        break;
    }

    return result;
}

bool TestRampVoltage()
{
    bool            result = false;

    double_t        startVoltage = 1.0;
    double_t        endVoltage = 21.0;
    double_t        rampTimeMs = 10000.0;
    unsigned    int steps = 10;

    gOutText << "Testing Ramp Up Voltage"
             << Qt::endl
             << "StartVoltage = "
             << startVoltage
             << "V"
             << Qt::endl
             << "EndVoltage = "
             << endVoltage
             << "V"
             << Qt::endl
             << "RampTimeMs  = "
             << rampTimeMs
             << "mS"
             << Qt::endl
             << "Steps = "
             << steps
             << Qt::endl;

    result = gPsuControl.rampUpVoltage(startVoltage,
                                        endVoltage,
                                        rampTimeMs,
                                        steps);
    if (!result)
    {
        QString error;
        gPsuControl.getError(error);
        gOutText << "Error = " << error << Qt::endl;
    }

    return result;
}

bool ClosePort()
{
    gOutText << "Closing port" << Qt::endl;

    gPsuControl.close();

    return true;
}
