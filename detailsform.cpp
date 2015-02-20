#include "detailsform.h"
#include "ui_detailsform.h"

DetailsForm::DetailsForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DetailsForm)
{
    ui->setupUi(this);
    this->isInfiniteReady = false;
    this->isDiscreteReady = false;
    this->signalDataIsSet = false;
}

DetailsForm::~DetailsForm()
{
    delete ui;
}

void DetailsForm::prepareInfiniteFiltrationDetails(){
    ui->tableWidget->setColumnCount(2);
    this->isInfiniteReady = true;
    this->isDiscreteReady = false;
}

void DetailsForm::prepareDiscreteFiltrationDetails(){
    ui->tableWidget->setColumnCount(2);
    this->isInfiniteReady = true;
    this->isDiscreteReady = false;
}

void DetailsForm::setSignalData(QVector <double> data){
    int maxRows;
    maxRows = fmax(ui->tableWidget->columnCount(),data.size());

    ui->tableWidget->setRowCount(maxRows);
    for (int i=0;i<data.size();i++){
        QVariant *v = new QVariant(data.value(i));
        QTableWidgetItem *item = new QTableWidgetItem(v->toString());
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i,0,item);
        /*delete item;
        delete v;*/
    }
    this->signalDataIsSet = true;
}

void DetailsForm::setFilteredSignalData(QVector <double> data){

    int colNum;
    if (this->isInfiniteReady){
        colNum = 1;
    } else if (this->isDiscreteReady) {
        colNum = 3;
    } else {
        return;
    }

    if (this->signalDataIsSet){
        for (int i=0;i<data.size();i++){
            QVariant *v = new QVariant(data.value(i));
            QTableWidgetItem *item = new QTableWidgetItem(v->toString());
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->tableWidget->setItem(i,colNum,item);
            /*delete item;
            delete v;*/
        }
    }
    this->filteredSignalDataIsSet = true;
}

void DetailsForm::resetTable() {
    ui->tableWidget->reset();
    emit tableResetted();
}
