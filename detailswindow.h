#ifndef DETAILSWINDOW_H
#define DETAILSWINDOW_H

#include <QMainWindow>

namespace Ui {
class detailswindow;
}

class detailswindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit detailswindow(QWidget *parent = 0);
    ~detailswindow();

private:
    Ui::detailswindow *ui;
};

#endif // DETAILSWINDOW_H
