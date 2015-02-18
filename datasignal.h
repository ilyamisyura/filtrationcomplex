#ifndef DATASIGNAL_H
#define DATASIGNAL_H

#include <QObject>

class DataSignal : public QObject
{
    Q_OBJECT
public:
    explicit DataSignal(QObject *parent = 0);

signals:

public slots:

};

#endif // DATASIGNAL_H
