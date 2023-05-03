#include "mapwin.h"
#include "ui_mapwin.h"
#include <QGraphicsPixmapItem>
#include<QGraphicsProxyWidget>
#include<QWheelEvent>
#include<QScrollBar>
#include<QPoint>
#include<QMessageBox>
#include<QUndoStack>
#include<QPainter>
#include<QListView>
#include<QStandardItemModel>
MapButton::MapButton(const position&pos,QWidget*parent):QRadioButton(parent)
{
}
ButtonGroup::ButtonGroup(QWidget*parent):QButtonGroup(parent)
{
}
//来源:https://zhuanlan.zhihu.com/p/469523496?utm_id=0
MapView::MapView(QWidget*parent):QGraphicsView(parent){

        this->m_scale=1;
        this->setGeometry(QRect(535, 20, 401, 561));
        setStyleSheet("padding: 0px; border: 0px;");//无边框
        setMouseTracking(true);//跟踪鼠标位置
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖条
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        setDragMode(QGraphicsView::ScrollHandDrag);
};

//来源:https://blog.csdn.net/GoForwardToStep/article/details/77035287
void MapView::wheelEvent(QWheelEvent *event)
{
    // 获取当前鼠标相对于view的位置;
    QPointF cursorPoint = event->position();
    // 获取当前鼠标相对于scene的位置;
    QPointF scenePos = this->mapToScene(QPoint(cursorPoint.x(), cursorPoint.y()));

    // 获取view的宽高;
    qreal viewWidth = this->viewport()->width();
    qreal viewHeight = this->viewport()->height();

    // 获取当前鼠标位置相当于view大小的横纵比例;
    qreal hScale = cursorPoint.x() / viewWidth;
    qreal vScale = cursorPoint.y() / viewHeight;
    int wheelDeltaValue = event->angleDelta().y();
    // 向上滚动，放大;
    if (wheelDeltaValue > 0)
    {
        if(m_scale>=1.728) return;
        this->scale(1.2, 1.2);
        m_scale*=1.2;
    }
    // 向下滚动，缩小;
    else
    {
        if(m_scale<=0.482) return;
        this->scale(1.0 / 1.2, 1.0 / 1.2);
        m_scale/=1.2;
    }
    // 将scene坐标转换为放大缩小后的坐标;
    QPointF viewPoint = this->transform().map(scenePos);
    // 通过滚动条控制view放大缩小后的展示scene的位置;
    horizontalScrollBar()->setValue(int(viewPoint.x() - viewWidth * hScale));
    verticalScrollBar()->setValue(int(viewPoint.y() - viewHeight * vScale));
}
MapWin::MapWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapWin)
{
    ui->setupUi(this);
    begin=-1;
    mv=new MapView(this);
    bg=new ButtonGroup(this);
    mp=new map(QString("map.json"));
    auto sce=new QGraphicsScene(mv);

    mv->setScene(sce);
    auto pic=new QPixmap("../picture/map2.png");
    auto qpi=sce->addPixmap(*pic);
    for(auto&e:mp->idtopos)
    {
        if(e.bx==-1||e.by==-1)
        {
            continue;
        }
        auto button=new MapButton(e);
        auto proxy=sce->addWidget(button);
        proxy->setScale(1.2);
        proxy->setPos(e.bx,e.by);
        proxy->setParentItem(qpi);
        bg->addButton(button,e.id);
    }
    bg->setExclusive(true);
    qsim=new QStandardItemModel(this);
    ui->listView->setModel(qsim);
}
void MapWin::on_pushButton_clicked()
{
    begin=this->bg->checkedId();
    if(begin==-1) return;
    ui->textBrowser->setPlainText(mp->idtopos[begin].name);
    this->bg->setExclusive(false);
    this->bg->button(begin)->setChecked(false);
    this->bg->setExclusive(true);
}
void MapWin::on_pushButton_2_clicked()
{
    int tmp=this->bg->checkedId();
    if(tmp==-1)return;
    end.push_back(tmp);
    std::sort(end.begin(),end.end());
    end.erase(std::unique(end.begin(),end.end()),end.end());
    QString now;
    for(auto&e:end)
    {
        now.append(mp->idtopos[e].name);
        now.append("\n");
    }
    ui->textBrowser_2->setPlainText(now);
    this->bg->setExclusive(false);
    this->bg->button(tmp)->setChecked(false);
    this->bg->setExclusive(true);
}
void MapWin::on_pushButton_3_clicked()
{
    this->end.clear();
    begin=-1;
    int tmp=this->bg->checkedId();
    if(tmp!=-1)
    {
        this->bg->setExclusive(false);
        this->bg->button(tmp)->setChecked(false);
        this->bg->setExclusive(true);
    }
    ui->textBrowser->setPlainText("");
    ui->textBrowser_2->setPlainText("");
    while(this->head.size())
    {
        this->mv->scene()->removeItem(head.top());
        delete head.top();
        head.pop();
    }
    qsim->clear();
    while(this->tail.size())
    {
        delete tail.top();
        tail.pop();
    }
    while(this->itemhead.size())
    {
        //delete itemhead.top();
        itemhead.pop();
    }
    while(this->itemtail.size())
    {
        //delete itemtail.top();
        itemtail.pop();
    }
}
void MapWin::on_pushButton_4_clicked()
{

    if(begin==-1)
    {
        QMessageBox::warning(this, tr("失败！"), tr("出发点未确定，请重试！"), QMessageBox::Ok);
        return;
    }
    if(!end.size())
    {
        QMessageBox::warning(this, tr("失败！"), tr("目的地未确定，请重试！"), QMessageBox::Ok);
        return;
    }
    while(this->head.size())
    {
        this->mv->scene()->removeItem(head.top());
        delete head.top();
        head.pop();
    }
    while(this->tail.size())
    {
        delete tail.top();
        tail.pop();
    }
    path pth;
    if(end.size()==1)
    {
        pth=mp->route(mp->idtopos[begin],mp->idtopos[end[0]]);
    }else
    {
        std::vector<position>need;
        for(auto&e:end)
        {
            need.push_back(mp->idtopos[e]);
        }
        pth=mp->route(mp->idtopos[begin],need);
    }
    //ui->textBrowser_3->setPlainText(pth.output(*mp));
    QStringList strlist;
    for(auto&e:pth.outputvec(*this->mp))
    {
        strlist.append(e);
        QStandardItem*item=new QStandardItem(e);
        this->itemhead.push(item);
        qsim->appendRow(item);
    }
    int nowx,nowy;
    nowx=mp->idtopos[begin].x;
    nowy=mp->idtopos[begin].y;
    QPen qp(QColor(238 ,99, 99));
    qp.setWidth(7);
    qp.setCapStyle(Qt::RoundCap);
    qp.setJoinStyle(Qt::MiterJoin);
    for(int i=1;i<pth.size();i++)
    {
        /*
        if(pth[i].x==-1||pth[i].y==-1||nowx==-1||nowy==-1)
        {
            nowx=pth[i].x;
            nowy=pth[i].y;
            continue;
        }
        */
        //已经不存在权值为0的边了
        QGraphicsLineItem* line=new QGraphicsLineItem(qreal(nowx),qreal(nowy),qreal(pth[i].x),qreal(pth[i].y));
        line->setPen(qp);
        //this->mv->scene()->addItem(line);
        this->head.push(line);
        nowx=pth[i].x;
        nowy=pth[i].y;
    }
    while(this->head.size())
    {
        this->tail.push(this->head.top());
        this->head.pop();
    }
    while(this->itemhead.size())
    {
        this->itemtail.push(this->itemhead.top());
        this->itemhead.pop();
    }
    this->bg->setExclusive(true);
}
void MapWin::on_pushButton_5_clicked()
{
    if(this->tail.empty())
    {
        QMessageBox::warning(this, tr("失败！"), tr("路线已导航完毕！"), QMessageBox::Ok);
        return;
    }
    this->mv->scene()->addItem(this->tail.top());
    this->head.push(this->tail.top());
    this->tail.pop();
    ui->listView->setCurrentIndex(this->qsim->indexFromItem(this->itemtail.top()));
    this->itemhead.push(this->itemtail.top());
    this->itemtail.pop();
}
void MapWin::on_pushButton_6_clicked()
{
    if(this->head.empty())
    {
        return;
    }
    this->mv->scene()->removeItem(this->head.top());
    this->tail.push(this->head.top());
    this->head.pop();
    ui->listView->setCurrentIndex(this->qsim->indexFromItem(this->itemhead.top()));
    this->itemtail.push(this->itemhead.top());
    this->itemhead.pop();
}
MapWin::~MapWin()
{
    delete ui;
}
