#include <QCoreApplication>

#include "psucontrol.h"

// Assume Hakadi 18650 1500mA battery
// Max applied voltage 4.1V
// Min applied voltage 1.5V
// Max charge current 0.5C = 0.750A
// Charge complete current (Cahrge current / 10) = 0.0750A
// Voltage adjustment increment = 0.012V

// The following defines attempt to mimic what key/value pairs the
// configuration file will contain
#define MAX_CHARGE_PERIOD       0
#define UPDATE_PERIOD           1000
#define MAX_CONSTANT_CURRENT    0.750
#define COMPLETED_CURRENT       0.075
#define MAX_APPLIED_VOLTS       4.1
#define MIN_APPLIED_VOLTS       1.5
#define VOLTS_INCREMENT         0.012

// SodiumIonInitialisationTest has shown that 3.044V is a good start point
// using a 3.9ohm resistor as test load

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PsuContol   psuControl;

    qreal       voltage;
//    qreal       current;
    QString     error;
    bool        bResult;

    int         result = -1;

    while(true)
    {
        bool    outputEnable;
//        bool    outputEnabled;
//        bool    outputCurrentProtect;

        // Start with the initialisation suggested value
        voltage = 3.044;

        // Open the PSU (testing here so use hard coded file path)
        bResult = psuControl.open("/dev/ttyACM0");
        if (!bResult)
        {
            qDebug() << "Error opening serial port" << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        // Enter output voltage
        bResult = psuControl.setVoltage(voltage);
        if (!bResult)
        {
            qDebug() << error << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        // Enter loop with enabled output
        outputEnable = true;
        bResult = psuControl.setChannelOutput(outputEnable);
        if (!bResult)
        {
            qDebug() << "Error setChannelOutput" << Qt::endl;
            psuControl.getError(error);
            qDebug() << error << Qt::endl;
            break;
        }

        while (true)
        {

        }
    }

    return result;
}
