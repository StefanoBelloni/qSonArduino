#ifndef QSBRADARDATA_H
#define QSBRADARDATA_H

struct QsbSonarData {
    double grads;
    int distance;
    bool reset;
};

Q_DECLARE_METATYPE(QsbSonarData);

#endif // QSBRADARDATA_H
