#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupPlot();

private slots:
    void on_switchingRegimeGeneratorButton_clicked();

    void on_switchingRegimeFilterButton_clicked();

private:
    Ui::MainWindow *ui;
};

class Graph {
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
    Graph();

    void connectWithPlot();

    void show();
    void hide();

    void setPen(QPen pen);

    void setGraphNum(int num);

    void connectWithPlot(QCustomPlot *plot);

    void setXAxis(QVector <double> x);
    void setYAxis(QVector <double> y);

    bool readyForPlot();

    void sendDataToPlot();

};

#endif // MAINWINDOW_H
