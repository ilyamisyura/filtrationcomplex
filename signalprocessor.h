#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H

#include <math.h>
#include <QVector>

class SignalProcessor
{
private:

    //parameters for H generation (1.33)
    double hStart; //tau(i-1) from (1.32)
    double hSigma; //for quadratic equation (b)
    double hNoiseParam; //a
    double hSignalParam; //A

    //parameters for F generation (1.33)
    double fStart; //tau(i-1) from (1.32)
    double fSigma; //for quadratic equation (b)
    double fNoiseParam; //a
    double fSignalParam; //A

public:
    SignalProcessor();

    void setFunctionHParameters(
        double start,
        double sigma,
        double noiseParam,
        double signalParam);

    void setFunctionFParameters(
        double start,
        double sigma,
        double noiseParam,
        double signalParam);

    double switchingRegimeFunctionH(double time);
    double switchingRegimeFunctionF(double time);

    double generateGammaValue(double t,double start,double sigma,double A,double a);

    QVector <double> solveQuadraticEquation(double a,double b, double c);

    QVector <double> switchingRegimeFilter(
                QVector <double> signal,
                double start,
                double sigma,
                double discretizationStep,
                double signalGenerationParam, //A
                double noiseGenerationParam //a
            );

    double countIntegralH(
                double lower,
                double higher,
                double integrationStep
            );
};

#endif // SIGNALPROCESSOR_H
