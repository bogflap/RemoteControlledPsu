#ifndef CONFIGURATIONDATA_H
#define CONFIGURATIONDATA_H

#include <QObject>
#include <QSettings>

class ConfigurationData : public QObject
{
public:
    ConfigurationData(QObject* parent);
    bool    open(QString& filePath);

    int getMaxChargePeriod() const;

    int getVoltsIncrement() const;

    int getUpdatePeriod() const;

    int getMaxAppliedVolts() const;

    int getMinAppliedVolts() const;

    int getMaxChargeCurrent() const;

    int getCompletedCurrent() const;

    bool isValid() const;

    QString getConfigurationId() const;

    int getMaxConstantCurrent() const;

private:
    bool        getIntValue(QString group, QString key, int& value);

private:
    QSettings   *settings;
    bool        valid;
    QString     configurationId;
    int         updatePeriod;
    int         maxChargePeriod;
    int         maxAppliedVolts;
    int         minAppliedVolts;
    int         maxConstantCurrent;
    int         maxChargeCurrent;
    int         completedCurrent;
    int         voltsIncrement;
};

#endif // CONFIGURATIONDATA_H
