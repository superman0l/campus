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
    load(0,0,1);
}

AffairWin::~AffairWin()
{
    delete ui;
}

void AffairWin::on_comboBox_currentIndexChanged(int index)
{
    int a=ui->timesort->text()=="按时间升序"?1:2;
    load(index,ui->tags->currentIndex(),a);

}

void AffairWin::load(int day,int tag, int sorttype){
    ui->comboBox->setCurrentIndex(day);
    ui->tags->setCurrentIndex(tag);
    if(sorttype==1)
        ui->timesort->setText("按时间升序");
    else
        ui->timesort->setText("按时间降序");
    QJsonObject stuobject;
    open_json(QString::number(user_online->get_id())+".json",stuobject);
    QJsonArray actvtarray=stuobject["activities"].toArray();
    if(day==0&&tag==0){
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
                if(activity["platform"].toString()!=""&&activity["url"].toString()!="")
                    data+="  "+activity["platform"].toString()+"  "+activity["url"].toString();

                listwidgetItem* item=new listwidgetItem();
                item->setText(data);
                item->setSortType(sorttype);
                ui->activitylist->addItem(item);
            }
        }
    }
    else if(day==0&&tag!=0){
        ui->activitylist->clear();
        bool isempty=1;
        for(int i=0;i<actvtarray.size();i++){
            QJsonObject activity=actvtarray.at(i).toObject();
            if(activity["tag"].toInt()!=tag)continue;
            int time=activity["time"].toInt();
            QString timestr=QString::number(time)+":00-"+QString::number(time+1)+":00";
            int placeid=activity["destination_id"].toInt();
            QString place=school_online->idtopos[placeid].name;
            QString data=activity["name"].toString()+"  "+num_to_qstr(activity["day"].toInt())+"  "+timestr+"  "+place;
            if(activity["platform"].toString()!=""&&activity["url"].toString()!="")
                data+="  "+activity["platform"].toString()+"  "+activity["url"].toString();
            isempty=0;

            listwidgetItem* item=new listwidgetItem();
            item->setText(data);
            item->setSortType(sorttype);
            ui->activitylist->addItem(item);
        }
        if(isempty)
            ui->activitylist->addItem("当前日期和类型无活动");
    }
    else if(day!=0&&tag==0){
        ui->activitylist->clear();
        bool isempty=1;
        for(int i=0;i<actvtarray.size();i++){
            QJsonObject activity=actvtarray.at(i).toObject();
            if(activity["day"].toInt()!=day)continue;
            int time=activity["time"].toInt();
            QString timestr=QString::number(time)+":00-"+QString::number(time+1)+":00";
            int placeid=activity["destination_id"].toInt();
            QString place=school_online->idtopos[placeid].name;
            QString data=activity["name"].toString()+"  "+num_to_qstr(activity["day"].toInt())+"  "+timestr+"  "+place;
            if(activity["platform"].toString()!=""&&activity["url"].toString()!="")
                data+="  "+activity["platform"].toString()+"  "+activity["url"].toString();
            isempty=0;

            listwidgetItem* item=new listwidgetItem();
            item->setText(data);
            item->setSortType(sorttype);
            ui->activitylist->addItem(item);
        }
        if(isempty)
            ui->activitylist->addItem("当前日期和类型无活动");
    }
    else{
        ui->activitylist->clear();
        bool isempty=1;
        for(int i=0;i<actvtarray.size();i++){
            QJsonObject activity=actvtarray.at(i).toObject();
            if(activity["day"].toInt()!=day||activity["tag"].toInt()!=tag)continue;
            int time=activity["time"].toInt();
            QString timestr=QString::number(time)+":00-"+QString::number(time+1)+":00";
            int placeid=activity["destination_id"].toInt();
            QString place=school_online->idtopos[placeid].name;
            QString data=activity["name"].toString()+"  "+num_to_qstr(activity["day"].toInt())+"  "+timestr+"  "+place;
            if(activity["platform"].toString()!=""&&activity["url"].toString()!="")
                data+="  "+activity["platform"].toString()+"  "+activity["url"].toString();
            isempty=0;

            listwidgetItem* item=new listwidgetItem();
            item->setText(data);
            item->setSortType(sorttype);
            ui->activitylist->addItem(item);
        }
        if(isempty)
            ui->activitylist->addItem("当前日期和类型无活动");
    }
    ui->activitylist->sortItems();
}

void AffairWin::on_addactvt_clicked()
{
    addactivity* addwind = new addactivity(this);
    connect(addwind,SIGNAL(flash(int,int,int)),this,SLOT(load(int,int,int)));
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
                int a=ui->timesort->text()=="按时间升序"?1:2;
                load(ui->comboBox->currentIndex(),ui->tags->currentIndex(),a);
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
    std::vector<QString> result=user_online->query_time(ui->comboBox->currentIndex());
    for(int i=0;i<result.size();i++){
        ui->checktimelist->addItem(result[i]);
    }
}


void AffairWin::on_tags_currentIndexChanged(int index)
{
    int a=ui->timesort->text()=="按时间升序"?1:2;
    load(ui->comboBox->currentIndex(),index,a);
}


void AffairWin::on_timesort_clicked()
{


    if(ui->timesort->text()=="按时间升序"){
        ui->timesort->setText("按时间降序");
        load(ui->comboBox->currentIndex(),ui->tags->currentIndex(),2);
    }
    else{
        ui->timesort->setText("按时间升序");
        load(ui->comboBox->currentIndex(),ui->tags->currentIndex(),1);
    }
}


bool listwidgetItem::operator<(const QListWidgetItem &other) const
{
    if(mType!=0){
        QString a, b;
        a=this->text();
        b=other.text();
        QStringList alist=a.split("  "),blist=b.split("  ");
        int daya=qstr_to_num(alist[1]),dayb=qstr_to_num(blist[1]);
        if(daya!=dayb){
            if(mType==1)return daya < dayb;
            else return daya > dayb;
        }
        else{
            int sta,stb,eda,edb;
            qstr_to_time(alist[2],sta,eda);qstr_to_time(blist[2],stb,edb);
            if(mType==1)return sta < stb;
            else return sta > stb;
        }
    }
    else{
        return QListWidgetItem::operator<(other);
    }
}
