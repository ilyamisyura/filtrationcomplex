#ifndef DETAILSFORM_H
#define DETAILSFORM_H

#include <QMainWindow>

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
};

#endif // DETAILSFORM_H
