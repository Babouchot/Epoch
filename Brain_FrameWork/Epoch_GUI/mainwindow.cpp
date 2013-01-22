#include "mainwindow.h"

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
    fftPlot->xAxis->setRange(0, 64);

    fftPlot->replot();

    // Drawing custom frequency arousal graph area
    QCustomPlot* freqPlot = ui->FreqCustomPlot;
    freqPlot->addGraph();

    freqPlot->xAxis->setLabel("time (seconds)");
    freqPlot->yAxis->setLabel("Arousal amplitude");

    freqPlot->replot();

    QCustomPlot* rawPlot = ui->rawCustomPlot;
    rawPlot->addGraph();
    rawPlot->replot();
    rawPlot->xAxis->setLabel("time (seconds)");
    rawPlot->yAxis->setLabel("raw signal");

    QCustomPlot* abPlot = ui->abHistPlot;

    bars = new QCPBars(abPlot->xAxis, abPlot->yAxis);

    abPlot->addPlottable(bars);
    bars->setName("Alpha & Beta waves");

    abPlot->addGraph();
    abPlot->xAxis->setRange(0, 3);
    abPlot->xAxis->setLabel("Alpha                                   Beta");
    abPlot->yAxis->setRange(0, 300000);
    abPlot->yAxis->setLabel("Amplitude");
    abPlot->replot();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete bars;
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
    /*
    MainWindow* w = new MainWindow();
    daughters.push_back(w);
    w->show();
    */
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
    renderSpinners();
    if (acquisition) {
        if (reader.readNextFrequencies()) {
            //ui->log->append("frequencies acquisition successfull");
            updateGraphs();

            for (int i = 0; i < daughters.size(); ++i) {
                daughters[i]->updateGraphs();
            }
        }
    }
}

void MainWindow::renderSpinners() {
    ui->spinBoxStart->setMaximum(ui->spinBoxEnd->value()-1);
    ui->spinBoxEnd->setMinimum(ui->spinBoxStart->value()+1);
}

void MainWindow::on_StopAcqButton_clicked()
{
    if (acquisition) {
        reader.endReading();
        acquisition = false;
        reset(false);
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

        std::vector<double> amplitudes = reader.getFrequenciesRangedFromChannel(startFreq,
                                                                                endFreq,
                                                                                ui->channelComboBox->currentIndex());
        std::vector<double> fft = reader.getFrequenciesFromChannel(ui->channelComboBox->currentIndex());
        std::vector<double> rawData = reader.getRawDataFromChannel(ui->channelComboBox->currentIndex());
        //std::vector<double> counter = reader.getRawDataFromChannel(0);

        int newValue=0;

        for (int i = 0; i < amplitudes.size(); ++i) {
            newValue += amplitudes[i];
        }

        /*
        if (rawDataVector.size() > 5*128) {
            rawDataVector.erase(rawDataVector.begin(),rawDataVector.begin()+128);
            xRawDataVector.erase(xRawDataVector.begin(),xRawDataVector.begin()+128);
        }*/
        for (int i = 0; i < rawData.size(); ++i) {
            rawDataVector.push_back(rawData[i]);
            xRawDataVector.push_back(xRawDataVector.size());
        }

        xPositions.push_back(xPositions.size());
        yPositions.push_back(newValue);


        QVector<double> x(xPositions.size());
        QVector<double> y(yPositions.size());

        QVector<double> QRawDataVector(rawDataVector.size());
        QVector<double> QXRawDataVector(xRawDataVector.size());

        for (int i = 0; i < xPositions.size(); ++i) {
            x[i] = xPositions[i];
            y[i] = yPositions[i];
        }

        for (int i = 0; i < rawDataVector.size(); ++i) {
            QRawDataVector[i] = rawDataVector[i];
            QXRawDataVector[i] = xRawDataVector[i];
        }

        /*std::stringstream stream;
        stream << "x[" << x.size()-1 << "] = " << xPositions[xPositions.size()-1] << "; " << "y[" << y.size()-1 << "] = " << yPositions[yPositions.size()-1];
        stream << std::endl << newValue;
        char* ss = &stream.str()[0];
        ui->log->append(ss);
        */

        QVector<double> xFFT(fft.size()/2);
        QVector<double> yFFT(fft.size()/2);

        for (int i = 1; i < fft.size()/2; ++i) {
            xFFT[i-1] = i;
            yFFT[i-1] = fft[i];
        }

        QCustomPlot* freqPlot = ui->FreqCustomPlot;
        QCustomPlot* fftPlot = ui->FFTCustomPlot;
        QCustomPlot* rawPlot = ui->rawCustomPlot;

        freqPlot->graph(0)->setData(x,y);
        fftPlot->graph(0)->setData(xFFT, yFFT);
        rawPlot->graph(0)->setData(QXRawDataVector, QRawDataVector);

        freqPlot->rescaleAxes();
        fftPlot->rescaleAxes();
        rawPlot->rescaleAxes();

        freqPlot->replot();
        fftPlot->replot();
        rawPlot->replot();

        /*freqPlot->update();
        fftPlot->update();
        rawPlot->update();*/

        QCustomPlot* abPlot = ui->abHistPlot;

        QVector<double> keyBars;
        QVector<double> valueBars;

        keyBars << 1 << 2;

        std::vector<double> alphaWaves = reader.getAlphaWavesFromChannel(ui->channelComboBox->currentIndex());
        std::vector<double> betaWaves = reader.getBetaWavesFromChannel(ui->channelComboBox->currentIndex());
        double alphaValue, betaValue = 0;

        for (int i = 0; i < alphaWaves.size(); ++i) {
            alphaValue += alphaWaves[i];
        }

        for (int i = 0; i < betaWaves.size(); ++i) {
            betaValue += betaWaves[i];
        }

        valueBars << alphaValue << betaValue;

        bars->setData(keyBars, valueBars);

        //abPlot->rescaleAxes();
        abPlot->replot();

    }
    catch (ArousalReader::NoDataReadException e) {
        ui->log->append("No data read ...");
        std::cout << "No data read" << std::endl;
    }
    catch (ArousalReader::PacketLostException e) {
        reset(false);
        ui->log->append("Error : packet lost, reseting ...");
    }
}

void MainWindow::reset(bool modif) {
    if (modif)
        ui->log->append("reseting acquisition because of settings modification");
    xPositions.clear();
    yPositions.clear();
    rawDataVector.clear();
    xRawDataVector.clear();
}

void MainWindow::on_spinBoxStart_valueChanged(int arg1)
{
    reset(true);
}

void MainWindow::on_spinBoxEnd_valueChanged(int arg1)
{
    reset(true);
}

void MainWindow::on_channelComboBox_currentIndexChanged(int index)
{
    reset(true);
}

void MainWindow::on_clearLogButton_clicked()
{
    ui->log->clear();
}
