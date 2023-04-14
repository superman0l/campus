#ifndef CLASSESWIND_H
#define CLASSESWIND_H

#include <QDockWidget>

namespace Ui {
class ClassesWind;
}

class ClassesWind : public QDockWidget
{
    Q_OBJECT

public:
    explicit ClassesWind(QWidget *parent = nullptr);
    ~ClassesWind();

private:
    Ui::ClassesWind *ui;
};

#endif // CLASSESWIND_H
