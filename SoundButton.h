#ifndef SOUNDBUTTON_H
#define SOUNDBUTTON_H

#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QChar>
#include <QPalette>
#include <QShortcut>
#include <QObject>
#include <QMouseEvent>
#include <QFileDialog>
#include <QUrl>
#include <QResizeEvent>
#include <QSlider>
#include <QMenu>
#include <QInputDialog>

#include <iostream>

#include "AudioPlayer/AudioPlayerFactory.h"
#include "CIniFile/IniFile.h"
#include "qlabelwrapellip.h"
#include "QSliderFixedSize.h"

class SoundButton :public QGroupBox,  public AudioPlayerCallback
{
    Q_OBJECT

private:
    static const QString CLR_ENABLED,
                  CLR_DISABLED,
                  CLR_PRESSED,
                  CLR_PLAYING,
                  CLR_NOT_SET,
                  CLR_TXT_ENABLED,
                  CLR_TXT_DISABLED,
                  CLR_DRAG;
    static const QString FILETYPES;
    //What this button is; for example, '1', 'a', 'q', etc...
    QChar ID;
    //Percentage of volume level
    int volume_level;
    //Layout containing the widgets
    QGridLayout* layout;
    //The name widget
    QLabelWrapEllip* name;
    //The id widget
    QLabel* id;
    //slider
    QSliderFixedSize* volume;
    //Audio Player Object
    AudioPlayer* player;
    //path to the MP3, WAV, etc
    QString sound_file_path;
    //Path to the ini_file_path of the soundboard this is associated with
    QString* boards_ini_file_path;
    //Menu for pressing the ID
    QMenu* popupMenu;
    //The nickname
    QString nickname;


public:
    void turnOff() { setStyleSheet("background-color: #" + CLR_ENABLED); }

    //Basic constructor
    SoundButton(QChar id);

    //Consctuctor for reading in data
    SoundButton(QChar id,
                 QString path,
                 int vol,
                 QString nick
                 //,doneAction, releaseAction, repressAction
                 );

    //Copy Constructor
    SoundButton(SoundButton &s)
    { }
    ~SoundButton()
    {
        if(layout) delete layout;
        if(name) delete name;
        if(id) delete id;
        if(player) delete player;
        if(boards_ini_file_path) delete boards_ini_file_path;
    }
    void resizeEvent(QResizeEvent* event);

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
    virtual void playingFinished();

    void saveToFile(QString*);


private:
    void saveVolume(QString* filePath);
    void saveNickname(QString* filePath);
    bool measured;

protected:
#ifdef _WIN32
    bool winEvent( MSG * message, long * result )
    {
        switch(message->message)
        {
        case 0x03b9:    //MM_MCINOTIFY
        //case 0x01:      //MCI_NOTIFY_SUCCESS
        //case 0x02:      //MCI_NOTIFY_SUPERSEDED
        //case 0x04:      //MCI_NOTIFY_ABORTED
        //case 0x05:      //MCI_NOTIFY_FAILURE
            this->playingFinished();
        }
        return QGroupBox::winEvent(message,result);
    }
#endif
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void setMedia(QString s);
private slots:
    void volumeChanged(int);
    void saveVolume() { if(boards_ini_file_path) saveVolume(boards_ini_file_path); }
    void rename();
    void changeSound();
    void destroy();
};

#endif // SOUNDBUTTON_H
