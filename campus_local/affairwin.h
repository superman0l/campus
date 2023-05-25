#ifndef AFFAIRWIN_H
#define AFFAIRWIN_H

#include <QWidget>
#include <QListWidgetItem>
#include "basic.h"

namespace Ui {
class AffairWin;
}

class AffairWin : public QWidget
{
    Q_OBJECT

public:
    explicit AffairWin(QWidget *parent = nullptr);
    ~AffairWin();
    std::vector<std::vector<tmpaffair> > queryaffairresult;
    int f=0;
    void load_affair(int tag, int sorttype);
private slots:
    void on_comboBox_currentIndexChanged(int index);
    void load(int day, int tag,int sorttype);
    void on_addactvt_clicked();

    void on_deleteactvt_clicked();

    void on_querytime_clicked();

    void on_tags_currentIndexChanged(int index);

    void on_timesort_clicked();

    void on_aff_timesort_clicked();

    void on_aff_tags_currentIndexChanged(int index);

    void on_addtmpaffair_clicked();

    void on_deltmpaffair_clicked();

    void on_queryta_clicked();

    void on_next_clicked();

    void on_pre_clicked();

    void on_navigate_clicked();

    void on_aff_alarmcheck_stateChanged(int arg1);

    void on_tmpaffairlist_itemClicked(QListWidgetItem *item);

    void on_activitylist_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_tmpaffairlist_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::AffairWin *ui;
};

class listwidgetItem :public QListWidgetItem
{
public:
    bool operator<(const QListWidgetItem &other) const;

    void setSortType(int t)
    {
        mType = t;
    }

private:
    int mType = 0;

};

#endif // AFFAIRWIN_H
