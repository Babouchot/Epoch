#include <QFileDialog>
#include <cstdlib>
#include "mainwindow.h"
#include "../Acquisition/ArousalReader.h"
#include "../Acquisition/fft.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    reader(new FFT()),
    yPositions(),
    xPositions()
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
    rawPlot->xAxis->setLabel("number of sample");
    rawPlot->yAxis->setLabel("raw signal");

    QCustomPlot* abPlot = ui->abHistPlot;

    bars = new QCPBars(abPlot->xAxis, abPlot->yAxis);

    abPlot->addPlottable(bars);
    bars->setName("Alpha & Beta waves");

    abPlot->addGraph();
    abPlot->xAxis->setRange(0, 3);
    abPlot->xAxis->setLabel("Alpha                                   Beta");
    abPlot->yAxis->setRange(0, 40000);
    abPlot->yAxis->setLabel("Amplitude");
    abPlot->replot();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete bars;
}

void MainWindow::on_actionQuit_triggered()
{
    //delete this;
    exit(0);
}


void MainWindow::on_startAcqButton_clicked()
{
    if (!acquisition) {
        reset(false);
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
            updateGraphs();
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
        ui->log->append("reading stopped");
        bool save = QMessageBox::information( this, "Save Acquisition ?",
                                              "Do you want to save this acquisition ?",
                                              "No", "Save Now",
                                              0, 1 );
        if (save) {
            saveAcquisition();
            ui->log->append("Acquisition saved");
        }
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

        int newValue=0;

        for (int i = 0; i < amplitudes.size(); ++i) {
            newValue += amplitudes[i];
        }

        //count the number of raw sample read
        static int rawCounter(0);

        
        if (rawDataVector.size() > 4*128) {
            rawDataVector.erase(rawDataVector.begin(),rawDataVector.begin()+128);
            xRawDataVector.erase(xRawDataVector.begin(),xRawDataVector.begin()+128);
        }

        for (int i = 0; i < rawData.size(); ++i) {
            rawDataVector.push_back(rawData[i]);
            xRawDataVector.push_back(rawCounter);
            ++rawCounter;
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
        reset(false);
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

void MainWindow::saveAcquisition() {
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Save acquisition as"),tr("../"));
    std::string file = saveFile.toStdString();
    if (file.find('.') > file.size()) {
        file += ".cvs";
    }
    reader.printArrayToFile(file, &yPositions[0], yPositions.size());
}

void MainWindow::loadAcquisition() {
    QString openFile = QFileDialog::getOpenFileName(this, tr("Open acquisition file"),tr("../"));
    std::string file = openFile.toStdString();
    try {
        xPositions.clear();
        yPositions.clear();

        yPositions = reader.getVectorFromFile(file);

        QVector<double> x(yPositions.size()-1);
        QVector<double> y(yPositions.size()-1);

        for (int i = 0; i < yPositions.size()-1; ++i) {
            xPositions.push_back(i);
            x[i] = xPositions[i];
            y[i] = yPositions[i];
        }

        ui->FreqCustomPlot->graph(0)->setData(x, y);
        ui->FreqCustomPlot->rescaleAxes();
        ui->FreqCustomPlot->replot();
    }
    catch (ArousalReader::WrongFileFormatException e) {
        QMessageBox::information( this, "Error",
                                  "Could not open load file (wrong file format or not a .cvs file)",
                                  "Ok", 0);
    }
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

void MainWindow::on_actionOpen_triggered()
{
    loadAcquisition();
}

void MainWindow::on_actionSave_triggered()
{
    saveAcquisition();
}

void MainWindow::on_addGameButton_clicked()
{
   QString openFile = QFileDialog::getOpenFileName(this, tr("Select your game"),tr("/"));
   std::string file = openFile.toStdString();
   if (file.size() > 0) {
       games.push_back(file);
       ui->gameComboBox->addItem(basename(file.c_str()));
   }
}

void MainWindow::on_StartGameButton_clicked()
{
    if (games.size() > 0) {
        std::cout<<games[ui->gameComboBox->currentIndex()].c_str()<<std::endl;

        std::string cmd,path;
        cmd+="(cd ";
        path=games[ui->gameComboBox->currentIndex()];
        int i=path.rfind("/");
        path=path.substr(0,i);
        std::cout<<path<<std::endl;
        cmd+=path;
        cmd+="; ";
        //std::system(cmd.c_str());
        cmd+=games[ui->gameComboBox->currentIndex()];
        cmd+=")";


        if (!fork()) 
        {
            std::system(cmd.c_str());
        }
    }
}
