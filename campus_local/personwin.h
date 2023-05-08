#ifndef PERSONWIN_H
#define PERSONWIN_H

#include <QWidget>

namespace Ui {
class PersonWin;
}

class PersonWin : public QWidget
{
    Q_OBJECT

public:
    explicit PersonWin(QWidget *parent = nullptr);
    ~PersonWin();

private slots:
    void on_ChangePswd_clicked();

    void on_DestroyCount_clicked();

    void on_PositionBox_currentTextChanged(const QString &arg1);

private:
    Ui::PersonWin *ui;
    bool ChangePswd(void);
    bool CheckPswd(void);    
    bool FinConf;//标记初始化是否完成
signals:
    void DestroyCount();
};

#endif // PERSONWIN_H
