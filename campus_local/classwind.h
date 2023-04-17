#ifndef CLASSWIND_H
#define CLASSWIND_H

#include <QWidget>
#include <QStandardItemModel>
#include "person.h"


namespace Ui {
class classwind;
}

class classwind : public QWidget
{
    Q_OBJECT

public:
    explicit classwind(QWidget *parent = nullptr);
    ~classwind();
    QStandardItemModel* model = new QStandardItemModel(13, 7);

    //bool eventFilter(QObject* object,QEvent* event) override;

    void load();;

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::classwind *ui;

};
#endif // CLASSWIND_H
