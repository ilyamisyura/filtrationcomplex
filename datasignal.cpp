#include "datasignal.h"

DataSignal::DataSignal(QObject *parent) :
    QObject(parent)
{
}

/**
 * sets main data
 * @brief DataSignal::setData
 * @param data
 */
void DataSignal::setData(QVector <double> data){
    this->data = data;
    emit dataChanged(data);
}

void DataSignal::setTimeScale(QVector <double> data){
    this->timeScale = data;
    emit timeScaleChanged(data);
}

void DataSignal::setNoise(QVector <double> noise){
    this->noise = noise;
    emit noiseChanged(noise);
}

QVector <double> DataSignal::getData(){
    return this->data;
}

QVector <double> DataSignal::getTimeScale(){
    return this->timeScale;
}

void DataSignal::clear(){
    this->data.clear();
    this->timeScale.clear();
    emit cleared();
}
