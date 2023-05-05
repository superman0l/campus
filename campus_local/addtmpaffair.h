#ifndef ADDTMPAFFAIR_H
#define ADDTMPAFFAIR_H

#include <QDialog>

namespace Ui {
class addtmpaffair;
}

class addtmpaffair : public QDialog
{
    Q_OBJECT

public:
    explicit addtmpaffair(QWidget *parent = nullptr);
    ~addtmpaffair();

private slots:
    void on_add_clicked();

signals:
    void flash(int,int);

private:
    Ui::addtmpaffair *ui;
};

#endif // ADDTMPAFFAIR_H
