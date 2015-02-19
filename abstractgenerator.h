#ifndef ABSTRACTGENERATOR_H
#define ABSTRACTGENERATOR_H

#include <QObject>
#include "datasignal.h"

class AbstractGenerator : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGenerator(QObject *parent = 0);

signals:
    void signalIsGenerated(DataSignal *signal);

public slots:

};

#endif // ABSTRACTGENERATOR_H
