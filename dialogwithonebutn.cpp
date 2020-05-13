#include "dialogwithonebutn.h"
#include "ui_dialogwithonebutn.h"
#include <QDesktopServices>
#include <QUrl>

DialogWithOneButn::DialogWithOneButn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWithOneButn)
{
    ui->setupUi(this);
}

DialogWithOneButn::~DialogWithOneButn()
{
    delete ui;
}

void DialogWithOneButn::setMainLabel(const QString &labelStr)
{
    ui->label->setText(labelStr);
    ui->url->hide();
}

void DialogWithOneButn::showUrl()
{
    ui->url->show();
}

void DialogWithOneButn::on_pushButton_clicked()
{
    close();
}

void DialogWithOneButn::on_url_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.baidu.com"));
    close();
}
