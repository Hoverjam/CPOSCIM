#include "dialogwithtwobutn.h"
#include "ui_dialogwithtwobutn.h"

DialogWithTwoButn::DialogWithTwoButn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWithTwoButn)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/icos/attention2.ico"));
}

DialogWithTwoButn::~DialogWithTwoButn()
{
    delete ui;
}

void DialogWithTwoButn::on_tru_clicked()
{
    emit tru(who);
    close();
}

void DialogWithTwoButn::on_fal_clicked()
{
    close();
}

void DialogWithTwoButn::isWho(int sta)
{
    who = sta;
    if (who == 1)
    {
        ui->label->setText("你确定要删除所有（牵引站）参数吗？\n此操作无法撤销！");
        return;
    }
    if (who == 2)
        ui->label->setText("你确定要删除所有（负载）参数吗？\n此操作无法撤销！");
}
