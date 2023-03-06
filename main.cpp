#include "homewindow.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication::setApplicationDisplayName("QBangla Dictionary");
    QApplication::setApplicationName("QBangla Dictionary");
    QApplication::setOrganizationDomain("liotauhid@gmail.com");
    QApplication::setOrganizationName("QBangla");
    QApplication::setApplicationVersion("3.0.0");
    QApplication a(argc, argv);

    QTranslator t;
    QSettings setting;
    QString lang=setting.value("lang").toString();
    if(lang=="বাংলা"){
        t.load(":/BijoyEkush_bn_BD.qm");
        a.installTranslator(&t);
    }
    HomeWindow w;
    w.show();
    a.setWindowIcon(QIcon(":/logo.ico"));
    return a.exec();
}
