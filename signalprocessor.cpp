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
    double sum;
    double j;
    int N;

    sum = switchingRegimeFunctionH(lower);

    N = (higher - lower)/integrationStep;

    j = lower + integrationStep;

    for (int i = 1; i < N; i++){
        sum+= switchingRegimeFunctionH(j);
        j+= integrationStep;
    }

    sum = sum*2;
    sum+= switchingRegimeFunctionH(j);
    sum = sum*(integrationStep/2);

    return sum;
}

QVector <double> SignalProcessor::switchingRegimeFilter(
            QVector <double> signal,
            QVector <QVector <double> > tauAndSigmas,
            double discretizationStep,
            double signalGenerationParam, //A
            double noiseGenerationParam //a
        )
{

    QVector <double> currentSigmaPart;
    double currentSigma;
    double tauStart;
    double tauEnd;

    QVector <double> result;

    int ns; // number of sigma parts
    ns = tauAndSigmas.size();

    double member1, member2, member3;

    double s; //time between current time and previous time
    double t; //current time
    double integrationStep = discretizationStep/100; //for integral counting

    double prevSignalValue;
    double currSignalValue;
    double resultSum;

    double firstIntegral;
    double secondIntegral;

    int resI;
    int i;

    resI = 0;


//    qDebug("=========================================================");
//    qDebug("NEW FILTRATION");
//    qDebug("=========================================================");
//    qDebug("Here we have dem tau with %d parts", ns);
    for (int k=0; k<ns; k++){
        currentSigmaPart = tauAndSigmas.value(k);
        currentSigma = currentSigmaPart.value(1);
        tauStart = currentSigmaPart.value(0);
//        qDebug("%d: %f",k,tauStart);
        tauEnd = tauAndSigmas.value(k+1).value(0);
    }
//    qDebug("===========================");
//    qDebug("starting to parse Tau and Sigmas vector");
    for (int k=0; k<ns; k++){
        currentSigmaPart = tauAndSigmas.value(k);
        currentSigma = currentSigmaPart.value(1);
        tauStart = currentSigmaPart.value(0);
        tauEnd = tauAndSigmas.value(k+1).value(0);
//        qDebug("%f: %f",tauStart,tauEnd);
        t = tauStart;

        int np = currentSigmaPart.size();

        result.insert(resI,signal.value(resI));
        member1 = result.value(resI);

        i = 0;

        while (t < tauEnd-discretizationStep) {
            resI++;
            t+= discretizationStep;

            prevSignalValue = signal.value(resI-1);
            currSignalValue = signal.value(resI);

            s = ((tauStart + (i*discretizationStep)) + (tauStart + ((i-1)*discretizationStep))/2);

            setFunctionHParameters(tauStart,currentSigma,noiseGenerationParam,signalGenerationParam);
            secondIntegral = countIntegralH(tauStart, t, integrationStep);
            member3 = exp(secondIntegral);

            firstIntegral = countIntegralH(tauStart, s, integrationStep);

            setFunctionFParameters(tauStart,currentSigma,noiseGenerationParam,signalGenerationParam);

            member2 = exp(firstIntegral)*switchingRegimeFunctionF(s) * (currSignalValue-prevSignalValue);

            resultSum = (member1+member2)*member3;

            result.insert(resI,resultSum);

            i++;
        }
    }

    return result;
}


