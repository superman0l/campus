#ifndef ADDEXAM_H
#define ADDEXAM_H

#include <QDialog>

namespace Ui {
class addexam;
}

class addexam : public QDialog
{
    Q_OBJECT

public:
    explicit addexam(QWidget *parent = nullptr);
    ~addexam();

private slots:
    void on_set_clicked();

private:
    Ui::addexam *ui;

signals:
    void change();
};

#endif // ADDEXAM_H
