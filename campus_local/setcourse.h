#ifndef SETCOURSE_H
#define SETCOURSE_H

#include <QDialog>

namespace Ui {
class setcourse;
}

class setcourse : public QDialog
{
    Q_OBJECT

public:
    explicit setcourse(QWidget *parent = nullptr);
    ~setcourse();
    QString name,teacher;
    int day;

private:
    Ui::setcourse *ui;
    void closeEvent(QCloseEvent *event);

private slots:
    void receivesetData(QModelIndex indx, QString show);

    void on_startweek_currentIndexChanged(int index);
    void on_endweek_currentIndexChanged(int index);
    void on_set_clicked();

signals:
    void change();
};

#endif // SETCOURSE_H
