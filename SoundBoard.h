#ifndef SOUNDBOARD_H
#define SOUNDBOARD_H

#include <QTabBar>
#include "SoundButton.h"

#include <QPushButton>
#include <QGridLayout>
#include "CIniFile/IniFile.h"

class SoundBoard : public QWidget{
public:
    static const int BUTTON_COUNT = 8*3;
    static const QChar ids[BUTTON_COUNT];

private:
    SoundButton* buttons[BUTTON_COUNT];
    QGridLayout* grid;
    QString name;
    bool hasBeenModified, virgin;
    //Path to the ini_file_path
    QString ini_file_path;
    void init();
    void initLower();

public:
    SoundBoard();
    SoundBoard(QString);
    ~SoundBoard()
    {
        for(int i=0; i<BUTTON_COUNT; i++) if(buttons[i]) delete buttons[i];
        if(grid) delete grid;
    }

    SoundButton* getButton(int i);

    QString getName();

    void pressKey(QChar c);

    void releaseKey(QChar c);

    void saveToFile(QString path);

    void setModified() { hasBeenModified = true; virgin = true;}
    bool getModified() { return hasBeenModified; }
    bool isVirgin() { return virgin; }
};

#endif // SOUNDBOARD_H
