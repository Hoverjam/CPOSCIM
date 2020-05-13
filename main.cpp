#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    MainWindow w;
    QDesktopWidget *pDesk = QApplication::desktop();
    w.move((pDesk->width() - w.width()) / 2, (pDesk->height() - w.height()) / 2 - 30);
    w.show();
    return a.exec();
}
