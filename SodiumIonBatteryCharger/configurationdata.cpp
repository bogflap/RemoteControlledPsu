#include "configurationdata.h"

ConfigurationData::ConfigurationData(QObject *parent)
    : QObject(parent)
    , settings(nullptr)
    , valid(false)
{

}

bool ConfigurationData::open(QString &filePath)
{
    bool    result = true;

    if (settings != nullptr)
    {
        delete settings;
    }

    settings = new QSettings(   filePath,
                                QSettings::IniFormat,
                                this);

    valid = false;

    while (true)
    {
        QVariant    v;
        QString gk = "Identification/ConfigurationId";

        v = settings->value(gk);
        configurationId = v.toString();
        if (!v.isValid())
        {
            result = false;
            break;
        }
        if (!getIntValue("Timings", "MaxChargePeriod", maxChargePeriod))
        {
            result = false;
            break;
        }
        if (!getIntValue("Timings", "UpdatePeriod", updatePeriod))
        {
            result = false;
            break;
        }
        if (!getIntValue("Voltages", "MaxAppliedVolts", maxAppliedVolts))
        {
            result = false;
            break;
        }
        if (!getIntValue("Voltages", "MinAppliedVolts", minAppliedVolts))
        {
            result = false;
            break;
        }
        if (!getIntValue("Currents", "MaxConstantCurrent", maxConstantCurrent))
        {
            result = false;
            break;
        }
        if (!getIntValue("Currents", "MaxChargeCurrent", maxChargeCurrent))
        {
            result = false;
            break;
        }
        if (!getIntValue("Currents", "CompletedCurrent", completedCurrent))
        {
            result = false;
            break;
        }
        if (!getIntValue("Increments", "VoltsIncrement", voltsIncrement))
        {
            result = false;
            break;
        }

        valid = true;
        break;
    }

    delete settings;
    settings = nullptr;

    return result;
}

int ConfigurationData::getMaxChargePeriod() const
{
    return maxChargePeriod;
}

int ConfigurationData::getVoltsIncrement() const
{
    return voltsIncrement;
}

bool ConfigurationData::getIntValue(QString group, QString key, int &value)
{
    QVariant    v;
    bool        vOk;

    bool        result = true;

    while (true)
    {
        QString gk = group + "/" + key;

        v = settings->value(gk);
        if (!v.isValid())
        {
            result = false;
            break;
        }

        value = v.toInt(&vOk);
        if (!vOk)
        {
            result = false;
            break;
        }

        break;
    }

    return result;
}

QString ConfigurationData::getConfigurationId() const
{
    return configurationId;
}

bool ConfigurationData::isValid() const
{
    return valid;
}

int ConfigurationData::getCompletedCurrent() const
{
    return completedCurrent;
}

int ConfigurationData::getMaxChargeCurrent() const
{
    return maxChargeCurrent;
}

int ConfigurationData::getMinAppliedVolts() const
{
    return minAppliedVolts;
}

int ConfigurationData::getMaxAppliedVolts() const
{
    return maxAppliedVolts;
}

int ConfigurationData::getUpdatePeriod() const
{
    return updatePeriod;
}
