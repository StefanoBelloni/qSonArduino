#ifndef QSB_SERIALREADERMOCK_H
#define QSB_SERIALREADERMOCK_H

#include "qsb_serailreader.h"

class QsbSerialReaderMock : public QsbSerailReader
{
public:
    QsbSerialReaderMock();
public slots:
    void readFromSerial();
};

#endif // QSB_SERIALREADERMOCK_H
