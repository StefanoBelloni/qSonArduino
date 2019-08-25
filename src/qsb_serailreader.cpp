#include "qsb_serailreader.h"
#include <unistd.h>

#include <QThread>

/* === header to open the file descriptor === */
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include <cmath>

QsbSerailReader::QsbSerailReader(QString name, int offset)
    : QObject(0),
      deviceName(name),
      timer(new QTimer(this))
{

    connect(timer, SIGNAL(timeout()),
            this, SLOT(readFromSerial()));
    timer->setSingleShot(true);
    timer->start(offset);

}

QsbSerailReader::~QsbSerailReader()
{
   if ( fileDescriptor != -1 ) {
      close(fileDescriptor);
   }
}

void QsbSerailReader::readFromSerial()
{
   if ( fileDescriptor == -1 ) {
       openPort(deviceName.toLocal8Bit().constData());
   }

   if ( ! withError ) {
       lseek(fileDescriptor, 0, SEEK_END);
   }

   forever {
      if ( QThread::currentThread()->isInterruptionRequested() ) {
          break;
      }
      if ( readData(1000000) ) {

          if ( bufferData[0] == 'C' ) {
             // TODO:
             for ( int j = 0; j < sizeBuffer; ++j ) {
                bufferData[j] = '\0';
             }

           } else if ( bufferData[0] == 'R' ) {
             // TODO:
             for ( int j = 0; j < sizeBuffer; ++j ) {
                bufferData[j] = '\0';
             }
             emit reset();
          } else {
              parseSerialData();
          }
      }
      if ( numberInvalidReads > maxInvalidReads ) {
          emit connectionError();
          break;
      }

   }
   close(fileDescriptor);
   delete timer;
}

/* ========================================================================== */

void QsbSerailReader::openPort(const char *device)
{
  int rc = -1;
  char readBuf[16 + 1];
  fileDescriptor = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fileDescriptor == -1) {
    perror("open_port: Unable to open device.");
    withError = true;
    emit connectionError();
    return;
  }
  rc = read(fileDescriptor, readBuf, 16);
  if (fileDescriptor == -1 || rc <= 0) {
    perror("read from divice: Unable to read");
    withError = true;
    emit connectionError();
    return;
  }

}

/* ========================================================================== */

bool QsbSerailReader::readData(int timeout)
{

    if ( withError ) {
        // fprintf(stdout, "::E::\n");
        return false;
    }

    int i = 0, ret;
    char c = '\0';
    while ( true ) {
       if ( timeout != -1 && i > timeout ) {
           return false;
       }
       // usleep(1000);
       ret = read( fileDescriptor, &c, 1);
       if ( ret > 0 ) {
           i = 0;
           numberInvalidReads = 0;
           bufferData[sizeBuffer] = c;
           sizeBuffer++;
           // fprintf(stdout, "%c", c);
       }

       if ( ret < 0 ) {
           // perror("Error reading file descriptor.");
           numberInvalidReads++;
           // printf("Invalids reading %d\n", numberInvalidReads);
           // close(fileDescriptor);
           // exit(EXIT_FAILURE);
       }
       if ( c == '\n' ) {
           // fprintf(stdout, "%s\n", bufferData);
           return true;
       }

       i++;

    }

    return false;

}

void QsbSerailReader::parseSerialData()
{

   bufferData[sizeBuffer] = '\0';
   // fprintf(stdout, "B: %s", bufferData);

   for ( int i = 0; i < sizeBuffer; ++i) {
      if ( bufferData[i] == ',' ) {
          QsbSonarData data;
          bufferData[i] = '\0';
          data.distance = atoi(bufferData);
          data.grads = atof(bufferData + i + 1) * factor;
          data.reset = false;
          if ( data.distance != 0 && ( data.grads >= - 2 * 3.142  || data.grads <= 2 * 3.142) ) {
             if ( std::abs( lastAngle - data.grads ) > 0.0 ||
                data.distance < 100) {
                // fprintf(stdout, "P: %d;%f\n", data.distance, data.grads);
                lastAngle = data.grads;
                emit newDataReady(data);
             }
          } else {
              // fprintf(stdout, "E: out of range %d;%f\n", data.distance, data.grads);
              for ( int j = 0; j < sizeBuffer; ++j ) {
                  bufferData[j] = '\0';
              }
          }
          sizeBuffer = 0;
      }
   }

}

