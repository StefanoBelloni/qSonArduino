#include "mainwindow.h"

#include <iostream>

#include <QDesktopWidget>
#include <QPainter>
#include <QTimer>
#include <QLinearGradient>
#include <QPushButton>
#include <QLayout>
#include <QVBoxLayout>


#include "qsb_sonar.h"

#include <math.h>

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

/* === = === = === = === = === = === = === = === = === = === = === = === */

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(bool mock) :
    QMainWindow(0)
{

    resize(
      QDesktopWidget()
               .availableGeometry(this)
               .size() * 0.7
   );
   // setCentralWidget(new QsbRadar(this, mock));
   setCentralWidget(new QsbSonar(this, mock));

}

/* === = === = === = === = === = === = === = === = === = === = === = === */

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
}
