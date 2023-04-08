#ifndef REGSTE_H
#define REGSTE_H

#include <QDialog>

namespace Ui {
class Regste;
}

class Regste : public QDialog
{
    Q_OBJECT

public:
    explicit Regste(QWidget *parent = nullptr);
    ~Regste();

private:
    Ui::Regste *ui;
    void closeEvent(QCloseEvent *event);//处理关闭窗口事件
signals:
    void SendRegData(QString, QString, QString, QString);//发送注册信息
    void RetToLogIn();//返回登录窗口
private slots:
    void on_RegesConfirm_clicked();
    void on_RetToLogIn_clicked();
};

#endif // REGSTE_H
