#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H
#include <QVector>
#include <QTime>
#include <random>

class SignalGenerator
{
public:
    SignalGenerator();

    QVector <double> generateSwitchingRegimeNoise(
            double t1,
            double t2,
            double discretizationStep,
            double sigma,
            double startingValue,
            double noiseA);

    QVector <double> generateSwitchingRegimeSignal(QVector <double> noise,
                                                   double signalA,
                                                   double discretizationStep
                                                   );
};

#endif // SIGNALGENERATOR_H
