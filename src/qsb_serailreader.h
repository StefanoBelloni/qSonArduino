#ifndef QSBSERAILREADER_H
#define QSBSERAILREADER_H

#include <QObject>
#include <QRunnable>
#include <QTimer>

#include <atomic>

#include "qsb_sonardata.h"

class QsbSerailReader
        : public QObject
{
    Q_OBJECT
public:
    explicit QsbSerailReader(QString nameDevice, int offset = 100);
    ~QsbSerailReader();

signals:
    virtual void newDataReady(const QsbSonarData &data);
    void reset();
    void connectionError();

public slots:
    virtual void readFromSerial();

protected:
    QTimer *timer = nullptr;
    int fileDescriptor = -1;
    QString deviceName;
    char bufferData[1024];
    int sizeBuffer = 0;
    double lastAngle = 0.0;
    bool withError = false;
    int numberInvalidReads = 0;
    int maxInvalidReads = 1000;

    /* === Comunication with Arduino === */
    void openPort(const char *device);
    bool readData(int timeout);
    const double factor = 3.141592654 / 180.0;
    void parseSerialData();

};

#endif // QSBSERAILREADER_H
