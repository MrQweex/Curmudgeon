#include "MainWindow.h"
#include <QApplication>
#include <QtPlugin>
#include "SoundBoard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(APP_VERSION);
    MainWindow w;
    QApplication::setWindowIcon(QIcon(":/logo/images/logo_512.png"));
    w.show();
    bool opened = false;
    if(Options::init_returnLoadLast())
    {
        for(int i=0; i<5; i++)
        {
            if(Options::recentFiles[i].length()==0)
                break;
            w.AddSoundboard(new SoundBoard(Options::recentFiles[i]));
            opened=true;
        }
    }
    if(!opened)
        w.newSoundboard();
    return a.exec();
}
