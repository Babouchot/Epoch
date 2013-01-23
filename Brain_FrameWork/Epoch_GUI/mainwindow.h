#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "ArousalReader.h"
#include "fft.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void renderGraphs();
    ~MainWindow();
    
private slots:
    void on_actionQuit_triggered();

    void on_actionOpen_window_triggered();

    void on_startAcqButton_clicked();

    void on_StopAcqButton_clicked();

    void on_spinBoxStart_valueChanged(int arg1);

    void on_spinBoxEnd_valueChanged(int arg1);

    void on_channelComboBox_currentIndexChanged(int index);

    void on_clearLogButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    ArousalReader reader;
    std::vector<MainWindow*> daughters;

    // plot vectors
    std::vector<double> xPositions;
    std::vector<double> yPositions;
    std::vector<double> rawDataVector;
    std::vector<double> xRawDataVector;
    QCPBars* bars;

    void reset(bool);

    bool acquisition;
    void updateGraphs();
    void renderSpinners();
};

#endif // MAINWINDOW_H
