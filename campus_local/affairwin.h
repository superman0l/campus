#ifndef AFFAIRWIN_H
#define AFFAIRWIN_H

#include <QWidget>

namespace Ui {
class AffairWin;
}

class AffairWin : public QWidget
{
    Q_OBJECT

public:
    explicit AffairWin(QWidget *parent = nullptr);
    ~AffairWin();


private slots:
    void on_comboBox_currentIndexChanged(int index);
    void load();
    void on_addactvt_clicked();

    void on_deleteactvt_clicked();

    void on_querytime_clicked();

private:
    Ui::AffairWin *ui;
};

#endif // AFFAIRWIN_H
