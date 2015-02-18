#include "datasignal.h"

DataSignal::DataSignal(QObject *parent) :
    QObject(parent)
{
}

void DataSignal::setData(QVector <double> data){
    this->data = data;
    emit dataChanged(data);
}

void DataSignal::setTimeScale(QVector <double> data){
    this->timeScale = data;
    emit timeScaleChanged(data);
}

QVector <double> DataSignal::getData(){
    return this->data;
}

QVector <double> DataSignal::getTimeScale(){
    return this->timeScale;
}
