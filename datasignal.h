#ifndef DATASIGNAL_H
#define DATASIGNAL_H

#include <QObject>
#include <QVector>

class DataSignal : public QObject
{
    Q_OBJECT

protected:

public:
    explicit DataSignal(QObject *parent = 0);
//    DataSignal( const DataSignal &obj);  // copy constructor
    QVector <double> getData();
    QVector <double> getTimeScale();

private:
    QVector <double> data;
    QVector <double> timeScale;

signals:
    void dataChanged(QVector <double> data);
    void timeScaleChanged(QVector <double> data);
    void cleared();

public slots:
    void setData(QVector <double> data);
    void setTimeScale(QVector <double> timeScale);
    void clear();

};

#endif // DATASIGNAL_H
