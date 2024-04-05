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

    oTs <<  ";"    <<  Qt::endl;
    oTs <<  "; Configuration file the Hakadi 18650 1500mA Sodium Ion battery"    <<  Qt::endl;
    oTs <<  ";"    <<  Qt::endl;
    oTs <<  Qt::endl;
    oTs <<  "[Timings]"<<   Qt::endl;
    oTs <<  "MaxChargeTime=1000"    <<  Qt::endl;
    oTs <<  Qt::endl;
    oTs <<  "[Increments]"<<    Qt::endl;
    oTs <<  "VoltageIncrement=12"<< Qt::endl;

    oTs.flush();
    oFile.close();

    return 0;
}
