#include "affairwin.h"
#include "ui_affairwin.h"
#include <QJsonObject>
#include "basic.h"
#include "online_data.h"
#include "addactivity.h"
#include <QMessageBox>

AffairWin::AffairWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AffairWin)
{
    ui->setupUi(this);
    load();
}

AffairWin::~AffairWin()
{
    delete ui;
}

void AffairWin::on_comboBox_currentIndexChanged(int index)
{
    QJsonObject stuobject;
    open_json(QString::number(user_online->get_id())+".json",stuobject);
    QJsonArray actvtarray=stuobject["activities"].toArray();
    if(index==0){
        if(actvtarray.isEmpty()){
            ui->activitylist->clear();
            ui->activitylist->addItem("当前无活动");
        }
        else{
            ui->activitylist->clear();
            for(int i=0;i<actvtarray.size();i++){
                QJsonObject activity=actvtarray.at(i).toObject();
                int time=activity["time"].toInt();
                QString timestr=QString::number(time)+":00-"+QString::number(time+1)+":00";
                int placeid=activity["destination_id"].toInt();
                QString place=school_online->idtopos[placeid].name;
                QString data=activity["name"].toString()+"  "+num_to_qstr(activity["day"].toInt())+"  "+timestr+"  "+place;
                ui->activitylist->addItem(data);
            }
        }
    }
    else{

    }
}
void AffairWin::load(){
    on_comboBox_currentIndexChanged(0);
}

void AffairWin::on_addactvt_clicked()
{
    addactivity* addwind = new addactivity(this);
    connect(addwind,SIGNAL(flash()),this,SLOT(load()));
    addwind->setWindowTitle("新增活动");
    addwind->show();
    addwind->setAttribute(Qt::WA_DeleteOnClose);
}


void AffairWin::on_deleteactvt_clicked()
{
    QString data = ui->activitylist->currentItem()->text();
    QStringList strlist=data.split("  ");

    QString name=strlist[0];
    int day=qstr_to_num(strlist[1]);
    int st,ed;qstr_to_time(strlist[2],st,ed);
    if(name!=""){
        QMessageBox::StandardButton button;
        button=QMessageBox::question(this,tr("删除活动"),"确认删除活动：\n"+name,QMessageBox::Yes|QMessageBox::No);
        if(button == QMessageBox::Yes){
            if(user_online->del_activity(name,day,st)){
                    QMessageBox::information(this, "提示", "删除成功");
                    load();
                }
                else{
                    QMessageBox::information(this, "提示", "删除失败，请重新尝试");
                }
        }
        else if(button == QMessageBox::No){
        }
    }
    else{
        QMessageBox::information(this, "提示", "未选中活动，请重新选择");
    }
}


void AffairWin::on_querytime_clicked()
{
    ui->checktimelist->clear();
    std::vector<QString> result=user_online->query_time();
    for(int i=0;i<result.size();i++){
        ui->checktimelist->addItem(result[i]);
    }
}

