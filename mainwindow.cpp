#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filtercore.h"
#include "signalgenerator.h"
#include "signalprocessor.h"

Graph::Graph(){

    this->visible = true;
    this->plotIsConnected = false;
    this->graphNumIsSet = false;

    QPen *pen = new QPen;
    pen->setWidth(1);
    pen->setColor(QColor(0,0,0));
    setPen(*pen);
    delete pen;
}

void Graph::setPen(QPen pen) {
    Graph::pen = pen;
    if (plotIsConnected && graphNumIsSet){
        this->connectedPlot->graph(this->graphNum)->setPen(this->pen);
        this->connectedPlot->replot();
    }
}

void Graph::show(){
    this->visible = true;
    if (plotIsConnected){
        this->sendDataToPlot();
    }
}

void Graph::hide(){
    this->visible = false;
    if (plotIsConnected){
        this->sendDataToPlot();
    }
}

void Graph::connectWithPlot(QCustomPlot *pPlot){
    this->connectedPlot = pPlot;
    this->plotIsConnected = true;
}

void Graph::setGraphNum(int num){
    this->graphNum = num;
    this->graphNumIsSet = true;
}

bool Graph::readyForPlot(){
    return (this->plotIsConnected && this->graphNumIsSet && !this->xAxis.empty() && !this->yAxis.empty());
}

void Graph::sendDataToPlot(){
    if (this->readyForPlot()){
        this->connectedPlot->graph(this->graphNum)->setData(this->xAxis,this->yAxis);
        this->connectedPlot->graph(this->graphNum)->setVisible(this->visible);
        this->connectedPlot->replot();
    }
}

void Graph::setXAxis(QVector <double> x){
    this->xAxis = x;
}

void Graph::setYAxis(QVector <double> y){
    this->yAxis = y;
}

//class instances
FilterCore core;
SignalGenerator generator;
SignalProcessor processor;

//Switching regime
QVector < QVector<double> > signalsVector;
QVector <double> swx;// X axis for graphs
double tauStart;
double sigmaStart;
QVector <double> end;
double tauEnd;
double sigmaEnd;
int graphI;

namespace common {
    QVector <double> signalVector;
    QVector <double> sigmaVector;
    QVector <double> filteredSignalVector;
    QVector <double> xAxis;
}

namespace switchingRegimeInputs {
    QVector <double> startVec;

    QVector < QVector<double> > tauAndSigmas;



    double lowerSigma;

    double sigmaStep;
    int numberOfIntervals;

    double volatility;

    double discretizationStep;

    double exitCondition;
    double startingValueMu;

    double startingValueSigma;

    double noiseGeneratorParam;

    double signalGeneratorParam;
}

namespace signalGraph {
    Graph sigma;
    Graph noise;
    Graph signal;
}

namespace filteredSignalGraph {
    Graph estimation;
    Graph sigma;
    Graph signal;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //plot pre-setup
    setupPlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPlot()
{
    //Main signal graphs
    ui->plot->addGraph();
    signalGraph::sigma.connectWithPlot(ui->plot);
    signalGraph::sigma.setGraphNum(0);

    ui->plot->addGraph();// for noise, now unused

    ui->plot->addGraph();
    signalGraph::signal.connectWithPlot(ui->plot);
    signalGraph::signal.setGraphNum(2);

    ui->plot->addGraph();
    filteredSignalGraph::estimation.connectWithPlot(ui->plot);
    filteredSignalGraph::estimation.setGraphNum(3);
//  ui->plot->graph()->setPen(QPen(Qt::black));
}

void MainWindow::on_switchingRegimeGeneratorButton_clicked()
{

    using namespace switchingRegimeInputs;

    bool ok;
    int resErr = 0;

    lowerSigma = ui->lowerSigmaLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;

    sigmaStep = ui->sigmaStepLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;

    numberOfIntervals = ui->numberOfIntervalsLineEdit->text().toInt(&ok);
    if (!ok)
        resErr=1;

    volatility = ui->volatilityLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;

    discretizationStep = ui->discretizationStepLineEdit->text().toDouble();
    if (!ok)
        resErr=1;

    exitCondition = ui->exitConditionLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;
    ui->plot->xAxis->setRange(0,exitCondition);

    startingValueMu = ui->startingValueMuLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;

    startingValueSigma = ui->startingValueSigmaLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;

    noiseGeneratorParam = ui->noiseParamLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;

    signalGeneratorParam = ui->signalParamLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;


    tauAndSigmas.clear();
    tauAndSigmas = core.switchingRegimeSigmaGenerator(lowerSigma,sigmaStep,numberOfIntervals,volatility,exitCondition);

    int size;
    size= tauAndSigmas.size();

    double j;
    j = 0;
    int graphI;
    graphI = 0;

    common::sigmaVector.clear();

    for (int i=0; i<=size-1; i++){
        startVec = tauAndSigmas.value(i);
        double sigmaStart = startVec.value(1);

        end.clear();
        end = tauAndSigmas.value(i+1);
        tauEnd = end.value(0);
        sigmaEnd = end.value(1);

        while ((j+discretizationStep < tauEnd)&&(j < exitCondition)) {
            common::xAxis.insert(graphI, j);
            common::sigmaVector.insert(graphI, sigmaStart);
            graphI++;
            j+= discretizationStep;
        }
    }

    signalGraph::sigma.setXAxis(common::xAxis);
    signalGraph::sigma.setYAxis(common::sigmaVector);
    signalGraph::sigma.sendDataToPlot();

    common::signalVector.clear();
    common::signalVector = generator.generateSwitchingRegimeSignal(tauAndSigmas,noiseGeneratorParam,signalGeneratorParam,exitCondition,discretizationStep);

    ui->plot->yAxis->setRange(core.getTwoSignalsMin(common::signalVector,common::sigmaVector)-1,
                               core.getTwoSignalsMax(common::signalVector,common::sigmaVector)+1);

    signalGraph::signal.setXAxis(common::xAxis);
    signalGraph::signal.setYAxis(common::signalVector);
    QPen *pen = new QPen;
    pen->setWidth(2);
    pen->setColor(QColor(255,0,0));
    signalGraph::signal.setPen(*pen);
    delete pen;
    signalGraph::signal.sendDataToPlot();
}

void MainWindow::on_switchingRegimeFilterButton_clicked()
{
    using namespace switchingRegimeInputs;

    QVector <QVector <double>> filtrationTauAndSigmas;
    QVector <double> currentEstimation;

    filtrationTauAndSigmas = core.switchingRegimeSigmaGenerator(lowerSigma,sigmaStep,numberOfIntervals,volatility,exitCondition);

    currentEstimation = processor.switchingRegimeFilter(common::signalVector, filtrationTauAndSigmas, discretizationStep, signalGeneratorParam, noiseGeneratorParam);

    filteredSignalGraph::estimation.setXAxis(common::xAxis);
    filteredSignalGraph::estimation.setYAxis(currentEstimation);
    filteredSignalGraph::estimation.sendDataToPlot();


}

void MainWindow::on_signalSigmaCheckBox_clicked()
{
    if (ui->signalSigmaCheckBox->isChecked()) {
        signalGraph::sigma.show();
    } else {
        signalGraph::sigma.hide();
    }
}

void MainWindow::on_signalCheckBox_clicked()
{
    if (ui->signalCheckBox->isChecked()) {
        signalGraph::signal.show();
    } else {
        signalGraph::signal.hide();
    }
}
