#ifndef ADDCOURSE_H
#define ADDCOURSE_H

#include <QDialog>
#include "affair.h"
#include "basic.h"

namespace Ui {
class addcourse;
}

class addcourse : public QDialog
{
    Q_OBJECT

public:
    explicit addcourse(QWidget *parent = nullptr);
    ~addcourse();

private slots:
    void on_check_clicked();

    void on_endweek_currentIndexChanged(int index);

    void on_startweek_currentIndexChanged(int index);

private:
    Ui::addcourse *ui;

signals:
    void change();
};

#endif // ADDCOURSE_H
