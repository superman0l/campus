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
    std::vector<QJsonArray> checktables;

private slots:
    void on_add_clicked();

    void on_check_stateChanged(int arg1);

    void on_tag_currentIndexChanged(int index);

    void on_add_2_clicked();

    void on_day_currentIndexChanged(int index);

signals:
    void flash(int a,int b,int c);
    void flash_2(int a, int b);

private:
    Ui::addactivity *ui;
};

#endif // ADDACTIVITY_H
