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
    std::map<int, std::string> channelMap =  reader.getChannelMap();
    for (int i = 0; i<channelMap.size(); i++) {
        ui->channelComboBox->addItem(channelMap[i].c_str());
    }

    // Drawing FFT graph area
    QCustomPlot* fftPlot = ui->FFTCustomPlot;
    fftPlot->addGraph();

    fftPlot->xAxis->setLabel("Frequency");
    fftPlot->yAxis->setLabel("FFT value");

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
    try {
        std::vector<double> amplitudes = reader.getFrequenciesRangedFromChannel(startFreq,
                                                                            endFreq,
                                                                            ui->channelComboBox->currentIndex());
        QVector<double> x(endFreq-startFreq+1);
        QVector<double> y(amplitudes.size());
        int j = 0;
        for (int i = startFreq; i < endFreq; ++i) {
            x[j] = i;
            y[j++] = amplitudes[i];
        }
        QCustomPlot* plot = ui->FFTCustomPlot;
        plot->xAxis->setRange(startFreq,endFreq);
        plot->yAxis->setRange(-100, 100);
        plot->graph(0)->setData(x,y);
        plot->replot();

    }
    catch (ArousalReader::NoDataReadException e) {
        std::cout << "No data read" << std::endl;
    }
}
