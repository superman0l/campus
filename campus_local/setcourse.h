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

private:
    Ui::setcourse *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // SETCOURSE_H
