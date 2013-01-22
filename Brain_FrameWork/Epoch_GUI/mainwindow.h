#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;
    ArousalReader reader;
    std::vector<MainWindow*> daughters;
    bool acquisition;

    void updateGraphs();
};

#endif // MAINWINDOW_H
