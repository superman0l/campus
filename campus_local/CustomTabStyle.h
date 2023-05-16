#ifndef CUSTOMTABSTYLE_H
#define CUSTOMTABSTYLE_H

//下面直接CV了代码，实现TabBar的左侧放至且文字水平
#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QRect>
#include <QSize>


class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 140; // 设置每个tabBar中item的大小
            s.rheight() = 120;
        }
        return s;
    }
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;
                allRect.setWidth(allRect.width() - 5);
                allRect.setHeight(allRect.height() - 2);
                //选中状态
                if (tab->state & QStyle::State_Selected) {
                    //save用以保护坐标，restore用来退出状态
                    painter->save();

                    QLinearGradient ling(QPoint(0,60),QPoint(120,60));//参数1指定起始点，参数2指定结束点
                    ling.setColorAt(0,QColor(14,120,255,220));//指定起点颜色
                    ling.setColorAt(1,QColor(226,226,226,150));//指定终点颜色
                    ling.setSpread(QGradient::PadSpread);//设置起点和终点之外的显示模式
                    QBrush brush(ling);
                    painter->setBrush(brush);
                    //painter->setBrush(QColor(34,190,255,150));
                    //矩形
                    //painter->drawRect(allRect.adjusted(0, 0, 0, 0));
                    //带有弧线矩形
                    painter->drawRoundedRect(tab->rect, 8, 8);
                    painter->restore();
                }
                //hover状态
                else if(tab->state & QStyle::State_MouseOver){
                    painter->save();
                    QLinearGradient ling(QPoint(0,60),QPoint(120,60));//参数1指定起始点，参数2指定结束点
                    ling.setColorAt(0,QColor(44,180,255,200));//指定起点颜色
                    ling.setColorAt(1,QColor(226,226,226,150));//指定终点颜色
                    ling.setSpread(QGradient::PadSpread);//设置起点和终点之外的显示模式
                    QBrush brush(ling);
                    painter->setBrush(brush);
                    //painter->setBrush(QBrush(0x696969));
                    painter->drawRoundedRect(allRect, 8, 8);
                    painter->restore();
                }
                else{
                    painter->save();
                    painter->setBrush(QColor(34,190,255,10));
                    painter->drawRoundedRect(allRect, 8, 8);
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                painter->setFont(QFont("楷体", 18, QFont::Bold));
                painter->setPen(0x000001);
                painter->drawText(allRect, tab->text, option);
                return;
            }
        }
        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};
//CV到这里为止
#endif // CUSTOMTABSTYLE_H
