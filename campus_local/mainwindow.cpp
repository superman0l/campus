#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timer.h"
#include <QMessageBox>
#include <QLabel>
#include <QCloseEvent>
#include <QTimer>
#include <QDialog>
#include <QInputDialog>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QGridLayout.h>
#include "CustomTabStyle.h"
#include "classwind.h"
#include "mapwin.h"
#include "affairwin.h"
#include "personwin.h"
#include "online_data.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("学生端"));

    //初始化时间
    tim = new timer();

    //建立切换页面
    QTabWidget* tab = new QTabWidget(this);
    QWidget* Classes = new classwind(tab);
    QWidget* Map = new MapWin(tab);
    QWidget* Affair = new AffairWin(tab);
    QWidget* person = new PersonWin(tab);
    //tab->addTab(Classes, "关闭");
    tab->addTab(Classes, "课表");
    tab->addTab(Map, "导航");
    tab->addTab(Affair, "事务");
    tab->addTab(person, "个人");
    tab->setTabPosition(QTabWidget::West);//QTabWidget竖向
    tab->tabBar()->setStyle(new CustomTabStyle);//注意，设置上述代码风格 就可以实现QTabBar横向
    setCentralWidget(tab);

    //实现信息传递
        connect(person, SIGNAL(DestroyCount()), this, SLOT(DestroyCount()));

    //初始化并美化时间标签
    timlabl = new QLabel(this);//显示时间的标签
    timlabl->setMinimumSize(QSize(250, 40));
    timlabl->setFont(QFont("宋体", 14));
    timlabl->setFrameShape(QLabel::WinPanel);
    timlabl->setFrameShadow(QLabel::Sunken);
    timlabl->setText(tim->toString("yyyy年MM月dd日 hh:mm"));
    ui->statusbar->addWidget(timlabl);
    //设置一个计时器
    QTimer *UpgraCount = new QTimer(this);//用于设置刷新时间
    UpgraCount->start(1000);//每秒刷新一次
    //连接倒计时与时间数据更新
    connect(UpgraCount, SIGNAL(timeout()), this, SLOT(timShowNewest()));

    //在状态栏添加一个暂停时间和一个更改时间的按钮
    QPushButton* pasbtn = new QPushButton("暂停时间", this);
    pasbtn->setMinimumSize(QSize(50, 40));
    QPushButton* setbtn = new QPushButton("设置时间", this);
    setbtn->setMinimumSize(QSize(50, 40));
    ui->statusbar->addPermanentWidget(pasbtn, 8);
    ui->statusbar->addPermanentWidget(setbtn, 9);
    connect(pasbtn, &QPushButton::clicked, this, &MainWindow::on_actionPauseTim_P_triggered);
    connect(setbtn, &QPushButton::clicked, this, &MainWindow::on_actionsettime_S_triggered);

    //在状态栏显示项目地址信息
    QLabel *perm = new QLabel(this);
    perm->setMinimumSize(QSize(0, 30));
    perm->setFont(QFont("楷体", 14));
    perm->setFrameStyle(QFrame::Box|QFrame::Sunken);
    perm->setText(tr("<a href=\"https://live.bilibili.com/22603245?live_from=85001&spm_id_from=444.41.live_users.item.click\">项目源码地址<a>"));
    perm->setTextFormat(Qt::RichText);
    perm->setOpenExternalLinks(true);
    ui->statusbar->addPermanentWidget(perm);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //重载关闭本窗口的事件
    //定义一个警告对话框
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("退出程序"),QString(tr("确认退出程序")),QMessageBox::Yes|QMessageBox::No);
    if(button == QMessageBox::Yes){
        //退出登录时，存储用户placeid信息,建议改到User的析构函数里
        QJsonObject obj;
        open_json(QString::number(user_online->get_id()) + ".json", obj);
        obj["place_id"] = user_online->get_place_id();
        write_json(QString::number(user_online->get_id()) + ".json", obj);
        qDebug() << "Saved";
        qApp->quit();
    }
    else if(button == QMessageBox::No){
        event->ignore();
    }
}
void MainWindow::on_action_S_triggered()
{
    //实现学生端切换用户的功能
    emit LogOut();
    this->hide();
}


void MainWindow::timShowNewest()
{
    tim->update();
    timlabl->setText(tim->toString("yyyy年MM月dd日 hh:mm"));
}

void MainWindow::on_actionBeginTim_B_triggered()
{
    tim->begin();
}


void MainWindow::on_actionPauseTim_P_triggered()
{
    tim->puase();
}


void MainWindow::on_actionChgTimRa_T_triggered()
{
    bool ok;
    int raio = QInputDialog::getInt(this, tr("调整时间倍率"), tr("请设置\n   程序1秒钟/现实1分钟\n的比值"), 1, 0, 60, 1, &ok);
    if(ok)
        tim->set_ratio(raio*60);
}


void MainWindow::DestroyCount()
{
    on_action_S_triggered();
}

void MainWindow::on_actionsettime_S_triggered()
{
    //暂停时间
    on_actionPauseTim_P_triggered();
    //初始化一个改变时间的窗口
    QDialog* TimeGeter = new QDialog(this, Qt::FramelessWindowHint);
    TimeGeter->setMinimumSize(QSize(500,250));
    TimeGeter->setWindowTitle("设置时间");
    QGridLayout* gridLay = new QGridLayout(TimeGeter);
    //qDebug() << tim->toString("yyyyMMdd") << tim->toString("hhmm");
    QDateTimeEdit* dataedit = new QDateTimeEdit(QDate::fromString(tim->toString("yyyyMMdd"), "yyyyMMdd"), TimeGeter);
    dataedit->setMinimumSize(QSize(250, 100));
    dataedit->setFont(QFont("楷体", 14));
    dataedit->setCalendarPopup(true);
    QDateTimeEdit* timeedit = new QDateTimeEdit(QTime::fromString(tim->toString("hhmm"), "hhmm"), TimeGeter);
    timeedit->setMinimumSize(QSize(250, 100));
    timeedit->setFont(QFont("楷体", 14));
    QPushButton* okbut = new QPushButton("确认", TimeGeter);
    okbut->setMinimumSize(QSize(125, 50));
    gridLay->addWidget(dataedit);
    gridLay->addWidget(timeedit);
    gridLay->addWidget(okbut);
    TimeGeter->show();
    //实现按钮与窗口之间的逻辑
    connect(dataedit, &QDateTimeEdit::dateChanged, this, &MainWindow::DateChanged);
    connect(timeedit, &QDateTimeEdit::timeChanged, this, &MainWindow::TimeChanged);
    connect(okbut, &QPushButton::clicked, TimeGeter, &QDialog::close);
    connect(okbut, &QPushButton::clicked, this, &MainWindow::on_actionBeginTim_B_triggered);
}

void MainWindow::DateChanged(QDate date)
{
    //更新日期
    tim->set_Date(date);
}

void MainWindow::TimeChanged(QTime time)
{
    //更新时间
    tim->set_Time(time);
}
