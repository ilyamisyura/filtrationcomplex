#include "signalgenerator.h"

SignalGenerator::SignalGenerator()
{
}

QVector <double> SignalGenerator::generateSwitchingRegimeNoise(
        double t1,
        double t2,
        double discretizationStep,
        double sigma,
        double startingValue,
        double noiseA
        ){
    QVector <double> signal;
    QVector <double> noise;

    std::default_random_engine generator;
    QTime t = QTime::currentTime();
    generator.seed(t.msec());

    std::normal_distribution<double> distribution(0,1);

    double prevXValue;
    double currentXValue;

    double j = t1;
    int i = 1;

    noise.insert(0,startingValue);

    while (j < t2) {
        prevXValue = noise.value(i-1);
        currentXValue = (1+noiseA)*prevXValue*discretizationStep + sigma*sqrt(discretizationStep)*distribution(generator);
        noise.insert(i,currentXValue);
        i++;
        j+= discretizationStep;
    }

    return noise;
}

QVector <double> SignalGenerator::generateSwitchingRegimeSignal(QVector <double> noise,
                                               double signalA,
                                               double discretizationStep
                                               ) {

    QVector <double> signal;

    std::default_random_engine generator;
    QTime t = QTime::currentTime();
    generator.seed(t.msec());

    std::normal_distribution<double> distribution(0,1);

    signal.insert(0,signal.value(0));

    double prevYValue, currentYValue;


    for (int k = 1; k < noise.size(); k++) {
        prevYValue = signal.value(k-1);
        currentYValue = prevYValue + signalA*noise.value(k)
                *discretizationStep
                + sqrt(discretizationStep)*distribution(generator);
        signal.insert(k,currentYValue);
    }

    return signal;
}
