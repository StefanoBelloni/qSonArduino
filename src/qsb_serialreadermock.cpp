#include "qsb_serialreadermock.h"

#include <cmath>
#include <unistd.h>

QsbSerialReaderMock::QsbSerialReaderMock()
    :QsbSerailReader("")
{
    connect(timer, SIGNAL(timeout()),
            this, SLOT(readFromSerial()));
    timer->setSingleShot(true);
    timer->start(100);

}

void QsbSerialReaderMock::readFromSerial()
{
   double rads = 0.0;
   double distance = 70;
   double distance_start = 100;
   /* ======================================= */
   forever {
       usleep(10000);
       QsbSonarData data;
       if ( data.distance > 100 ) {
           data.distance = 170;
       }
       emit newDataReady(data);
       data.grads = rads;
       data.distance = distance;
       rads += 1 * factor;
       distance = distance_start + distance_start * sin(rads) / 2;
   }
}
