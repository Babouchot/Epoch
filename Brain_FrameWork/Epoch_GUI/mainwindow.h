#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "../Acquisition/ArousalReader.h"
#include "../Acquisition/fft.h"

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
    
    /**
     * reset acquisition
     */
    void reset(bool);



private slots:
    void on_actionQuit_triggered();

    void on_startAcqButton_clicked();

    void on_StopAcqButton_clicked();

    void on_spinBoxStart_valueChanged(int arg1);

    void on_spinBoxEnd_valueChanged(int arg1);

    void on_channelComboBox_currentIndexChanged(int index);

    void on_clearLogButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_addGameButton_clicked();

    void on_StartGameButton_clicked();

private:
    Ui::MainWindow *ui;

    ArousalReader reader; // reads data from the headset

    // plot vectors
    std::vector<double> xPositions; // Time axis for the acquisition
    std::vector<double> yPositions; // frequencies sum amplitude
    std::vector<double> rawDataVector; // raw data received amplitude
    std::vector<double> xRawDataVector; // Time axis for raw data (auto-scaling)

    //histogram bars
    QCPBars* bars;

    // boolean to know if an acquisition is currently running
    bool acquisition;

    //Games added in the framework
    std::vector<std::string> games;

    void updateGraphs();
    void renderSpinners();
    void saveAcquisition();
    void loadAcquisition();
};

#endif // MAINWINDOW_H
