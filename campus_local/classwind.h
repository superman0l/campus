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
    void load();

private:
    Ui::classwind *ui;
    const User* user;

private slots:
    void UserIn(const User*);
};
#endif // CLASSWIND_H
