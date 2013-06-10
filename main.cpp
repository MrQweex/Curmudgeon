#ifdef SUPPORT_THE_DEV
    #include "Purchase/Purchase.h"
#endif
#include "MainWindow.h"
#include <QApplication>
#include <QtPlugin>
#include <QSystemTrayIcon>
#include "SoundBoard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(APP_VERSION);

    Options::init();
#ifdef SUPPORT_THE_DEV
        Purchase::readLicenseInfo();
#endif

    MainWindow w;
    QApplication::setWindowIcon(QIcon(":/logo/images/logo_512.png"));
    w.show();
    bool opened = false;
    std::cout << "!!!" << Options::loadLast << std::endl;
    if(Options::loadLast)
    {
        QString read;
        read = Options::settings->value("lastCount").toString();
        int lastCount = read.toInt();
        for(int i=0; i<lastCount; i++)
        {
            read = Options::settings->value(QString("last%1").arg(i)).toString();
            if(read.length()>0)
            {
                w.AddSoundboard(new SoundBoard(read));
                opened=true;
            }
        }
    }
    if(!opened)
        w.newSoundboard();

#ifdef SUPPORT_THE_DEV
    if(Purchase::firstTime && !Purchase::keyIsValid())
        Purchase::beg();
#endif

    QSystemTrayIcon duck;
    duck.showMessage("Herpderpderp","derp");
    return a.exec();
}
