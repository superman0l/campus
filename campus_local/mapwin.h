#ifndef MAPWIN_H
#define MAPWIN_H

#include <QWidget>
#include<QRadioButton>
#include<QButtonGroup>
#include<QGraphicsView>
#include"map.h"
namespace Ui {
class MapWin;
}
class MapButton:public QRadioButton
{
    Q_OBJECT
public:
    explicit MapButton(const position&pos,QWidget*parent=nullptr);

};
class ButtonGroup:public QButtonGroup
{
    Q_OBJECT
public:
    explicit ButtonGroup(QWidget*parent=nullptr);
    explicit ButtonGroup(map*mp,QWidget*parent=nullptr);
};
class MapView:public QGraphicsView
{
    Q_OBJECT
 public:
    qreal m_scale;
    explicit MapView(QWidget*parent=nullptr);
    void wheelEvent(QWheelEvent*event);
};

class MapWin : public QWidget
{
    Q_OBJECT

public:
    explicit MapWin(QWidget *parent = nullptr);
    ~MapWin();
    ButtonGroup* bg;
    map*mp;
    MapView* mv;
    QGraphicsItemGroup*gig;
    int status;//表示窗口状态,第1位置1表示出发点确定,第2位置1表示终点确定
    int begin;//起始点,导航功能中使用
    std::vector<int>end;//目的地,导航功能中使用
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
private:
    Ui::MapWin *ui;
};

#endif // MAPWIN_H