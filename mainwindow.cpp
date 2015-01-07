#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filtercore.h"
#include "signalgenerator.h"

//class instances
filtercore core;
SignalGenerator generator;

//Switching regime
QVector < QVector<double> > signalsVector;
double tauStart;
double sigmaStart;
QVector <double> end;
double tauEnd;
double sigmaEnd;
int graphI;

namespace common {
    QVector <QVector <double> > signalsVector;
}

namespace switchingRegimeInputs {
    QVector <double> startVec;

    QVector < QVector<double> > tauAndSigmas;

    QVector <double> swx;// X axis for graphs

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
  // The following plot setup is mostly taken from the plot demos:
  ui->plot->addGraph();
  ui->plot->graph()->setPen(QPen(Qt::blue));
  ui->plot->addGraph();
  ui->plot->graph()->setPen(QPen(Qt::red));
  ui->plot->addGraph();
  ui->plot->graph()->setPen(QPen(Qt::black));
}

void MainWindow::on_switchingRegimeGeneratorButton_clicked()
{

    using namespace switchingRegimeInputs;

    common::signalsVector.clear();

    QVector <double> swy;

    QVector <double> signalY;

    signalY.clear();

    QVector <double> filteredSignal;
    QVector <double> filteredGraph;
    QVector <double> noiseGraph;

    QVector <double> gammaPart;
    QVector <double> gammaSignal;

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

    QVector <double> noiseSignal; //noise (X)
    QVector <double> unfilteredSignal; //signal with noise (Y)

    tauAndSigmas = core.switchingRegimeSigmaGenerator(lowerSigma,sigmaStep,numberOfIntervals,volatility,exitCondition);

    int size = tauAndSigmas.size();

    unfilteredSignal.clear();
    unfilteredSignal.insert(0,1);

    /**
     * @todo reset to normal distribution value
     */
    double lastSignalPartValue = 0;

    double j = 0;

    for (int i=0; i<=size-1; i++){

        startVec.clear();
        unfilteredSignal.clear();
        noiseSignal.clear();

        startVec = tauAndSigmas.value(i);
        double tauStart = startVec.value(0);
        double sigmaStart = startVec.value(1);

        end.clear();
        end = tauAndSigmas.value(i+1);
        tauEnd = end.value(0);
        sigmaEnd = end.value(1);

        noiseSignal = generator.generateSwitchingRegimeNoise(tauStart,tauEnd,discretizationStep,sigmaStart,lastSignalPartValue,noiseGeneratorParam);
        unfilteredSignal = generator.generateSwitchingRegimeSignal(noiseSignal, signalGeneratorParam, discretizationStep);
        signalsVector.push_back(unfilteredSignal);
//        gammaPart = Filter.generateGammaProcess(tauStart,tauEnd,discretizationStep,sigmaStart,signalGeneratorParam,noiseGeneratorParam);

//        filteredSignal= Filter.switchingRegimeFilter(unfilteredSignal,tauStart,sigmaStart,discretizationStep,signalGeneratorParam,noiseGeneratorParam);

        lastSignalPartValue = unfilteredSignal.value(unfilteredSignal.size()-1);

        int signalI = 0;

        graphI = 0;

        while ((j+discretizationStep < tauEnd)&&(j < exitCondition)) {
            swx.insert(graphI, j);
            swy.insert(graphI, sigmaStart);
            signalY.insert(graphI,unfilteredSignal.value(signalI));
            signalI++;
            gammaSignal.insert(graphI,gammaPart.value(signalI));
            noiseGraph.insert(graphI,noiseSignal.value(signalI));
            filteredGraph.insert(graphI,filteredSignal.value(signalI));
            graphI++;
            j+= discretizationStep;
        }

//        qDebug("j end = %f,",j);
//        qDebug("tau start = %f, tau end = %f",tauStart,tauEnd);
    }

    double signalMin = core.getSignalMin(signalY);
    double signalMax = core.getSignalMax(signalY);

    double sigmaMax = core.getSignalMax(swy);

    double graphMin = fmin(signalMin, lowerSigma);
    double graphMax = fmax(signalMax, sigmaMax);

    /**
     * @todo comment in future
     */

//    double gammaMin = core.getSignalMax(gammaSignal);
//    double gammaMax = core.getSignalMax(gammaSignal);
//    graphMin = fmin(graphMin,gammaMin);
//    graphMax = fmax(graphMax,gammaMax);


//    qDebug("lower sigma: %f", lowerSigma);
//    qDebug("higher sigma: %f", lowerSigma + sigmaStep*numberOfIntervals);
//    qDebug("signal min: %f", signalMin);
//    qDebug("signal max: %f", signalMax);
//    qDebug("GrapMin: %f", graphMin);
//    qDebug("GrapMax: %f", graphMax);

    QPen redBoldPen;
    redBoldPen.setWidth(2);
    redBoldPen.setColor(QColor(255,0,0));

    QPen blueBoldPen;
    blueBoldPen.setWidth(2);
    blueBoldPen.setColor(QColor(0,0,255));

    QPen blackBoldPen;
    blackBoldPen.setWidth(1);
    blackBoldPen.setColor(QColor(0,0,0));

    ui->plot->graph(0)->setData(swx,swy);
    ui->plot->graph(0)->setPen(blueBoldPen);

    ui->plot->graph(1)->setData(swx,signalY);
    ui->plot->graph(1)->setPen(redBoldPen);

    ui->plot->graph(2)->clearData();
//    ui->plot->graph(2)->setData(swx,noiseGraph);
//    ui->plot->graph(2)->setPen(blackBoldPen);

//    ui->plot->graph(2)->setData(swx,filteredGraph);
//    ui->plot->graph(2)->setPen(blackBoldPen);

    ui->plot->xAxis->setRange(0,exitCondition);
    ui->plot->yAxis->setRange(graphMin-1, graphMax-graphMin+1);

    ui->plot->replot();

    filteredSignal.clear();
    filteredGraph.clear();
    noiseGraph.clear();
    noiseSignal.clear();
    unfilteredSignal.clear();
    swx.clear();
}

void MainWindow::on_switchingRegimeFilterButton_clicked()
{

}
