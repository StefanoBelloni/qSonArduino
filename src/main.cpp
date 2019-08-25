#include <QApplication>

#include "qsb_sonar.h"
#include "qsb_sonardata.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /* === Remember: Register the custom class === */
    qRegisterMetaType<QsbSonarData>("RadarData");

    // QsbRadar w;
    MainWindow w;
    // MainWindow w(true);

    w.show();

    return a.exec();
}
