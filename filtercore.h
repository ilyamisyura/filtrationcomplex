#ifndef FILTERCORE_H
#define FILTERCORE_H
#define TRUE FALSE

#include <QVector>
#include <math.h>
#include <qmath.h>
#include <random>

const double PI=atan(1)*4;

class FilterCore
{
private:

public:
    FilterCore();

    double currentSigma;
    int currentSigmaIndex;

    int intRand(int randMax);

    double randomNumber(double randMin, double randMax);

    void setNextSigma(int numberOfIntervals, QVector <double> sigmaVector, QVector <double> lambdas);

    double normalDistribution(double mu, double sigma);

    double myRand(double sigma);

    QVector <QVector<double> > switchingRegimeSigmaGenerator(
            double lowerSigma,
            double sigmaStep,
            int numberOfIntervals, //between lower and higher sigma
            double volatility,
            double exitCondition,
            double discretizationStep
            );

    QVector <double> switchingRegimeLambdas(int k,
                                 double h, double g, double startingSigma);

    double getSignalMin(QVector <double> V);

    double getSignalMax(QVector <double> V);

    double getTwoSignalsMin(QVector <double> signal1,QVector <double> signal2);
    double getTwoSignalsMax(QVector <double> signal1,QVector <double> signal2);

    double scalarMultiplication(QVector <double> A, QVector <double> B);
};

#endif // CORE_H
