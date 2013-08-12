#ifndef SOUNDBOARD_H
#define SOUNDBOARD_H

#include <QTabBar>
#include "SoundButton.h"

#include <QPushButton>
#include <QGridLayout>
#include <QResizeEvent>
#include "CIniFile/IniFile.h"
#include <QSizePolicy>
#include <QEvent>
#include <QToolTip>
#include "Options.h"

class SoundBoard : public QWidget{

    Q_OBJECT

public:
    static const int BUTTON_ROW_SIZE = 10;
    static const int BUTTON_COUNT = BUTTON_ROW_SIZE*4;
    static const QChar ids[BUTTON_COUNT];

private:
    SoundButton* buttons[BUTTON_COUNT];
    QGridLayout* grid;
    QString name;
    bool virgin;
    int boardVolume, boardBalance;
    //Path to the ini_file_path
    QString ini_file_path;
    void init();
    void initLower();
    QSlider *volSlider, *panSlider;

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
    QString getPath() { return ini_file_path; }

    void pressKey(QChar c);

    void releaseKey(QChar c);

    void saveToFile(QString path);

    bool shouldPromptToSave()
    {
        return !virgin && ini_file_path.length()==0;
    }
    bool isVirgin() { return virgin; }
    void breakVirgin();

public slots:
    void trackVolume(int);
    void trackBalance(int);
    void stopAll();
};

#endif // SOUNDBOARD_H
