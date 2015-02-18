#include "detailswindow.h"
#include "ui_detailswindow.h"

detailswindow::detailswindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::detailswindow)
{
    ui->setupUi(this);
}

detailswindow::~detailswindow()
{
    delete ui;
}
