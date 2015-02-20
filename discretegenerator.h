#ifndef DISCRETEGENERATOR_H
#define DISCRETEGENERATOR_H

#include "abstractgenerator.h"
#include "filtercore.h"

class DiscreteGenerator : public AbstractGenerator
{
public:
    DiscreteGenerator();
public slots:
    DataSignal* generateSimpleSignal(
            double num,
            double num_jumps,
            double alpha,
            double sigmaS,
            double sigmaN,
            FilterCore *core);
};

#endif // DISCRETEGENERATOR_H
