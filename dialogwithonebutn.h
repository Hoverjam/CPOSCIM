#ifndef DIALOGWITHNOBUTN_H
#define DIALOGWITHNOBUTN_H

#include <QDialog>

namespace Ui {
class DialogWithOneButn;
}

class DialogWithOneButn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWithOneButn(QWidget *parent = nullptr);
    ~DialogWithOneButn();

    void setMainLabel(const QString &labelStr);
    void showUrl();

private slots:
    void on_pushButton_clicked();

    void on_url_clicked();

private:
    Ui::DialogWithOneButn *ui;
};

#endif // DIALOGWITHNOBUTN_H
