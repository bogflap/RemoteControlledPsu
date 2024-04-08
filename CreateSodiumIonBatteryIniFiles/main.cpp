#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QFile   oFile;
    QString filePath("/home/pi/QtProjects/RemoteControlledPsu/CreateSodiumIonBatteryIniFiles/InitialisationFiles/SodiumIon_18650_1500mA.ini");

    QCoreApplication a(argc, argv);

    oFile.setFileName(filePath);
    oFile.open(QIODeviceBase::WriteOnly);

    QTextStream oTs(&oFile);

    oTs << ";"    <<  Qt::endl;
    oTs << "; Configuration file the Hakadi 18650 1500mA Sodium Ion battery" <<  Qt::endl;
    oTs << "; Spec says maximum charge voltage is 4.1V" << Qt::endl;
    oTs << "; maximum charge current is 0.5C" << Qt::endl;
    oTs << "; charge complete current = 0.05C" <<  Qt::endl;
    oTs << ";" <<  Qt::endl;
    oTs << Qt::endl;

    oTs << "[Timings]"<<   Qt::endl;
    oTs << "; Maximum charging period in units of milliseconds." << Qt::endl;
    oTs << "; If the charging time period is exceeded then the charging" << Qt::endl;
    oTs << "; cycle will be automatically terminated." <<  Qt::endl;
    oTs << "MaxChargePeriod=10000" <<  Qt::endl;
    oTs << "; Charging cycle monitoring period in units of milliseconds." << Qt::endl;
    oTs << "; This is the time period over which checks on the PSU actual" << Qt::endl;
    oTs << "; voltages and currents are obtained and the charging algorithm" << Qt::endl;
    oTs << "; applied. It is also the the time period when the main window" << Qt::endl;
    oTs << "; is updated with the actual values obtained." << Qt::endl;
    oTs << "UpdatePeriod=1000" << Qt::endl;
    oTs <<  Qt::endl;

    oTs << "[Voltages]" << Qt::endl;
    oTs << "; The maximum applied voltage in units of millivolts. This is" << Qt::endl;
    oTs << "; the maximum voltage that can be applied during the constant" << Qt::endl;
    oTs << "; current, constant voltage or initialisation modes of charging." << Qt::endl;
    oTs << "MaxAppliedVolts=4100" << Qt::endl;
    oTs << "; The minimum applied voltage in units of millivolts. This is" << Qt::endl;
    oTs << "; the minimum voltage that can be applied during the" << Qt::endl;
    oTs << "; constant current, constant voltage or initialisation modes of" << Qt::endl;
    oTs << "; charging." << Qt::endl;
    oTs << "MinAppliedVolts=1500" << Qt::endl;
    oTs <<  Qt::endl;

    oTs << "[Currents]" << Qt::endl;
    oTs << "; The maximum constant current in units of milliamps. This" << Qt::endl;
    oTs << "; is the maximum current allowed when charging in the constant" << Qt::endl;
    oTs << "; current mode." << Qt::endl;
    oTs << "MaxConstantCurrent=750" << Qt::endl;
    oTs << "; The maximum charge current in units of milliamps. This is" << Qt::endl;
    oTs << "; the maximum current allowed when charging. Reaching or" << Qt::endl;
    oTs << "; exceeding this value causes the charging cycle to terminate" << Qt::endl;
    oTs << "; with an error." << Qt::endl;
    oTs << "MaxChargeCurrent=1000" << Qt::endl;
    oTs << "; The completed current in units of milliamps. This is the" << Qt::endl;
    oTs << "; current when charging in the constant voltage mode and is" << Qt::endl;
    oTs << "; used to indicate when charging is complete." << Qt::endl;
    oTs << "CompletedCurrent=75" << Qt::endl;
    oTs <<  Qt::endl;

    oTs << "[Increments]"<<    Qt::endl;
    oTs << "Voltage increments in units of millivolts. This is the" << Qt::endl;
    oTs << "; voltage increment used when it is required to keep the" << Qt::endl;
    oTs << "; charging current within the limits as determined by the" << Qt::endl;
    oTs << "; maximum charge constant current and completed charge current" << Qt::endl;
    oTs << "; parameters." << Qt::endl;
    oTs << "VoltsIncrement=12"<< Qt::endl;

    oTs.flush();
    oFile.close();

    return 0;
}
