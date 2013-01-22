#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ArousalReader.h"
#include "fft.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    reader(new FFT())
{
    ui->setupUi(this);
    acquisition = false;

    // Channel comboBox initialize
    std::map<int, std::string> channelMap =  reader.getChannelMap();
    for (int i = 0; i<channelMap.size(); i++) {
        ui->channelComboBox->addItem(channelMap[i].c_str());
    }

    // Drawing FFT graph area
    QCustomPlot* fftPlot = ui->FFTCustomPlot;
    fftPlot->addGraph();

    fftPlot->xAxis->setLabel("Frequency");
    fftPlot->yAxis->setLabel("FFT value");
    fftPlot->xAxis->setRange(0, 128);
    fftPlot->yAxis->setRange(0,10);

    fftPlot->replot();

    // Drawing custom frequency arousal graph area
    QCustomPlot* freqPlot = ui->FreqCustomPlot;
    freqPlot->addGraph();

    freqPlot->xAxis->setLabel("Time (seconds)");
    freqPlot->yAxis->setLabel("Arousal amplitude");

    freqPlot->replot();

}

MainWindow::~MainWindow()
{
    delete ui;
    for (int i = 0; i <= daughters.size(); ++i) {
        delete daughters[i];
    }
}

void MainWindow::on_actionQuit_triggered()
{
    delete this;
    exit(0);
}

void MainWindow::on_actionOpen_window_triggered()
{
    MainWindow* w = new MainWindow();
    daughters.push_back(w);
    w->show();
}

void MainWindow::on_startAcqButton_clicked()
{
    if (!acquisition) {
        reader.initialiseReading();
        acquisition = true;
        ui->log->append("Emotiv Engine start up sucssessful \nreading initialise...");
    }
    else
        ui->log->append("Already running, stop process before");
}

void MainWindow::renderGraphs() {
    if (acquisition) {
        if (reader.readNextFrequencies()) {
            ui->log->append("frequencies acquisition successfull");
            updateGraphs();
        }
    }
}

void MainWindow::on_StopAcqButton_clicked()
{
    if (acquisition) {
        reader.endReading();
        acquisition = false;
        ui->log->append("reading stopped");
    }
    else {
        ui->log->append("No process running");
    }
}


/**
 * Draws the 2 graphs of the framework
 * (called in the render method)
 */
void MainWindow::updateGraphs() {

    int startFreq = ui->spinBoxStart->value();
    int endFreq = ui->spinBoxEnd->value();

    try {
        std::cout << "Dans le try" << std::endl;
        std::vector<double> amplitudes = reader.getFrequenciesRangedFromChannel(startFreq,
                                                                                endFreq,
                                                                                ui->channelComboBox->currentIndex());
        QVector<double> x(endFreq-startFreq+1);
        QVector<double> y(amplitudes.size());
        int j = 0;
        for (int i = startFreq; i < endFreq; ++i) {
            x[j] = i;
            y[j++] = amplitudes[i];
            std::stringstream stream;
            //stream << "x[" << j << "] = " << i << "; " << "y[" << j << "] = " << amplitudes[i];
            char* ss = &stream.str()[0];
            ui->log->append(ss);
        }
        QCustomPlot* freqPlot = ui->FreqCustomPlot;
        freqPlot->graph(0)->setData(x,y);
        freqPlot->rescaleAxes();
        freqPlot->replot();
        freqPlot->update();


    }
    catch (ArousalReader::NoDataReadException e) {
        ui->log->append("No data read ...");
        std::cout << "No data read" << std::endl;
    }

}
