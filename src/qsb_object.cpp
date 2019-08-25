#include "qsb_object.h"
#include <cmath>

QsbObject::QsbObject(int distance, QPoint position, double angle)
    : position(position),
      angle(angle),
      distance(distance),
      countPaint(maxPaints)
{
    // creationTime.start();
}

QsbObject::QsbObject(const QsbObject& object)
{
   this->countPaint = maxPaints;
   this->angle = object.angle;
   this->distance = object.distance;
   this->position = object.position;

}

void QsbObject::paint(QPainter &painter)
{
   painter.setPen(QPen(Qt::red, 2));
   // int passed = creationTime.elapsed() / 100;

   for ( int i = 0; i <= std::min(countPaint, 5); i += 1 ) {
      painter.drawEllipse(this->position, i, i );
   }

   // QBrush brush = painter.brush();
   // painter.setBrush(Qt::red);
   // painter.drawEllipse( this->position, 2, 2 );
   /*QLineF line;
   line.setP1(position);
   line.setAngle(angle / factor);
   line.setLength(2 * distance);
   painter.drawLine(line);
   */
   countPaint--;
   // painter.setBrush(brush);
}
