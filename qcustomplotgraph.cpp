#include "qcustomplotgraph.h"

QCustomPlotGraph::QCustomPlotGraph(QObject *parent) :
    QObject(parent)
{
    this->visible = true;
    this->plotIsConnected = false;
    this->graphNumIsSet = false;

    QPen *pen = new QPen;
    pen->setWidth(1);
    pen->setColor(QColor(0,0,0));
    setPen(*pen);
    delete pen;
}

void QCustomPlotGraph::setPen(QPen pen) {
    this->pen = pen;
    if (plotIsConnected && graphNumIsSet){
        this->connectedPlot->graph(this->graphNum)->setPen(this->pen);
        this->connectedPlot->replot();
    }
}

void QCustomPlotGraph::show(){
    this->visible = true;
    if (plotIsConnected){
        this->sendDataToPlot();
    }
}

void QCustomPlotGraph::hide(){
    this->visible = false;
    if (plotIsConnected){
        this->sendDataToPlot();
    }
}

void QCustomPlotGraph::connectWithPlot(QCustomPlot *pPlot){
    this->connectedPlot = pPlot;
    this->plotIsConnected = true;
}

void QCustomPlotGraph::setGraphNum(int num){
    this->graphNum = num;
    this->graphNumIsSet = true;
}

bool QCustomPlotGraph::readyForPlot(){
    return (this->plotIsConnected && this->graphNumIsSet && !this->xAxis.empty() && !this->yAxis.empty());
}

void QCustomPlotGraph::sendDataToPlot(){
    if (this->readyForPlot()){
        this->connectedPlot->graph(this->graphNum)->setData(this->xAxis,this->yAxis);
        this->connectedPlot->graph(this->graphNum)->setVisible(this->visible);
        this->connectedPlot->replot();
    }
}

void QCustomPlotGraph::setXAxis(QVector <double> x){
    this->xAxis = x;
}

void QCustomPlotGraph::setYAxis(QVector <double> y){
    this->yAxis = y;
}

void QCustomPlotGraph::clearData() {
    this->connectedPlot->graph(graphNum)->clearData();
    this->connectedPlot->replot();
}

void QCustomPlotGraph::setSignal(DataSignal *inputSignal){
    this->setXAxis(inputSignal->getTimeScale());
    this->setYAxis(inputSignal->getData());
}
