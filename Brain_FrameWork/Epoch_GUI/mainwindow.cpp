#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ArousalReader.h"
#include "fft.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    exit(0);
}

void MainWindow::on_actionOpen_window_triggered()
{

}

void MainWindow::on_startAcqButton_clicked()
{
    int startFreq = ui->spinBoxStart->value();
    int endFreq = ui->spinBoxEnd->value();
    FFT fft;
    ArousalReader reader(&fft);
    double amplitudes[128];
}
