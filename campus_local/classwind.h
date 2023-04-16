#ifndef CLASSWIND_H
#define CLASSWIND_H

#include <QWidget>
#include <QStandardItemModel>
#include "person.h"

extern const User* user_online;

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
    void load();

private:
    Ui::classwind *ui;

};
#endif // CLASSWIND_H
