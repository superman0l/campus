#include "affairwin.h"
#include "ui_affairwin.h"
#include <QJsonObject>
#include "basic.h"
#include "online_data.h"
#include "addactivity.h"
#include "addtmpaffair.h"
#include <QMessageBox>

AffairWin::AffairWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AffairWin)
{
    ui->setupUi(this);
    tim->load_affair=false;
    load(0,0,1);
    load_affair(0, 1);
    tim->load_affair=true;//初次加载闹钟
    ui->activitylist->setCurrentRow(0);
    ui->tmpaffairlist->setCurrentRow(0);
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
                QJsonObject alarm=activity["alarm"].toObject();
                if(!tim->load_affair&&!alarm.isEmpty()&&alarm["enable"].toBool())
                {
                    talarm tmp;
                    tmp.day=alarm["day"].toInt();
                    tmp.hour=alarm["hour"].toInt();
                    tmp.minute=alarm["minute"].toInt();
                    tmp.periodicity=alarm["minute"].toInt();
                    if(placeid==-1)
                    {
                        tmp.info=QString("活动：%1\n活动平台：%2\n活动链接：%3").arg(activity["name"].toString()).arg(activity["platform"].toString()).arg(activity["url"].toString());
                    }else
                    {
                        tmp.info=QString("活动：%1\n地点：%2\n导航路径：%3").arg(activity["name"].toString()).arg(activity["platform"].toString()).arg(school_online->navigate(user_online->get_place_id(),placeid));
                    }
                    if(tmp.day==0)
                    {
                        for(int i=1;i<=7;i++)
                        {
                            tim->insert(talarm(i,tmp.hour,tmp.minute,(1<<(i-1)),tmp.info));
                        }
                    }else
                    {
                        tim->insert(tmp);
                    }
                }
                listwidgetItem* item=new listwidgetItem();
                bool isavailable=1;//失效活动处理
                if(activity["day"].toInt()==0){
                    for(int i=0;i<7;i++)
                        if(timetable_online[i][time-6]>=4)
                            isavailable=0;
                }
                else if(timetable_online[activity["day"].toInt()-1][time-6]>=4)isavailable=0;
                if(!isavailable){
                    item->setBackground(QColor(255,150,30));
                    data+="（已失效）";

                    //timetable update
                    if(activity["day"].toInt()==0){
                        for(int i=0;i<7;i++)
                            timetable_online[i][time-6]&=~2;
                    }
                    else timetable_online[activity["day"].toInt()-1][time-6]&=~2;

                    actvtarray.removeAt(i);stuobject["activities"]=actvtarray;i--;
                    stuobject["timetable"]=timetable_to_array();

                }
                item->setText(data);
                item->setSortType(sorttype);
                ui->activitylist->addItem(item);
            }
            write_json(QString::number(user_online->get_id())+".json",stuobject);
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
    connect(addwind,SIGNAL(flash_2(int,int)),this,SLOT(load_affair(int,int)));
    addwind->setWindowTitle("新增活动");
    addwind->show();
    addwind->setAttribute(Qt::WA_DeleteOnClose);
}


void AffairWin::on_deleteactvt_clicked()
{
    if(ui->activitylist->currentItem()->background()==QColor(255,150,30)){
        load(0,0,1);
        return;
    }
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
                    log_action(tr("删除活动%1成功").arg(name));
                    QMessageBox::information(this, "提示", "删除成功");
                int a=ui->timesort->text()=="按时间升序"?1:2;
                load(ui->comboBox->currentIndex(),ui->tags->currentIndex(),a);
                }
                else{

                log_action(tr("删除活动%1失败").arg(name));
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
    log_action(tr("查询事务可行时间"));
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
    if(mType==1||mType==2){
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
    else if(mType==3){
        QString a, b;
        a=this->text();
        b=other.text();
        QStringList alist=a.split("  "),blist=b.split("  ");
        int sta,stb,eda,edb;
        qstr_to_time(alist[1],sta,eda);qstr_to_time(blist[1],stb,edb);
        return sta < stb;
    }
    else{
        return QListWidgetItem::operator<(other);
    }
}


void AffairWin::load_affair(int tag, int sorttype){
    ui->aff_tags->setCurrentIndex(tag);
    if(sorttype==1)
        ui->aff_timesort->setText("按时间升序");
    else
        ui->aff_timesort->setText("按时间降序");
    QJsonObject stuobject;
    open_json(QString::number(user_online->get_id())+".json",stuobject);
    QJsonArray array=stuobject["affairs"].toArray();
    ui->tmpaffairlist->clear();
    if(tag==0){
        if(array.isEmpty()){
            ui->tmpaffairlist->addItem("当前无事务");
            tim->has_affair=false;
        }
        else{
            tim->has_affair=true;
            for(int i=0;i<array.size();i++){
                    QJsonObject tmp=array.at(i).toObject();
                    int time=tmp["time"].toInt();
                    QString timestr=QString::number(time)+":00-"+QString::number(time+1)+":00";
                    int placeid=tmp["destination_id"].toInt();
                    QString place=school_online->idtopos[placeid].name;
                    QString data=tmp["name"].toString()+"  "+num_to_qstr(tmp["day"].toInt())+"  "+timestr+"  "+place;
                    QJsonObject alarm=tmp["alarm"].toObject();
                    if(!alarm.isEmpty()&&alarm["enable"].toBool())
                    {
                        talarm tmpalarm;
                        tmpalarm.day=tim->get_days();
                        tmpalarm.hour=alarm["hour"].toInt();
                        tmpalarm.minute=alarm["minute"].toInt();
                        tmpalarm.periodicity=0;
                        tmpalarm.info=QString("事务：%1\n地点：%2\n导航路径：%3").arg(tmp["name"].toString()).arg(place).arg(school_online->navigate(user_online->get_place_id(),placeid));
                        if(!tim->load_affair)
                        {
                            tim->insert(tmpalarm);
                        }else if(tim->del_affair)
                        {
                            tim->erase(tmpalarm);
                        }
                    }
                    listwidgetItem* item=new listwidgetItem();

                    //无效事务处理
                    bool isavailable=1;
                    if(timetable_online[tmp["day"].toInt()-1][time-6]>=2)isavailable=0;
                    if(!isavailable){
                        item->setBackground(QColor(255,150,30));
                        data+="（已失效）";

                        //timetable update
                        timetable_online[tmp["day"].toInt()-1][time-6]&=~1;

                        array.removeAt(i);stuobject["affairs"]=array;i--;
                        stuobject["timetable"]=timetable_to_array();

                    }
                    item->setText(data);
                    item->setSortType(sorttype);
                    ui->tmpaffairlist->addItem(item);
            }
            write_json(QString::number(user_online->get_id())+".json",stuobject);
        }
    }
    else{
        bool isempty=1;
        for(int i=0;i<array.size();i++){
            QJsonObject tmp=array.at(i).toObject();
            if(tmp["tag"].toInt()!=tag)continue;
            int time=tmp["time"].toInt();
            QString timestr=QString::number(time)+":00-"+QString::number(time+1)+":00";
            int placeid=tmp["destination_id"].toInt();
            QString place=school_online->idtopos[placeid].name;
            QString data=tmp["name"].toString()+"  "+num_to_qstr(tmp["day"].toInt())+"  "+timestr+"  "+place;
            isempty=0;

            listwidgetItem* item=new listwidgetItem();
            item->setText(data);
            item->setSortType(sorttype);
            ui->tmpaffairlist->addItem(item);
        }
        if(isempty)
            ui->tmpaffairlist->addItem("当前类型无事务");
    }
    ui->tmpaffairlist->sortItems();
}

void AffairWin::on_aff_timesort_clicked()
{
    if(ui->aff_timesort->text()=="按时间升序"){
        ui->aff_timesort->setText("按时间降序");
        load_affair(ui->aff_tags->currentIndex(),2);
    }
    else{
        ui->aff_timesort->setText("按时间升序");
        load_affair(ui->aff_tags->currentIndex(),1);
    }
}


void AffairWin::on_aff_tags_currentIndexChanged(int index)
{
    int s;
    if(ui->aff_timesort->text()=="按时间升序")s=1;
    else s=2;
    load_affair(index,s);
}


void AffairWin::on_addtmpaffair_clicked()
{
    addtmpaffair* addwind = new addtmpaffair(this);
    connect(addwind,SIGNAL(flash(int,int)),this,SLOT(load_affair(int,int)));
    addwind->setWindowTitle("新增临时事务");
    addwind->show();
    addwind->setAttribute(Qt::WA_DeleteOnClose);
}


void AffairWin::on_deltmpaffair_clicked()
{
    if(ui->tmpaffairlist->currentItem()->background()==QColor(255,150,30)){
        load_affair(0,1);
    }

    QString data = ui->tmpaffairlist->currentItem()->text();
    QStringList strlist=data.split("  ");

    QString name=strlist[0];
    int st,ed;qstr_to_time(strlist[2],st,ed);
    if(name!=""){
        QMessageBox::StandardButton button;
        button=QMessageBox::question(this,tr("删除事务"),"确认删除事务：\n"+name,QMessageBox::Yes|QMessageBox::No);
        if(button == QMessageBox::Yes){
            if(user_online->del_tmpaffair(name,st)){
                    QMessageBox::information(this, "提示", "删除成功");
                    log_action(tr("删除临时事务%1成功").arg(name));
                int a=ui->timesort->text()=="按时间升序"?1:2;
                load_affair(ui->aff_tags->currentIndex(),a);
                }
                else{
                    log_action(tr("删除临时事务%1失败").arg(name));
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


void AffairWin::on_queryta_clicked()
{
    log_action("查询同时临时事务");
    ui->checkafrlist->clear();
    queryaffairresult=user_online->query_tmpaffair();
    if(queryaffairresult.empty()){
        ui->checkafrlist->addItem("无多个同时临时活动");
    }
    else{
        f=0;
        ui->pre->setEnabled(false);
        if(f!=queryaffairresult.size()-1)ui->next->setEnabled(true);
        for(int i=0;i<queryaffairresult[f].size();i++){
                QString name=queryaffairresult[f][i].name;
                QString day=num_to_qstr(queryaffairresult[f][i].day);
                QString timestr=QString::number(queryaffairresult[f][i].start)+":00-"+QString::number(queryaffairresult[f][i].end)+":00";
                QString place=queryaffairresult[f][i].place.name;
                QString data=name+"  "+timestr+"  "+place;

                listwidgetItem* item=new listwidgetItem();
                item->setText(data);
                item->setSortType(3);
                ui->checkafrlist->addItem(item);
        }
        ui->checkafrlist->sortItems();
    }
}


void AffairWin::on_next_clicked()
{
    f++;
    if(f==queryaffairresult.size()-1)ui->next->setEnabled(false);
    if(f!=0)ui->pre->setEnabled(true);
    ui->checkafrlist->clear();
    for(int i=0;i<queryaffairresult[f].size();i++){
        QString name=queryaffairresult[f][i].name;
        QString day=num_to_qstr(queryaffairresult[f][i].day);
        QString timestr=QString::number(queryaffairresult[f][i].start)+":00-"+QString::number(queryaffairresult[f][i].end)+":00";
        QString place=queryaffairresult[f][i].place.name;
        QString data=name+"  "+timestr+"  "+place;

        listwidgetItem* item=new listwidgetItem();
        item->setText(data);
        item->setSortType(3);
        ui->checkafrlist->addItem(item);
    }
    ui->checkafrlist->sortItems();
}


void AffairWin::on_pre_clicked()
{
    f--;
    if(f==0)ui->pre->setEnabled(false);
    if(f!=queryaffairresult.size()-1)ui->next->setEnabled(true);
    ui->checkafrlist->clear();
    for(int i=0;i<queryaffairresult[f].size();i++){
        QString name=queryaffairresult[f][i].name;
        QString day=num_to_qstr(queryaffairresult[f][i].day);
        QString timestr=QString::number(queryaffairresult[f][i].start)+":00-"+QString::number(queryaffairresult[f][i].end)+":00";
        QString place=queryaffairresult[f][i].place.name;
        QString data=name+"  "+timestr+"  "+place;

        listwidgetItem* item=new listwidgetItem();
        item->setText(data);
        item->setSortType(3);
        ui->checkafrlist->addItem(item);
    }
    ui->checkafrlist->sortItems();
}


void AffairWin::on_navigate_clicked()
{
    int stid;
    stid=user_online->get_place_id();
    std::vector<int> edid;
    QString tmp1=school_online->idtopos[stid].name;
    QString tmp2;
    for(int i=0;i<queryaffairresult[f].size();i++){
        edid.push_back(queryaffairresult[f][i].place.id);
        tmp2.append(school_online->idtopos[edid[i]].name);
        tmp2.append(" ");
    }
    log_action(tr("查询从%1到%2的最佳路线").arg(tmp1).arg(tmp2));
    ui->naviresult->setText(school_online->navigate(stid,edid));
}


void AffairWin::on_aff_alarmcheck_stateChanged(int arg1)
{
    QString data=ui->tmpaffairlist->currentItem()->text();
    QStringList list=data.split("  ");
    QJsonObject user;
    open_json(QString::number(user_online->get_id())+".json",user);
    QJsonArray affairs=user["affairs"].toArray();
    for(int i=0;i<affairs.size();i++){
        QJsonObject affair=affairs.at(i).toObject();
        int st,ed;qstr_to_time(list[2],st,ed);
        if(affair["name"].toString()==list[0]&&affair["time"].toInt()==st){
                tmpaffair a=jsontotmpaffair(affair,*school_online);
                if(ui->aff_alarmcheck->isChecked()==true){
                    user_online->set_clock_tmpaffair(a,true);
                    log_action(tr("设置%1的闹钟").arg(a.name));
                }
                else{
                    user_online->set_clock_tmpaffair(a,false);
                    log_action(tr("取消%1的闹钟").arg(a.name));
                }
                break;

        }
    }
}


void AffairWin::on_tmpaffairlist_itemClicked(QListWidgetItem *item)
{
    if(item->background()==QColor(255,150,30)){
        ui->aff_alarmcheck->setEnabled(false);
    }
    QString data=item->text();
    QStringList list=data.split("  ");
    QJsonObject user;
    open_json(QString::number(user_online->get_id())+".json",user);
    QJsonArray affairs=user["affairs"].toArray();
    for(int i=0;i<affairs.size();i++){
        QJsonObject affair=affairs.at(i).toObject();
        int st,ed;qstr_to_time(list[2],st,ed);
        if(affair["name"].toString()==list[0]&&affair["time"].toInt()==st){
                QJsonObject al=affair["alarm"].toObject();
                ui->aff_alarmcheck->setEnabled(true);
                if(al["enable"].toBool()==true){
                    ui->aff_alarmcheck->setChecked(true);
                }
                else{
                    ui->aff_alarmcheck->setChecked(false);
                }
                break;
        }
        else ui->aff_alarmcheck->setEnabled(false);
    }
}


void AffairWin::on_activitylist_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current == NULL) return;
}


void AffairWin::on_tmpaffairlist_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current == NULL) return;
}

