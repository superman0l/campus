#ifndef ADDACTIVITY_H
#define ADDACTIVITY_H

#include <QDialog>

namespace Ui {
class addactivity;
}

class addactivity : public QDialog
{
    Q_OBJECT

public:
    explicit addactivity(QWidget *parent = nullptr);
    ~addactivity();

private slots:
    void on_add_clicked();

    void on_check_stateChanged(int arg1);

signals:
    void flash(int a,int b,int c);

private:
    Ui::addactivity *ui;
};

#endif // ADDACTIVITY_H
