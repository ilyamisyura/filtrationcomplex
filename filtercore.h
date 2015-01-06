#ifndef FILTERCORE_H
#define FILTERCORE_H

#define BOOST_UBLAS_MOVE_SEMANTICS

#include <mainwindow.h>
#include <QVector>
#include "math.h"
#include <qmath.h>
#include <random>

class filtercore
{
private:

public:
    filtercore();

    double currentSigma;
    int currentSigmaIndex;

    int intRand(int randMax);

    double randomNumber(double randMin, double randMax);

    void setNextSigma(int numberOfIntervals, QVector <double> sigmaVector, QVector <double> lambdas);

    double normalDistribution(double mu, double sigma);

    QVector <QVector<double> > switchingRegimeSigmaGenerator(
            double lowerSigma,
            double sigmaStep,
            int numberOfIntervals, //between lower and higher sigma
            double volatility,
            double exitCondition
            );

    QVector <double> switchingRegimeLambdas(int k,
                                 double h, double g, double startingSigma);

    double getSignalMin(QVector <double> V);

    double getSignalMax(QVector <double> V);
};

#endif // CORE_H
