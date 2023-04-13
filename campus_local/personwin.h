#ifndef PERSONWIN_H
#define PERSONWIN_H

#include <QWidget>

namespace Ui {
class PersonWin;
}

class PersonWin : public QWidget
{
    Q_OBJECT

public:
    explicit PersonWin(QWidget *parent = nullptr);
    ~PersonWin();

private:
    Ui::PersonWin *ui;
};

#endif // PERSONWIN_H
