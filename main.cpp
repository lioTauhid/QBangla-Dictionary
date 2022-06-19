#include "homewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeWindow w;
    w.show();
    a.setWindowIcon(QIcon(":/logo.ico"));
    return a.exec();
}
