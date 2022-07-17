#include "bijoyekush_bangla_dictionary.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BijoyEkush_Bangla_Dictionary w;
    w.show();

    return a.exec();
}
