#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// PSU thread commands
signals:
    void    setOutputCurrent(double_t current);
    void    getOutputCurrent();
    void    setOutputVoltage(double_t voltage);
    void    getOutputVoltage();
    void    getActualOutputCurrent();
    void    getActualOutputVoltage();
    void    setOutputEnable(bool enable);
    void    getStatus();
    void    getIdentification();
    void    recallPanelSetting(int number);
    void    savePanelSetting(int number);
    void    setOverCurrentPrtotection(bool enable);
    void    setKeyboardLock(bool enable);

private:
    void    getSerialPorts();
    void    makeConnections();

// GUI slots
private slots:
    void    openClicked(bool checked);
    void    closeClicked(bool checked);
    void    quitClicked(bool checked);

    void    enableOutputClicked(bool checked);

    void    serialPortActivated(int index);

    void    constantCurrentToggled(bool checked);
    void    ampsTensUpClicked(bool checked);
    void    ampsTensDownClicked(bool checked);
    void    ampsOnesUpClicked(bool checked);
    void    ampsOnesDownClicked(bool checked);
    void    ampsTenthsUpClicked(bool checked);
    void    ampsTenthsDownClicked(bool checked);
    void    ampsHundrethsUpClicked(bool checked);
    void    ampsHundrethsDownClicked(bool checked);
    void    ampsThousandthsUpClicked(bool checked);
    void    ampsThousandthsDownClicked(bool checked);
    void    ampsApplyClicked(bool checked);
    void    ampsResetClicked(bool checked);

    void    constantVoltageToggled(bool checked);
    void    voltsApplyClicked(bool checked);
    void    voltsResetClicked(bool checked);
    void    voltsTensUpClicked(bool checked);
    void    voltsTensDownClicked(bool checked);
    void    voltsOnesUpClicked(bool checked);
    void    voltsOnesDownClicked(bool checked);
    void    voltsTenthsUpClicked(bool checked);
    void    voltsTenthsDownClicked(bool checked);
    void    voltsHundrethsUpClicked(bool checked);
    void    voltsHundrethsDownClicked(bool checked);
    void    voltsThousandthsUpClicked(bool checked);
    void    voltsThousandthsDownClicked(bool checked);

// Slots for PSU thread results
private slots:
    void    resultSetOutputCurrent(QString errorString);
    void    resultGetOutputCurrent(double_t current,QString errorString);
    void    resultSetOutputVoltage(QString errorString);
    void    resultGetOutputVoltage(double_t voltage, QString errorString);
    void    resultGetActualOutputCurrent(double_t current, QString errorString);
    void    resultGetActualOutputVoltage(double_t voltage, QString errorString);
    void    resultGetOutputEnable(QString errorString);
    void    resultGetStatus(QString status, QString errorString);
    void    resultGetIdentification(QString identification, QString errorString);
    void    resultRecallPanelSetting(int number, QString errorString);
    void    resultSavePanelSetting(QString errorString);
    void    resultSetOverCurrentPrtotection(QString errorString);
    void    resultSetKeyboardLock(QString errorString);

private:
    Ui::MainWindow *ui;
    QThread psuThread;
};
#endif // MAINWINDOW_H
