#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , confData(parent)
{
    ui->setupUi(this);

    psuThread.moveToThread(&tPsuThread);
    tPsuThread.start();

    makeConnections();
}

MainWindow::~MainWindow()
{
    tPsuThread.quit();
    tPsuThread.wait();
    delete ui;
}

void MainWindow::selectIniFileClicked(bool checked)
{
    Q_UNUSED(checked);

    confFilePath = QFileDialog::getOpenFileName(this,
                                                "Open Initialisation File",
                                                "",
                                                "Ini File (*.ini)",
                                                nullptr,
                                                QFileDialog::ReadOnly);
    ui->iniFile->setText(confFilePath);
}

void MainWindow::openIniFileClicked(bool checked)
{
    Q_UNUSED(checked);

    if (!confData.open(confFilePath))
    {
        QString error("ERROR: Invalid Configuration File");
        ui->statusbar->showMessage(error);
    }
}

void MainWindow::openSerialPortClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::startClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::pauseClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::stopClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::quitClicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::makeConnections()
{
    connect(ui->selectIniFile, SIGNAL(clicked(bool)), this, SLOT(selectIniFileClicked(bool)));
    connect(ui->openIniFile, SIGNAL(clicked(bool)), this, SLOT(openIniFileClicked(bool)));
}
