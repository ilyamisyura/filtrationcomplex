#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QThread>

#include "qcustomplot.h"
#include "detailsform.h"
#include "datasignal.h"

const double PI=atan(1)*4;


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

    void on_signalSigmaCheckBox_clicked();

    void on_signalCheckBox_clicked();

    void on_filteredSignalCheckBox_clicked();

    void on_simpleGeneratorButton_clicked();

    void on_estimationCheckBox_clicked();

    void on_simpleFilterButton_clicked();

    void on_DetailsButton_clicked();


private:
    Ui::MainWindow *ui;

protected:
    DetailsForm *pDetailsForm;
};

class Graph : public QObject {
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

    void clearData();

    void sendDataToPlot();

signals:

public slots:
    void setSignal(DataSignal *signal);
};

#endif // MAINWINDOW_H
