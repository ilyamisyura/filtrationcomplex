#ifndef DISCRETEPROCESSOR_H
#define DISCRETEPROCESSOR_H

#include <QObject>
#include "datasignal.h"
#include "filtercore.h"

class DiscreteProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DiscreteProcessor(QObject *parent = 0);

private:
//    QVector <double> getLambdaVector(int num, double noiseSigma, double signalSigma);
//    double Hm1(int i, int j, QVector <double> mu, double ** F);
//    QVector <double> Cm1Y(int n, double sigs, double alpha,QVector <double> Y, QVector <double> vecres);

signals:

public slots:
//    QVector <double> simpleFilter(QVector <double> signal, double signalSigma, double noiseSigma);

//    DataSignal* bayesFilter(DataSignal signal, double signalSigma, double noiseSigma, double alpha);

};

#endif // DISCRETEPROCESSOR_H
