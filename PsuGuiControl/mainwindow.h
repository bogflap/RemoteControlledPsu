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

private:
    void    getSerialPorts();
    void    makeConnections();

private slots:
    void    openClicked(bool checked);
    void    closeClicked(bool checked);

private:
    Ui::MainWindow *ui;
    QThread psuThread;
};
#endif // MAINWINDOW_H
