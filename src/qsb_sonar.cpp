#include <QThread>
#include <QPainter>
#include <QPainterPath>
#include <QDesktopWidget>

#include "qsb_sonar.h"
#include "qsb_serailreader.h"
#include "qsb_serialreadermock.h"

#include <cmath>

QsbSonar::QsbSonar(QWidget *parent, bool mock)
    : QWidget(parent),
      connectionLost(false),
      mock(mock)
{
   startThread();
}

/* === = === = === = === = === = === = === = === = === = === = === = === */

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::startThread(int offset)
{

    thread = new QThread();
    QsbSerailReader * serialReader = getSerialReader(offset);
    QObject::connect(serialReader, SIGNAL(newDataReady(QsbSonarData)),
                     this, SLOT(forceUpdate(QsbSonarData)));
    QObject::connect(serialReader, SIGNAL(reset()),
                     this, SLOT(reset()));
    QObject::connect(serialReader, SIGNAL(connectionError()),
                     this, SLOT(tryToReconnect()));
    serialReader->moveToThread(thread);
    thread->start();

}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::stopThread()
{
   thread->requestInterruption();
   thread->quit();
   thread->wait();
}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::updateAngle()
{
    angle += 4.0 * 3.141592654 / 180;
    if ( angle > 2 * 3.141592654 ) { angle = 0.0; }
}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::paintRadarMonitor(QPainter &painter)
{
    QRect monitor = getRadarArea();
    painter.fillRect(monitor, Qt::black);
    painter.setPen(QPen(Qt::yellow, 10));
    painter.drawRect(monitor);

}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::paintRadarBorder(QPainter &painter)
{
    QRect monitor = getRadarArea();
    painter.setPen(QPen(Qt::yellow, 10));
    painter.drawRect(monitor);

}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::paintRadarAngleLines(QPainter &painter)
{
   const int minSize = sqrt(2) * qMax(this->width(), this->height()) / 2;
   int sizeLine = minSize;
   painter.save();
   painter.setPen(QPen(Qt::green));
   for ( int i = 0; i < 360; i += angleStep ) {
       painter.rotate(i);
       painter.drawLine(
          QPoint(0, 0),
          QPoint(sizeLine, sizeLine)
        );
   }
   painter.restore();
}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::paintRadarText(QPainter &painter)
{
   const int minSize = sqrt(2) * qMax(this->width(), this->height()) / 2;
   const int posText = sqrt(2) * qMax(this->width(), this->height()) / 2;
   painter.save();
   painter.setPen(QPen(Qt::yellow));
   int n = 0;
   for ( int i = 0; i < 360; i += angleStep ) {
       painter.drawText(
          QRect(
              (posText / 2),
              0,
              30, 30),
          QString::number(i) + "°"
       );
       if ( (n % 3) == 0 ) {
          for ( int j = distanceStep; j < minSize; j += distanceStep) {
             painter.drawText(
                QRect( j, -15, 30, 30),
                QString::number(j / distanceFactor)
             );
          }
       }
       n++;
       painter.rotate(angleStep);
   }
   painter.restore();
}


/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::paintRadarCircles(QPainter &painter)
{
   // painter.save();
   painter.setPen(QPen(Qt::green, 4));
   for ( int i = 0; i < this->width(); i += distanceStep ) {
       paintRadarCircle(painter, i);
   }
   // painter.restore();
}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::paintRadarCircle(QPainter &painter, int radius)
{
    painter.drawEllipse(QPoint(0, 0), radius, radius);
}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::paintRadarLine(QPainter &painter, double angle)
{

    // painter.save();
    int n_lines = 20;
    int n_step = 2;
    double theta = angle - n_lines * factor;
    int i = 0;
    int maxSize = qMax(this->width(), this->height()) / 4;
    while( theta < angle + 0 * factor) {

       QColor color = QColor(
                   255, 255 - i * 255.0 / n_lines / 2,
                   i * 255.0 / n_lines / 2);
       painter.setPen(QPen(color, 1));
       painter.drawLine(
           QPoint(0,0),
           QPoint( maxSize * cos(theta), maxSize * sin(theta))
        );
        theta += factor / n_step;
        i++;
    }

    painter.setPen(QPen(Qt::red, 3));
    painter.drawLine(
          QPoint(0,0),
          QPoint(maxSize * cos(angle), maxSize * sin(angle))
    );
   // painter.restore();

//   QPainterPath path;
//   path.moveTo(this->getRadarCenter());
//   path.lineTo(
//       this->getRadarCenter()
//                + QPoint(
//                    this->width() * cos(angle),
//                    this->height()/2  + this->width() * sin(angle))
//               );
//   path.arcTo(getRadarArea(), angle, angle - 10 * factor);
//   path.lineTo(this->getRadarCenter());
//   painter.drawPath(path);

}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::paintObstacles(QPainter &painter)
{
    // painter.save();
    QMutableListIterator<QsbObject> it(obstacles);
    while( it.hasNext() ) {
        if ( it.next().countPaint <= 0 ) {
            it.remove();
        } else {
            it.value().paint(painter);
        }
    }
   // painter.restore();

}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::forceUpdate(const QsbSonarData &data)
{

   connectionAttempt = 0;
   angle = data.grads;
   __counter = 5;

   // Visualize objects under 100 cm
   if ( data.distance < maxSonarDisplay ) {

      // map the distance to a smaller area.
      int distance = data.distance * distanceFactor;
      // position with ref to the center
      QsbObject obstacle(
            distance,
            QPoint( distance * cos(angle), distance * sin(angle) ),
            angle
            );
      obstacles.append(obstacle);

   }

   if ( std::abs(lastAngle - angle) > 0.0 ) {
      lastAngle = angle;
      update();
   }

}

void QsbSonar::reset()
{
   obstacles.clear();

}

void QsbSonar::tryToReconnect()
{
   connectionLost = true;
   update();
}

/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::doTryReconnect() {
   connectionLost = false;
   connectionAttempt++;
   stopThread();
   startThread(1000);
   return;
}

QsbSerailReader *QsbSonar::getSerialReader(int offset)
{
    if ( mock ) {
        return new QsbSerialReaderMock();
    } else {
        return new QsbSerailReader("/dev/ttyACM0", offset);
    }
}


/* === = === = === = === = === = === = === = === = === = === = === = === */

void QsbSonar::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    distanceFactor = std::min(this->width(), this->height()) / maxSonar / 2;
    distanceStep = maxSonar * distanceFactor / 7;

    if ( connectionLost ) {
        QTimer *timer = new QTimer();
        timer->setSingleShot(true);
        timer->start(2000);
        connect(timer, SIGNAL(timeout()), this, SLOT(doTryReconnect()));
        // paintRadarMonitor(painter);
        // paintRadarCircles(painter);
        painter.setFont(QFont("times",23));
        // QLinearGradient gradient(getRadarArea().topLeft(), getRadarArea().bottomRight());
        QRadialGradient gradient(getRadarCenter(), this->width());
        gradient.setColorAt(0, Qt::black);
        gradient.setColorAt(1, Qt::white);
        painter.fillRect(getRadarArea(), gradient);
        if ( ( connectionAttempt % 2) == 0 ) {
            painter.setPen(QPen(Qt::red));
        } else {
            painter.setPen(QPen(Qt::white));
        }
        QString text("Try to reconnect ... ");
        text += QString::number(connectionAttempt);
        painter.drawText(
                    getRadarCenter() - 1 * QPoint(this->width() / 8, 0) ,
                    text);

        if ( connectionAttempt == 11 ) { exit(EXIT_FAILURE); }

        return;
    }

    paintRadarMonitor(painter);
    // with ref to the center
    painter.translate(getRadarCenter());
    paintRadarAngleLines(painter);
    paintRadarCircles(painter);
    paintRadarLine(painter, angle);
    paintRadarText(painter);
    paintObstacles(painter);
    painter.translate(-getRadarCenter());
    paintRadarBorder(painter);
    // this->updateAngle();

}

/* === = === = === = === = === = === = === = === = === = === = === = === */

QRect QsbSonar::getRadarArea()
{
    return QRect(
                0,
                this->height(),
                this->width(),
                - this->height() );
}

/* === = === = === = === = === = === = === = === = === = === = === = === */

QPoint QsbSonar::getRadarCenter()
{
   return QPoint(this->width() / 2, this->height() / 2);
}
