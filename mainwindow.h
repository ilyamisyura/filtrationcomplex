#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QThread>

#include "qcustomplot.h"
#include "qcustomplotgraph.h"
#include "detailsform.h"
#include "datasignal.h"

#include "discretegenerator.h"

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


    void on_signalNoiseCheckBox_clicked();

private:
    Ui::MainWindow *ui;

protected:
    DetailsForm *pDetailsForm;
};

#endif // MAINWINDOW_H
