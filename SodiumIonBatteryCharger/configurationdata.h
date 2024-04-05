#ifndef CONFIGURATIONDATA_H
#define CONFIGURATIONDATA_H

#include <QObject>
#include <QSettings>

class ConfigurationData : public QObject
{
public:
    ConfigurationData(QObject* parent);
    bool    open(QString& filePath);

    int getMaxChargeTime() const;

    int getVoltageIncrement() const;

private:
    QSettings   *settings;
    int         maxChargeTime;
    int         voltageIncrement;
};

#endif // CONFIGURATIONDATA_H
