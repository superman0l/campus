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
    void load(int day, int tag);
    void on_addactvt_clicked();

    void on_deleteactvt_clicked();

    void on_querytime_clicked();

    void on_tags_currentIndexChanged(int index);

private:
    Ui::AffairWin *ui;
};

#endif // AFFAIRWIN_H
