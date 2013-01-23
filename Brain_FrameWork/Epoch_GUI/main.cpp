#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    
    //return a.exec();
    while (true) {
        try {
            w.renderGraphs();
        }
        catch (ArousalReader::PacketLostException e) {
            std::cout << "Packet lost during acquisition" << std::endl;
        }
        catch (ArousalReader::NoDataReadException e) {
            std::cout << "No data read" << std::endl;
        }

        a.processEvents();
        usleep(25000);
    }
}
