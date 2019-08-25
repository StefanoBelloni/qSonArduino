#ifndef QSBRADAR_H
#define QSBRADAR_H

#include <QMainWindow>
#include <QPainter>
#include <QVector>

#include <atomic>

#include "qsb_serailreader.h"
#include "qsb_sonardata.h"
#include "qsb_object.h"

class QsbSonar : public QWidget
{
    Q_OBJECT
public:
    explicit QsbSonar(QWidget *parent = 0, bool mock = false);

signals:

public slots:
    void forceUpdate(const QsbSonarData &data);
    void reset();
    void tryToReconnect();
    void paintEvent(QPaintEvent *event);

private slots:
    void doTryReconnect();

private:
   QsbSerailReader* getSerialReader(int offset = 100);
   /* === Thread === */
   void startThread(int offset = 100);
   void stopThread();

   void updateAngle();

   /* === Paint === */
   void paintRadarMonitor(QPainter &painter);
   void paintRadarBorder(QPainter &painter);
   void paintRadarCircles(QPainter &painter);
   void paintRadarCircle(QPainter &painter, int radius);
   void paintRadarLine(QPainter &painter, double angle);
   void paintObstacles(QPainter &painter);

   int __counter = 0;
    QThread *thread;

    /* === size window === */
    QRect getRadarArea();
    QPoint getRadarCenter();
    int margin = 10;
    double angle = 0.0;
    double lastAngle = 0.0;
    const double factor = 3.141592654 / 180.0;

    /* === */
    std::atomic<bool> connectionLost;
    int connectionAttempt = 0;

    /* === objects === */
    QList<QsbObject> obstacles;
    bool mock = false;

};

#endif // QSBRADAR_H
