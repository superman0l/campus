#ifndef MAPWIN_H
#define MAPWIN_H

#include <QWidget>

namespace Ui {
class MapWin;
}

class MapWin : public QWidget
{
    Q_OBJECT

public:
    explicit MapWin(QWidget *parent = nullptr);
    ~MapWin();

private:
    Ui::MapWin *ui;
};

#endif // MAPWIN_H