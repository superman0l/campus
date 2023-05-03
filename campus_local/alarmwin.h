#ifndef ALARMWIN_H
#define ALARMWIN_H

#include <QDialog>
#include "timer.h"
namespace Ui {
class alarmwin;
}

class alarmwin : public QDialog
{
    Q_OBJECT

public:
    explicit alarmwin(timer*mtm,talarm mta,QWidget *parent = nullptr);
    ~alarmwin();
private slots:
    void on_close_button_clicked();
    void on_relay_button_clicked();
private:
    Ui::alarmwin *ui;
    timer*tm;
    talarm ta;
};

#endif // ALARMWIN_H
