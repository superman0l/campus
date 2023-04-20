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

private:
    Ui::addexam *ui;
};

#endif // ADDEXAM_H
