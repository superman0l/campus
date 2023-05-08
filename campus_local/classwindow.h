#ifndef CLASSWINDOW_H
#define CLASSWINDOW_H

#include <QWidget>

namespace Ui {
class ClassWindow;
}

class ClassWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ClassWindow(QWidget *parent = nullptr);
    ~ClassWindow();

private:
    Ui::ClassWindow *ui;
};

#endif // CLASSWINDOW_H
