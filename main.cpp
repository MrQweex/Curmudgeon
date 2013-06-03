#include "MainWindow.h"
#include <QApplication>
#include <QtPlugin>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(APP_VERSION);
    MainWindow w;
    QApplication::setWindowIcon(QIcon(":/logo/images/logo_512.png"));
    w.show();
    w.newSoundboard();
    //a.connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(requestQuit()));
    return a.exec();
}
//*/
