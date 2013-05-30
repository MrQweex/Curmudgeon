#ifndef SOUNDBUTTON_H
#define SOUNDBUTTON_H

#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QChar>
#include <iostream>
#include <QPalette>
#include <QShortcut>
#include <QObject>
#include <QMouseEvent>
#include <QFileDialog>
#include <QUrl> //Neede for drop

#include <SFML/Audio.hpp>
#include "AudioPlayer/AudioPlayerFactory.h"


class SoundButton :public QGroupBox,  public AudioPlayerCallback
{
    Q_OBJECT

private:
    static const QString CLR_ENABLED,
                  CLR_DISABLED,
                  CLR_PRESSED,
                  CLR_PLAYING,
                  CLR_TXT_ENABLED,
                  CLR_TXT_DISABLED,
                  CLR_DRAG;
    static const QString FILETYPES;
    //Path to the sound file
    QString sound_file_path;
    //What this button is; for example, '1', 'a', 'q', etc...
    QChar ID;
    //Percentage of volume level
    int volume_level;
    //Is it currently playing?
    bool is_playing;
    //Layout containing the widgets
    QGridLayout* layout;
    //The name widget
    QLabel* name;
    //The id widget
    QLabel* id;
    //callback to when track is finished
    void (SoundButton::*callback)();
    //Audio Player Object
    AudioPlayer* player;


public:
    void turnOff() { this->setStyleSheet("background-color: #" + CLR_ENABLED); }

    //Called when reading in information from file
    SoundButton(QChar id, QString path, int vol);

    //Basic constructor
    SoundButton(QChar id);

    //Copy Constructor
    SoundButton(SoundButton &s)
    { }
    ~SoundButton(){}

    //Initialize the object
    void init();

    //Mouse is pressed over button
    void mousePressEvent(QMouseEvent * event);

    //Mouse is release after mousePressEvent
    void mouseReleaseEvent(QMouseEvent * event);

    //The key on the keyboard has been pressed
    void pressKey();

    //THe key on the keyboard has been release
    void releaseKey();


    //Listener inherited from AudioPlayer
    void playingFinished();


protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void setMedia(QString s);
};

#endif // SOUNDBUTTON_H
