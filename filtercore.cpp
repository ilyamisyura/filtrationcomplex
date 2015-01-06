#include "filtercore.h"

template <typename T>  class QVector;


/**
 * @brief filtercore::filtercore
 * this class contains basic functions, that can be used both
 * in signal generation and while signal filtration.
 */
filtercore::filtercore()
{
    currentSigmaIndex = 0;
    currentSigma = 0;
}

/**
 * @brief filtercore::intRand returns integer from 0 to randMax
 * @param int randMax
 * @return int
 */
int filtercore::intRand(int randMax)
{
    double randNumber = filtercore::randomNumber(0,randMax);
    return ceil(randNumber);
}

/**
 * @brief filtercore::randomNumber returns random double value from randMin to randMax
 * @param randMin
 * @param randMax
 * @return double
 */
double filtercore::randomNumber(double randMin, double randMax){
    double randNumber = randMin + (randMax - randMin)*(qrand() / static_cast<double>( RAND_MAX ));
    return randNumber;
}

/**
 * @brief filtercore::normalDistribution returns normal distributed random variable. Parameters can be set
 * @param mu
 * @param sigma
 * @return double
 */
double filtercore::normalDistribution(double mu, double sigma) {

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mu,sigma);

    double number = distribution(generator);

    return number;
}

/**
 * @brief filtercore::setNextSigma
 * @param numberOfIntervals
 * @param sigmaVector
 * @param lambdas
 */
void filtercore::setNextSigma(  int numberOfIntervals,
                    QVector <double> sigmaVector,
                    QVector <double> lambdas){

    //probabilities
    double pDown = lambdas[0]/(lambdas[0]+lambdas[2]);
    double pUp = 1 - pDown;

    double rn = randomNumber(0,1);

    if (rn < pUp){
        if (this->currentSigmaIndex != numberOfIntervals){
            this->currentSigmaIndex++;
        } else {
            this->currentSigmaIndex--;
        }
    } else {
        if (this->currentSigmaIndex != 0){
            this->currentSigmaIndex--;
        } else {
            this->currentSigmaIndex++;
        }
    }
    this->currentSigma = sigmaVector[this->currentSigmaIndex];
}


QVector <double> filtercore::switchingRegimeLambdas(int k,
                             double h, double g, double startingSigma){

    QVector <double> res(3);

    double lambdaIIMinus1;
    if (currentSigmaIndex != 0){
        lambdaIIMinus1 = ( 1/(2*h*h) * g * currentSigma ) +
                        ( (1/h) * fmax(0,k*(startingSigma - currentSigma)));
    } else {
        lambdaIIMinus1 = 0;
    }
    res[0] = lambdaIIMinus1;

    double lambdaIIPlus1;
    if (currentSigmaIndex != k) {
        lambdaIIPlus1 = ( 1/(2*h*h) * g * currentSigma ) +
                        ( (1/h) * fmax(0,k*(currentSigma - startingSigma)));
    } else {
        lambdaIIPlus1 = 0;
    }
    res[2] = lambdaIIPlus1;

    double lambdaII = -lambdaIIMinus1 -lambdaIIPlus1;
    res[1] = lambdaII;

    return res;

}


QVector <QVector<double> > filtercore::switchingRegimeSigmaGenerator(
        double lowerSigma,
        double sigmaStep,
        int numberOfIntervals, //between lower and higher sigma
        double volatility,
        double exitCondition
        ){

    QVector < QVector<double> > sigmaGraph;
    QVector <double> tmpVector(2);
    QVector <double> tau;
    QVector <double> lambdas;
    QVector <double> sigmas(numberOfIntervals+1);

    double delta;
    double currentTauValue;
    double previousTauValue;

    qDebug("Sigma generator's started");

    for (int i=0; i<=numberOfIntervals; i++){
        sigmas[i] = lowerSigma + i * sigmaStep;
    }

    currentSigmaIndex = intRand(numberOfIntervals)+1;
    currentSigma = sigmas[currentSigmaIndex];

    double startingSigma = lowerSigma + (sigmaStep*currentSigmaIndex);

    currentTauValue = 0;

    tmpVector[0] = 0;
    tmpVector[1] = startingSigma;
    sigmaGraph.insert(0,tmpVector);

    int i = 1;
    tau.insert(0,0);

    while (currentTauValue < exitCondition) {
        previousTauValue = tau.value(i-1);
        lambdas = switchingRegimeLambdas(numberOfIntervals,
                                     sigmaStep, volatility, startingSigma);
        //qDebug("counting log");

        delta = log(randomNumber(0,1))/lambdas[1];
        currentTauValue = previousTauValue + delta;

        previousTauValue = tau.value(i-1);
        setNextSigma(numberOfIntervals,sigmas,lambdas);

        tmpVector[0] = currentTauValue;
        tmpVector[1] = currentSigma;

        tau.insert(i,currentTauValue);
        sigmaGraph.insert(i,tmpVector);
        i++;
    }
    return sigmaGraph;
}

double filtercore::getSignalMin(QVector <double> V){
    double min = V.value(0);
    for (int i=0; i<V.size(); i++)
    {
        if (V.value(i)<min)
            min = V.value(i);
    }
    return min;
}

double filtercore::getSignalMax(QVector <double> V){
    double max = V.value(0);
    for (int i=0; i<V.size(); i++)
    {
        if (V.value(i)>max)
            max = V.value(i);
    }
    return max;
}


