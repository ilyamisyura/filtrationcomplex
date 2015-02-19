#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filtercore.h"
#include "signalgenerator.h"
#include "signalprocessor.h"

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

    //new
    DataSignal unfilteredSignal;
}
namespace simpleInputs {
    double noiseSigma;
    double signalSigma;
    double proportionality;
    int countings;
    double jumpAlpha;
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
    QCustomPlotGraph sigma;
    QCustomPlotGraph noise;
    QCustomPlotGraph signal;
}

namespace filteredSignalGraph {
    QCustomPlotGraph estimation;
    QCustomPlotGraph sigma;
    QCustomPlotGraph signal;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pDetailsForm = new DetailsForm;
    pDetailsForm->prepareInfiniteFiltrationDetails();
    QObject::connect(&common::unfilteredSignal,&DataSignal::dataChanged,pDetailsForm,&DetailsForm::setSignalData);
    //plot pre-setup
    setupPlot();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pDetailsForm;
}

void MainWindow::setupPlot()
{
    //Main signal graphs
    ui->plot->addGraph();
    signalGraph::sigma.connectWithPlot(ui->plot);
    signalGraph::sigma.setGraphNum(0);
    signalGraph::sigma.hide();


    ui->plot->addGraph();// for noise, now unused

    ui->plot->addGraph();
    signalGraph::signal.connectWithPlot(ui->plot);
    signalGraph::signal.setGraphNum(2);
    QPen *pen = new QPen;
    pen->setWidth(2);
    pen->setColor(QColor(255,0,0));
    signalGraph::signal.setPen(*pen);
    delete pen;

    ui->plot->addGraph();
    filteredSignalGraph::estimation.connectWithPlot(ui->plot);
    filteredSignalGraph::estimation.setGraphNum(3);
    filteredSignalGraph::estimation.hide();

    ui->plot->addGraph();
    filteredSignalGraph::signal.connectWithPlot(ui->plot);
    filteredSignalGraph::signal.setGraphNum(4);
    QPen *blackPen = new QPen;
    blackPen->setWidth(3);
    blackPen->setColor(QColor(0,0,0));
    filteredSignalGraph::signal.setPen(*blackPen);
    delete blackPen;
//  ui->plot->graph()->setPen(QPen(Qt::black));

    //setting visibility
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
    tauAndSigmas = core.switchingRegimeSigmaGenerator(lowerSigma,sigmaStep,numberOfIntervals,volatility,exitCondition,discretizationStep);

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
    common::signalVector = generator.generateSwitchingRegimeSignal(tauAndSigmas,noiseGeneratorParam,signalGeneratorParam,exitCondition,discretizationStep,startingValueMu,startingValueSigma);

    //new
    common::unfilteredSignal.setData(common::signalVector);
    common::unfilteredSignal.setTimeScale(common::xAxis);


    double margin;
    margin = (core.getSignalMax(common::signalVector) - core.getSignalMin(common::signalVector))/20;

    ui->plot->yAxis->setRange(core.getSignalMin(common::signalVector)-margin,
                               core.getSignalMax(common::signalVector)+margin);

//    signalGraph::signal.setXAxis(common::xAxis);
//    signalGraph::signal.setYAxis(common::signalVector);
    signalGraph::signal.setSignal(&common::unfilteredSignal);
    signalGraph::signal.sendDataToPlot();

    filteredSignalGraph::signal.clearData();
    filteredSignalGraph::estimation.clearData();
}

void MainWindow::on_switchingRegimeFilterButton_clicked()
{
    using namespace switchingRegimeInputs;

    QVector <QVector <double>> filtrationTauAndSigmas;
    QVector <double> currentEstimation;

    int threadNum;

    threadNum = ui->threadsSpinBox->value();

    QVector <QFuture <QVector <double> > > futureEstimationsVector;
    QFuture <QVector <double> > currentFuture;

    for (int k=0;k<threadNum;k++){
        filtrationTauAndSigmas = core.switchingRegimeSigmaGenerator(lowerSigma,sigmaStep,numberOfIntervals,volatility,exitCondition,discretizationStep);
        currentFuture = QtConcurrent::run(processor,&SignalProcessor::switchingRegimeFilter,common::signalVector, filtrationTauAndSigmas, discretizationStep, signalGeneratorParam, noiseGeneratorParam);
        futureEstimationsVector.insert(k,currentFuture);
    }

    for (int k=0;k<threadNum;k++){
        futureEstimationsVector.value(k).waitForFinished();
    }

    double estSumValue;
    double finalEstimationValue;
    estSumValue = 0;
    int resultSize;
    for (int k=0;k<threadNum;k++){
        resultSize = futureEstimationsVector.value(k).result().size();
        for (int i=0;i<resultSize;i++){
            if (k==0){
                finalEstimationValue = 0;
            } else {
                finalEstimationValue = currentEstimation.value(i);
            }
            finalEstimationValue+= futureEstimationsVector.value(k).result().value(i);
            if (k==0){
                currentEstimation.insert(i,finalEstimationValue);
            } else {
                currentEstimation.replace(i,finalEstimationValue);
            }
        }
    }

    common::filteredSignalVector.clear();

    double plusValue;
    plusValue = common::signalVector.value(0);

    for (int i=0;i<currentEstimation.size();i++){
        finalEstimationValue = currentEstimation.value(i)/threadNum;
        currentEstimation.replace(i,finalEstimationValue);
        common::filteredSignalVector.insert(i,common::signalVector.value(i) - finalEstimationValue + plusValue);
    }

//    currentEstimation = processor.switchingRegimeFilter(common::signalVector, filtrationTauAndSigmas, discretizationStep, signalGeneratorParam, noiseGeneratorParam);

    filteredSignalGraph::estimation.setXAxis(common::xAxis);
    filteredSignalGraph::estimation.setYAxis(currentEstimation);
    filteredSignalGraph::estimation.sendDataToPlot();

    filteredSignalGraph::signal.setXAxis(common::xAxis);
    filteredSignalGraph::signal.setYAxis(common::filteredSignalVector);
    filteredSignalGraph::signal.sendDataToPlot();
}

void MainWindow::on_signalSigmaCheckBox_clicked()
{
    double margin;
    margin = (core.getSignalMax(common::signalVector) - core.getSignalMin(common::signalVector))/20;

    if (ui->signalSigmaCheckBox->isChecked()) {
        ui->plot->yAxis->setRange(core.getTwoSignalsMin(common::signalVector,common::sigmaVector)-margin,
                                   core.getTwoSignalsMax(common::signalVector,common::sigmaVector)+margin);
        signalGraph::sigma.show();
    } else {
        ui->plot->yAxis->setRange(core.getSignalMin(common::signalVector)-margin,
                                   core.getSignalMax(common::signalVector)+margin);
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

void MainWindow::on_estimationCheckBox_clicked()
{
    if (ui->estimationCheckBox->isChecked()) {
        filteredSignalGraph::estimation.show();
    } else {
        filteredSignalGraph::estimation.hide();
    }
}

void MainWindow::on_filteredSignalCheckBox_clicked()
{
    if (ui->filteredSignalCheckBox->isChecked()) {
        filteredSignalGraph::signal.show();
    } else {
        filteredSignalGraph::signal.hide();
    }
}

/*
 * SIMPLE MODEL
 */

void MainWindow::on_simpleGeneratorButton_clicked()
{
    using namespace simpleInputs;

    bool ok;
    int resErr;
    resErr = 0;

    noiseSigma = ui->noiseSigmaLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;

    proportionality = ui->proportionalityLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;

    jumpAlpha = ui->jumpAlphaLineEdit->text().toDouble(&ok);
    if (!ok)
        resErr=1;

    countings = ui->countingsLineEdit->text().toInt(&ok);
    if (!ok)
        resErr=1;

    signalSigma = noiseSigma/proportionality;

    common::signalVector.clear();
    common::signalVector = generator.generateSimpleSignal(countings,core.intRand(10),jumpAlpha,signalSigma,noiseSigma,&core);

    common::xAxis.clear();
    for (int i=0; i<countings; i++){
        common::xAxis.insert(i,i);
    }

    ui->plot->xAxis->setRange(0,countings);
    ui->plot->yAxis->setRange(core.getSignalMin(common::signalVector)-1,core.getSignalMax(common::signalVector)-core.getSignalMin(common::signalVector)+1);

    signalGraph::signal.setXAxis(common::xAxis);
    signalGraph::signal.setYAxis(common::signalVector);
    signalGraph::signal.sendDataToPlot();

    filteredSignalGraph::signal.clearData();

}

void MainWindow::on_simpleFilterButton_clicked()
{
    using namespace simpleInputs;
    common::filteredSignalVector = processor.simpleFilter(common::signalVector,signalSigma,noiseSigma);
    filteredSignalGraph::signal.setXAxis(common::xAxis);
    filteredSignalGraph::signal.setYAxis(common::filteredSignalVector);
    filteredSignalGraph::signal.sendDataToPlot();
}

void MainWindow::on_DetailsButton_clicked()
{
    pDetailsForm->show();
}
