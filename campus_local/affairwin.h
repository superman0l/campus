#ifndef AFFAIRWIN_H
#define AFFAIRWIN_H

#include <QWidget>

namespace Ui {
class AffairWin;
}

class AffairWin : public QWidget
{
    Q_OBJECT

public:
    explicit AffairWin(QWidget *parent = nullptr);
    ~AffairWin();

private:
    Ui::AffairWin *ui;
};

#endif // AFFAIRWIN_H
