#include "signalprocessor.h"

SignalProcessor::SignalProcessor()
{
    //parameters for H generation (1.33)
    hStart = 0; //tau(i-1) from (1.32)
    hSigma = 0; //for quadratic equation (b)
    hNoiseParam = 0; //a
    hSignalParam = 0; //A

    //parameters for F generation (1.33)
    fStart = 0; //tau(i-1) from (1.32)
    fSigma = 0; //for quadratic equation (b)
    fNoiseParam = 0; //a
    fSignalParam = 0; //A
}

void SignalProcessor::setFunctionHParameters(
    double start,
    double sigma,
    double noiseParam,
    double signalParam)
{
    hStart = start;
    hSigma = sigma;
    hNoiseParam = noiseParam;
    hSignalParam = signalParam;
}

void SignalProcessor::setFunctionFParameters(
    double start,
    double sigma,
    double noiseParam,
    double signalParam)
{
    fStart = start;
    fSigma = sigma;
    fNoiseParam = noiseParam;
    fSignalParam = signalParam;
}

double SignalProcessor::switchingRegimeFunctionH(double time)
{
    double gamma = generateGammaValue(time,hStart,hSigma,hSignalParam,hNoiseParam);
    return hNoiseParam - pow(hSignalParam,2)*gamma;
}

double SignalProcessor::switchingRegimeFunctionF(double time){
    return fSignalParam*generateGammaValue(time,fStart,fSigma,fSignalParam,fNoiseParam);
}

QVector <double> SignalProcessor::solveQuadraticEquation(double a,double b, double c) {

    double disc;
    double x1;
    double x2;

    QVector <double> res;

    disc = b*b - 4*a*c;
    x1 = (-b - sqrt(disc))/(2*a);
    x2 = (-b + sqrt(disc))/(2*a);


    res.insert(0,x1);
    res.insert(1,x2);

    return res;
}

double SignalProcessor::generateGammaValue(
                        double t,
                        double start,
                        double sigma,
                        double A,
                        double a)
{
    QVector <double> quadraticSolution;
    double x1;
    double x2;
    double e;
    double resValue;

    quadraticSolution = solveQuadraticEquation(A*A, -2*a, -sigma*sigma);

    x1 = quadraticSolution.value(0);
    x2 = quadraticSolution.value(1);

    e = exp(-(A*A)*(x2-x1)*(t - start));
    resValue = ((x1*x2)*(1 - e))/(x1 - x2*e);

    return resValue;

}

double SignalProcessor::countIntegralH(double lower,double higher,double integrationStep)
{
    double sum = 0;

    int N = (higher - lower)/integrationStep;

    sum+= switchingRegimeFunctionH(lower)/2 * integrationStep;

    for (int i = 1; i < N; i++){

    }
}

QVector <double> SignalProcessor::switchingRegimeFilter(
            QVector <double> signal,
            double start,
            double sigma,
            double discretizationStep,
            double signalGenerationParam, //A
            double noiseGenerationParam //a
        )
{

    QVector <double> result;
    result.insert(0,signal.value(0));

    int n; //number of steps
    n = signal.size();

    double member1, member2, member3;

    double s; //time between current time and previous time
    double t = start; //current time
    double integrationStep = discretizationStep/10; //for integral counting

    double prevSignalValue;
    double currSignalValue;
    double resultSum;

    double firstIntegral;
    double secondIntegral;

    member1 = result.value(0);

    for (int i = 1; i<n; i++) {

    }

    qDebug("===================Starting filtration===================");
    qDebug("Start at %f", start);
    qDebug("Member 1     |     Member 2    |      Member 3");

    for (int i = 1; i<n; i++) {
        prevSignalValue = signal.value(i-1);
        currSignalValue = signal.value(i);
        t+= discretizationStep;

        s = ((start + (i*discretizationStep)) + (start + ((i-1)*discretizationStep))/2);
//            qDebug("Time moment: %f", s);
        setFunctionHParameters(start,sigma,noiseGenerationParam,signalGenerationParam);
        secondIntegral = countIntegralH(start, t, integrationStep);
        member3 = exp(secondIntegral);

        firstIntegral = countIntegralH(start, s, integrationStep);

        setFunctionFParameters(start,sigma,noiseGenerationParam,signalGenerationParam);

        member2 = exp(firstIntegral)*switchingRegimeFunctionF(s) * (currSignalValue-prevSignalValue);

        resultSum = (member1+member2)*member3;

        qDebug("%f | %f | %f", member1, member2, member3);

        result.insert(i,resultSum);
    }

    return result;
}


