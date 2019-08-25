#ifndef QSB_OBJECT_H
#define QSB_OBJECT_H

#include <QPainter>
#include <QTime>

class QsbObject
{
public:
    QsbObject(int distance, QPoint position, double angle);
    QsbObject(const QsbObject &object);

    void paint(QPainter &painter);
    int countPaint = maxPaints;

protected:
    QPoint position;
    double angle;
    int distance;
    int maxPaints = 50;
    double factor = 3.141592654 / 180.0;
    // QTime  creationTime;

};

#endif // QSB_OBJECT_H
