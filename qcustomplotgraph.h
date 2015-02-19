#ifndef QCUSTOMPLOTGRAPH_H
#define QCUSTOMPLOTGRAPH_H

#include <QObject>
#include "datasignal.h"
#include "qcustomplot.h"

class QCustomPlotGraph : public QObject {
    Q_OBJECT
private:
    bool visible;
    bool plotIsConnected;

    QCustomPlot *connectedPlot;

    bool graphNumIsSet;
    int graphNum;

    QVector <double> xAxis;
    QVector <double> yAxis;
    QPen pen;

public:
    explicit QCustomPlotGraph(QObject *parent = 0);

    void connectWithPlot();

    void show();
    void hide();

    void setPen(QPen pen);

    void setGraphNum(int num);

    void connectWithPlot(QCustomPlot *plot);

    void setXAxis(QVector <double> x);
    void setYAxis(QVector <double> y);



    bool readyForPlot();

    void clearData();

    void sendDataToPlot();

signals:

public slots:
    void setSignal(DataSignal *signal);
};

#endif // QCUSTOMPLOTGRAPH_H
