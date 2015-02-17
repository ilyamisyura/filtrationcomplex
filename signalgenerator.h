#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H
#include <QVector>
#include <QTime>
#include <random>
#include "filtercore.h"

class SignalGenerator
{
public:
    SignalGenerator();

    QVector <double> generateSimpleSignal(
            double num,
            double num_jumps,
            double alpha,
            double sigmaS,
            double sigmaN,
            FilterCore *core);

    QVector <double> generateSwitchingRegimeNoise(
            double t1,
            double t2,
            double discretizationStep,
            double sigma,
            double startingValue,
            double noiseA);

    QVector <double> generateSwitchingRegimeSignalPart(
           QVector <double> noise,
           double signalA,
           double discretizationStep,
           double startValue
           );

    QVector <double> generateSwitchingRegimeSignal(
            QVector <QVector<double> > tauAndSigmas,
            double noiseGeneratorParam,
            double signalGeneratorParam,
            double exitCondition,
            double discretizationStep,
            double startValueMu,
            double startValueSigma
            );
};

#endif // SIGNALGENERATOR_H
