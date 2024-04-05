#include <QCoreApplication>

#include "psucontrol.h"

// Assume Hakadi 18650 1500mA battery
// Max applied voltage 4.1V
// Min applied voltage 1.5V
// Max charge current = 0.750A
// Voltage adjustment increment = 12mV

// Seems to need a delay between commands otherwise it does not work properly
// At present the following WAIT_TIME seems to work
#define DELAY_TIME_MS 50

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PsuContol   psuControl;

    qreal       voltage;
    qreal       current;
    QString     error;
    bool        bResult;

    int         result = -1;

    while(true)
    {
        bool    outputEnable;
        bool    outputEnabled;
        bool    outputCurrentProtect;

        // Start with maximum allowable
        voltage = 4.1;

        // Open the PSU (testing here so use hard coded file path)
        bResult = psuControl.open("/dev/ttyACM0");
        if (!bResult)
        {
            qDebug() << "Error opening serial port" << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        // Enter loop with disabled output
        outputEnable = false;
        bResult = psuControl.setChannelOutput(outputEnable);
        if (!bResult)
        {
            qDebug() << "Error setChannelOutput" << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        psuControl.waitForMilliSeconds(DELAY_TIME_MS);

        // Enter loop with current limit
        current = 0.750;
        bResult = psuControl.setCurrent(current);
        if (!bResult)
        {
            qDebug() << "Error setCurrent" << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        psuControl.waitForMilliSeconds(DELAY_TIME_MS);

        // Enter loop with enabled output current protect
        outputCurrentProtect = true;
        bResult = psuControl.setOverCurrentProtection(outputCurrentProtect);
        if (!bResult)
        {
            qDebug() << "Error setOverCurrentProtection" << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        psuControl.waitForMilliSeconds(DELAY_TIME_MS);

        while(true)
        {
            // Enter output voltage
            bResult = psuControl.setVoltage(voltage);
            if (!bResult)
            {
                qDebug() << error << Qt::endl;
                psuControl.getError(error);
                qDebug() << error << Qt::endl;
                break;
            }

            psuControl.waitForMilliSeconds(DELAY_TIME_MS);

            // Enable output
            outputEnable = true;
            bResult = psuControl.setChannelOutput(outputEnable);
            if (!bResult)
            {
                qDebug() << "Error setChannelOutput" << Qt::endl;
                psuControl.getError(error);
                qDebug() << error << Qt::endl;
                break;
            }

            psuControl.waitForMilliSeconds(DELAY_TIME_MS);

            // Check if output disabled (i.e. output current protect triggered)
            // If not then get out of loop error free
            bResult = psuControl.isOutputEnabled(outputEnabled);
            if (!bResult)
            {
                qDebug() << "Error isOutputEnabled" << Qt::endl;
                psuControl.getError(error);
                qDebug() << error << Qt::endl;
                break;
            }
            if (outputEnabled)
            {
                qDebug() << "SUCCESS at voltage = " << voltage << Qt::endl;
                result = 0;
                break;
            }

            psuControl.waitForMilliSeconds(DELAY_TIME_MS);

            // Disable output
            outputEnable = false;
            bResult = psuControl.setChannelOutput(outputEnable);
            if (!bResult)
            {
                qDebug() << "Error setChannelOutput" << Qt::endl;
                psuControl.getError(error);
                qDebug() << error << Qt::endl;
                break;
            }

            psuControl.waitForMilliSeconds(DELAY_TIME_MS);

            // Decrement output voltage
            voltage -= 0.012;

            // If output voltage less than minimum allowable then exit loop with error
            if (voltage < 1.5)
            {
                qDebug() << "ERROR Less than minimum voltage reached = " << voltage << Qt::endl;
                break;
            }

            // Loop to try again
        }

        // Disable output current protect
        outputCurrentProtect = false;
        bResult = psuControl.setOverCurrentProtection(outputCurrentProtect);
        if (!bResult)
        {
            qDebug() << "Error setOverCurrentProtection" << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        // Disable output
        outputEnable = false;
        bResult = psuControl.setChannelOutput(outputEnable);
        if (!bResult)
        {
            qDebug() << "Error setChannelOutput" << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        bResult = psuControl.close();
        if (!bResult)
        {
            qDebug() << "Error setChannelOutput" << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        break;
    }

    return result;
}
