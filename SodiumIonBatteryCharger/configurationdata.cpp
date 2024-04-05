#include "configurationdata.h"

ConfigurationData::ConfigurationData(QObject *parent)
    : QObject(parent)
    , settings(nullptr)
{

}

bool ConfigurationData::open(QString &filePath)
{
    bool    result = false;

    if (settings != nullptr)
    {
        delete settings;
    }

    settings = new QSettings(   filePath,
                                QSettings::IniFormat,
                                this);

    maxChargeTime = settings->value("Timings/MaxChargeTime").toInt();
    voltageIncrement = settings->value("Increments/VoltageIncrement").toInt();

    delete settings;
    settings = nullptr;
    result = true;

    return result;
}

int ConfigurationData::getMaxChargeTime() const
{
    return maxChargeTime;
}

int ConfigurationData::getVoltageIncrement() const
{
    return voltageIncrement;
}
