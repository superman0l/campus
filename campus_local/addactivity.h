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

signals:
    void flash();

private:
    Ui::addactivity *ui;
};

#endif // ADDACTIVITY_H
