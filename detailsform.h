#ifndef DETAILSFORM_H
#define DETAILSFORM_H

#include <QMainWindow>
#include <datasignal.h>
#include "math.h"

namespace Ui {
class DetailsForm;
}

class DetailsForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit DetailsForm(QWidget *parent = 0);
    ~DetailsForm();

private:
    Ui::DetailsForm *ui;
    bool isInfiniteReady;
    bool isDiscreteReady;
    bool signalDataIsSet;
    bool filteredSignalDataIsSet;

signals:
    void tableResetted();

public slots:
    void setSignalData(QVector <double> data);
    void setFilteredSignalData(QVector <double> data);
    void prepareInfiniteFiltrationDetails();
    void prepareDiscreteFiltrationDetails();
    void resetTable();
};

#endif // DETAILSFORM_H
