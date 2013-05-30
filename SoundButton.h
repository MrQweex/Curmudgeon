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

//#include <SFML/Audio.hpp>
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
    //QChar ID;
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
    QChar ID;
    void turnOff() { setStyleSheet("background-color: #" + CLR_ENABLED); }

    //Called when reading in information from file
    SoundButton(QChar id, QString path, int vol);

    //Basic constructor
    SoundButton(QChar id);

    //Copy Constructor
    SoundButton(SoundButton &s)
    { }
    ~SoundButton()
    {
        if(player)
            player->pause();
        delete layout;
        delete name;
        delete id;
        delete player;
    }

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
    bool winEvent( MSG * message, long * result )
    {
        switch(message->message)
        {
        case 0x03b9:    //MM_MCINOTIFY
        case 0x01:      //MCI_NOTIFY_SUCCESS
        case 0x02:      //MCI_NOTIFY_SUPERSEDED
        case 0x04:      //MCI_NOTIFY_ABORTED
        case 0x05:      //MCI_NOTIFY_FAILURE
            this->playingFinished();
        }
        return QGroupBox::winEvent(message,result);
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void setMedia(QString s);
};

#endif // SOUNDBUTTON_H
