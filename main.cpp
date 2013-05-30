#include "mainwindow.h"
#include <QApplication>

/* Testing blogk for SFML
#include <SFML/Audio.hpp>
//wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64
int main2(int argc, char *argv[])
{
    sf::Music music;
    if (!music.openFromFile("/Users/notbryant/byu/Winning.mp3"))
        return 1;
    music.play();
    while(music.getStatus())
    {

    }
}
*/


/*
 *  Curmudgeon
 *  Clatter
 *  Din
 *  Noise
 *  Ruckus
 *  Static
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(APP_VERSION);
    MainWindow w;
    QApplication::setWindowIcon(QIcon(":/logo/images/logo_512.png"));
    w.show();
    w.newSoundboard();
    return a.exec();
}
//*/
