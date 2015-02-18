#ifndef DATASIGNAL_H
#define DATASIGNAL_H

#include <QObject>
#include <QVector>

class DataSignal : public QObject
{
    Q_OBJECT
public:
    explicit DataSignal(QObject *parent = 0);
    QVector <double> getData();
    QVector <double> getTimeScale();

private:
    QVector <double> data;
    QVector <double> timeScale;

signals:
    void dataChanged(QVector <double> data);
    void timeScaleChanged(QVector <double> data);

public slots:
    void setData(QVector <double> data);
    void setTimeScale(QVector <double> timeScale);


};

#endif // DATASIGNAL_H
