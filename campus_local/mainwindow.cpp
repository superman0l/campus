#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timer.h"
#include <QMessageBox>
#include <QLabel>
#include <QCloseEvent>
#include <QTimer>
#include <QDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化时间
    tim = new timer();
    //初始化并美化时间标签
    timlabl = new QLabel(this);//显示时间的标签
    timlabl->setFrameShape(QLabel::WinPanel);
    timlabl->setFrameShadow(QLabel::Sunken);
    timlabl->setText(tim->toString("yyyy年MM月dd日 hh:mm"));
    ui->statusbar->addWidget(timlabl);
    //设置一个计时器
    QTimer *UpgraCount = new QTimer(this);//用于设置刷新时间
    UpgraCount->start(1000);//每秒刷新一次
    //连接倒计时与时间数据更新
    connect(UpgraCount, SIGNAL(timeout()), this, SLOT(timShowNewest()));

    //在状态栏显示项目地址信息
    QLabel *perm = new QLabel(this);
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

void MainWindow::ReiceiveUser(const User * data)
{
    per = data;
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

