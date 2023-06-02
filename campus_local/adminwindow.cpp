#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "QCloseEvent"
#include "person.h"//多重定义问题还没解决
#include "online_data.h"
//已添加person.cpp
#include <QMessageBox>
#include <QStandardItem>
#include <QAbstractItemView>

AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("../icon.png"));
    this->setWindowTitle("管理端");
    tim=new timer();
    logvar.init(admin_online);
    log_action(QString("登录!"));
    model->setHorizontalHeaderLabels({"周一", "周二", "周三", "周四", "周五", "周六", "周日"});
    model->setVerticalHeaderLabels({"8:00-9:00", "9:00-10:00", "10:00-11:00", "11:00-12:00", "休息", "13:00-14:00", "14:00-15:00", "15:00-16:00", "16:00-17:00", "17:00-18:00", "休息", "19:00-20:00", "20:00-21:00"});
    ui->comboBox->setCurrentIndex(tim->get_week()-1);
    load(tim->get_week());
    ui->tableView->setModel(model);
    for(int i=0;i<13;i++){
        if(i==4 || i==10)
            ui->tableView->setRowHeight(i,20);
        else ui->tableView->setRowHeight(i,180);
    }
    for(int i=0;i<7;i++)ui->tableView->setColumnWidth(i,80);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->admin_id->setText(ui->admin_id->text()+QObject::tr("<font color = blue>%1</font>").arg(QString::number(admin_online->get_id())));
    ui->admin_class->setText(ui->admin_class->text()+QObject::tr("<font color = blue>%1</font>").arg(QString::number(admin_online->get_classid())));
}

AdminWindow::~AdminWindow()
{
    delete tim;
    delete ui;
}

void AdminWindow::load(int weeknum)
{
    model->clear();
    QJsonArray courseArray=load_student_class_coursearray(QString::number(admin_online->get_id()));
    QString name,teacher,classroom;
    int starttime,period,day;
    int stweek,edweek;
    for(int i=0;i<courseArray.size();i++){
        QJsonObject course=courseArray.at(i).toObject();
        QString message;
        stweek=course["startweek"].toInt();
        edweek=course["endweek"].toInt();
        starttime=course["starttime"].toInt();
        day=course["weekday"].toInt();
        period=course["endtime"].toInt()-starttime+1;

        if(weeknum<stweek||weeknum>edweek){
            for(int j=0;j<period;j++){
                model->setItem(starttime-8+j,day-1,new QStandardItem(""));
            }
        }
        else{
            name=course["name"].toString();
            teacher=course["teacher"].toString();
            classroom=course["classroom"].toString();

            message=name+"\n\n"+teacher+"\n\n"+classroom+"\n\n"+QString::number(starttime)+":00"+"-"+QString::number(starttime+period)+":00";
            for(int j=0;j<period;j++){
                model->setItem(starttime-8+j,day-1,new QStandardItem(message));
                model->item(starttime-8+j,day-1)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}

void AdminWindow::closeEvent(QCloseEvent *event)
{
    //重载关闭本窗口的事件
    //定义一个警告对话框
        QMessageBox::StandardButton button;
        button=QMessageBox::question(this,tr("退出管理员界面"),QString(tr("确认退出管理员界面？这将退出整个程序！")),QMessageBox::Yes|QMessageBox::No);
        if(button == QMessageBox::Yes){
            QFile flagfile("../"+QString::number(admin_online->get_classid())+"_busy");
            flagfile.remove();
            delete tim;
            delete admin_online;
            qApp->quit();
        }
        else if(button == QMessageBox::No){
            event->ignore();
        }
}

void AdminWindow::ReceiveAdmData(const Admin * data)
{
    adm = data;//获取管理员信息数据
    this->show();//展示本页面
}

void AdminWindow::on_actionLogOut_O_triggered()
{
    emit LogOut();
    this->hide();
}


void AdminWindow::on_set_clicked()
{
    QModelIndex indx = ui->tableView->currentIndex();
    QString show = model->data(indx).toString();
    if(show==""){
        QMessageBox::information(this, "提示", "未选中课程，请重新选择");
        return;
    }
    setcourse* setwind = new setcourse(this);
    connect(setwind,SIGNAL(change()),this,SLOT(fresh()));
    connect(this,SIGNAL(sendsetData(QModelIndex, QString)),setwind,SLOT(receivesetData(QModelIndex, QString)));
    emit sendsetData(indx, show);
    setwind->setWindowTitle("修改课程");
    setwind->show();
    setwind->setAttribute(Qt::WA_DeleteOnClose);
}


void AdminWindow::on_add_clicked()
{
    addcourse* addwind = new addcourse(this);
    connect(addwind,SIGNAL(change()),this,SLOT(fresh()));
    addwind->setWindowTitle("新增课程");
    addwind->show();
    addwind->setAttribute(Qt::WA_DeleteOnClose);

}


void AdminWindow::on_delete_2_clicked()
{

    QModelIndex indx = ui->tableView->currentIndex();
    QVariant data = model->data(indx);
    QString show = data.toString();
    QStringList strlist=show.split("\n\n");
    int col=indx.column()+1;

    QString coursename=strlist[0];
    if(coursename!=""){
        QMessageBox::StandardButton button;
        button=QMessageBox::question(this,tr("删除课程"),"确认删除课程：\n"+coursename,QMessageBox::Yes|QMessageBox::No);
        if(button == QMessageBox::Yes){
                if(admin_online->erase_course(coursename,col,admin_online->get_classid())){
                    QMessageBox::information(this, "提示", "删除成功");
                    fresh();
                }
                else{
                    QMessageBox::information(this, "提示", "删除失败，请重新尝试");
                }
        }
        else if(button == QMessageBox::No){
        }
    }
    else{
        QMessageBox::information(this, "提示", "未选中课程，请重新选择");
    }
}


void AdminWindow::on_set_quiz_clicked()
{
    addexam* addwind = new addexam(this);
    connect(addwind,SIGNAL(change()),this,SLOT(fresh()));
    addwind->setWindowTitle("新增课程");
    addwind->show();
    addwind->setAttribute(Qt::WA_DeleteOnClose);
}


void AdminWindow::on_comboBox_currentIndexChanged(int index)
{
    load(index+1);
    ui->tableView->setModel(model);
    for(int i=0;i<13;i++){
            if(i==4 || i==10)
            ui->tableView->setRowHeight(i,20);
            else ui->tableView->setRowHeight(i,150);
    }
    for(int i=0;i<7;i++)ui->tableView->setColumnWidth(i,80);
}

void AdminWindow::fresh()
{
    load(ui->comboBox->currentIndex()+1);
    ui->tableView->setModel(model);
    for(int i=0;i<13;i++){
            if(i==4 || i==10)
            ui->tableView->setRowHeight(i,20);
            else ui->tableView->setRowHeight(i,150);
    }
    for(int i=0;i<7;i++)ui->tableView->setColumnWidth(i,80);
}

