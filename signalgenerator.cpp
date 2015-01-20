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

QVector <double> SignalGenerator::generateSwitchingRegimeSignalPart(QVector <double> noise,
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
//                *discretizationStep
                + sqrt(discretizationStep)*distribution(generator);
        if (currentYValue == 0){
            qDebug("ZERO MAZAFAKA!!! At %d: noise value %f", k, noise.value(k));
        }
        signal.insert(k,currentYValue);
    }

    return signal;
}

QVector <double> SignalGenerator::generateSwitchingRegimeSignal(
        QVector <QVector<double> > tauAndSigmas,
        double noiseGeneratorParam,
        double signalGeneratorParam,
        double exitCondition,
        double discretizationStep
        )
{

    QVector <double> startVec;
    QVector <double> endVec;

    QVector <double> signalPart;
    QVector <double> res;

    QVector <double> noiseSignal;

    double tauStart;
    double sigmaStart;
    double tauEnd;
    double sigmaEnd;

    double lastSignalPartValue;

    int resI;
    double j; // for complete signal making

    int size;

    lastSignalPartValue = 0;

    size = tauAndSigmas.size();
    j = 0;

    for (int i=0; i<=size-1; i++){

        startVec = tauAndSigmas.value(i);
        tauStart = startVec.value(0);
        sigmaStart = startVec.value(1);

        endVec = tauAndSigmas.value(i+1);
        tauEnd = endVec.value(0);
        sigmaEnd = endVec.value(1);

        if (i == (tauAndSigmas.size()-1)){
            tauEnd = exitCondition;
        }

        noiseSignal = this->generateSwitchingRegimeNoise(tauStart,tauEnd,discretizationStep,sigmaStart,lastSignalPartValue,noiseGeneratorParam);
        signalPart = this->generateSwitchingRegimeSignalPart(noiseSignal, signalGeneratorParam, discretizationStep);

        lastSignalPartValue = signalPart.last();

        resI = 0;

        while ((j+discretizationStep < tauEnd)&&(j < exitCondition)) {
            res.insert(resI,signalPart.value(resI));
            resI++;
            j+= discretizationStep;
        }
    }

    return res;
}
