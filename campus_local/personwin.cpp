#include "personwin.h"
#include "ui_personwin.h"
#include "person.h"
#include "basic.h"
#include <QMessageBox>
#include <QInputDialog>

extern User* user_online;

PersonWin::PersonWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonWin)
{
    ui->setupUi(this);
    //开始初始化
    FinConf = false;
    //填充内容
    ui->Name->setText(QString::fromStdString(user_online->get_name()));
    ui->ClassNum->setText(QString::number(user_online->get_classid()));
    ui->Number->setText(QString::number(user_online->get_id()));
    //设置楷体及字体大小
    QFont ft("KaiTi", 18);
    //修改标签格式
    ui->Name->setFont(ft);
    ui->ClassNum->setFont(ft);
    ui->Number->setFont(ft);

    //设置combobox内的内容
    ui->PositionBox->clear();
    QMap<int, QString> Position_Zone;
    Position_Zone.insert(7, "学十公寓");
    Position_Zone.insert(10, "学九公寓");
    Position_Zone.insert(12, "留学生2号公寓");
    Position_Zone.insert(13, "留学生1号公寓");
    Position_Zone.insert(21, "学十一公寓");
    Position_Zone.insert(28, "学六公寓");
    Position_Zone.insert(33, "学八公寓");
    Position_Zone.insert(35, "学五公寓");
    Position_Zone.insert(36, "学十三公寓");
    Position_Zone.insert(38, "学三公寓");
    Position_Zone.insert(40, "学四公寓");
    Position_Zone.insert(44, "学一公寓");
    Position_Zone.insert(46, "学二公寓");
    Position_Zone.insert(51, "学二九公寓");
    foreach(const int &id, Position_Zone.keys())
            ui->PositionBox->addItem(Position_Zone.value(id), id);

    //初始化完成
    FinConf = true;

    //根据user的存储默认位置展示内容
    QMap<int, QString>::iterator i = Position_Zone.find(user_online->get_place_id());
    qDebug() << user_online->get_place_id();
    if(i == Position_Zone.end()){
        ui->PositionBox->setCurrentIndex(7);
        qDebug() << "Not find";
    }
    else
    {
        ui->PositionBox->setCurrentText(i.value());
        qDebug() << "Finded" << user_online->get_place_id();
    }
    //qDebug() << i.value();

}

PersonWin::~PersonWin()
{
    delete ui;
}

void PersonWin::on_ChangePswd_clicked()
{
    if(CheckPswd()){
        if(ChangePswd())
            QMessageBox::information(this, "通知", "成功修改了密码！！");
    }
    else{
        QMessageBox::warning(this, "错误", "输入了错误的密码！！");
    }
}


void PersonWin::on_DestroyCount_clicked()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, "注意！！！", "您正在尝试危险操作：销毁账户，确定要继续吗？");
    if(button == QMessageBox::Yes){
        if(CheckPswd()){
            QJsonObject obj;
            QFile::remove("../Json/"+QString::number(user_online->get_id())+".json");
            open_json("id_pswd.json", obj);
            obj.remove(QString::number(user_online->get_id()));
            write_json("id_pswd.json", obj);
            QMessageBox::information(this, "通知", "成功销毁了您的账户，正在回到主页面!");
            emit DestroyCount();
        }
        else{
            QMessageBox::warning(this, "错误", "输入了错误的密码！！");
        }
    }
}

bool PersonWin::ChangePswd(void)
{
    bool conf;
    QString inpu = QInputDialog::getText(this, "请输入新密码", "请输入新的账户密码：", QLineEdit::Password,"", &conf);
    if(conf && !inpu.isEmpty()){
        bool reconf;
        QString reinpu = QInputDialog::getText(this, "请确认新密码", "请确认新的账户密码", QLineEdit::Password, "", &reconf);
        if(reinpu == inpu){
            QJsonObject obj;
            if(!open_json("id_pswd.json", obj))
            {
                return false;
            }
            obj[QString::number(user_online->get_id())] = inpu;
            if(!write_json("id_pswd.json", obj))
            {
                return false;
            }
            return true;
        }
        else{
            QMessageBox::warning(this, "错误", "两次输入的密码不一致");
        }
    }

    return false;
}

bool PersonWin::CheckPswd(void)
{
    bool conf;
    QString inpu = QInputDialog::getText(this, "请确认密码", "请输入账户密码：", QLineEdit::Password,"", &conf);
    if(conf && !inpu.isEmpty()){
        QJsonObject obj;
        if(!open_json("id_pswd.json", obj))
        {
            return false;
        }
        if(obj.value(QString::number(user_online->get_id())).toString() != inpu)
        {
            return false;
        }
        return true;
    }
    else{
        return false;
    }
}


void PersonWin::on_PositionBox_currentTextChanged(const QString &arg1)
{
    int placecode = ui->PositionBox->currentData().toInt();
    //设置用户起始位置
    if(FinConf)
    {
        user_online->set_place_id(placecode);
        qDebug() << "Changed" << user_online->get_place_id();
    }
}

