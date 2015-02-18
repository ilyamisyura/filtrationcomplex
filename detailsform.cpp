#include "detailsform.h"
#include "ui_detailsform.h"

DetailsForm::DetailsForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DetailsForm)
{
    ui->setupUi(this);
}

DetailsForm::~DetailsForm()
{
    delete ui;
}
