#include "discretegenerator.h"

DiscreteGenerator::DiscreteGenerator()
{
}

DataSignal* DiscreteGenerator::generateSimpleSignal(double num,double numJumps,
                                    double alpha, double sigmaS, double sigmaN, FilterCore *core){

    DataSignal *signal = new DataSignal;

    QVector <double> MyX;
    QVector <double> MyY;
    QVector <double> timeScale;
    QVector <double> MyRN1;
    QVector <double> MyRN2;

    for (int i=0; i<=num-1; i++){
        MyRN1.insert(i,core->myRand(sigmaS));
    }

    MyY.insert(0,0);
    int jumpsCount;
    jumpsCount=0;

    for (int i=1; i<=num-1; i++)
    {
        int rn = core->intRand(ceil(num/10));
        if ((rn==5) && (jumpsCount<numJumps))
        {
            jumpsCount++;
            MyY.insert(i,MyY.value(i-1)+alpha*MyRN1.value(i-1));
        }
        else
        {
            MyY.insert(i,MyY[i-1]+MyRN1[i-1]);
        }
    }

    for (int i=0; i<=num-1; i++){
        MyRN2.insert(i,core->myRand(sigmaN));
    }

    for (int i=0; i<=num-1; i++)
    {
        timeScale.insert(i,i);
        MyX.insert(i,MyY.value(i)+MyRN2.value(i));
    }

    signal->setData(MyX);
    signal->setTimeScale(timeScale);
    emit signalIsGenerated(signal);
    return signal;
}
