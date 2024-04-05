#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "configurationdata.h"
#include "psuThread.h"

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

private slots:
    void    selectIniFileClicked(bool checked);
    void    openIniFileClicked(bool checked);
    void    openSerialPortClicked(bool checked);
    void    startClicked(bool checked);
    void    pauseClicked(bool checked);
    void    stopClicked(bool checked);
    void    quitClicked(bool checked);

private:
    void    makeConnections();

private:
    Ui::MainWindow *ui;
    ConfigurationData   confData;
    PsuThread           psuThread;
    QThread             tPsuThread;
};
#endif // MAINWINDOW_H
