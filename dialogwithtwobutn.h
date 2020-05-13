#ifndef DIALOGWITHTWOBUTN_H
#define DIALOGWITHTWOBUTN_H

#include <QDialog>

namespace Ui {
class DialogWithTwoButn;
}

class DialogWithTwoButn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWithTwoButn(QWidget *parent = nullptr);
    ~DialogWithTwoButn();

    void isWho(int sta);

signals:
    void tru(int);

private slots:
    void on_tru_clicked();

    void on_fal_clicked();

private:
    Ui::DialogWithTwoButn *ui;
    int who;
};

#endif // DIALOGWITHTWOBUTN_H
