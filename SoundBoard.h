#ifndef SOUNDBOARD_H
#define SOUNDBOARD_H

#include <QTabBar>
#include "SoundButton.h"

#include <QPushButton>
#include <QGridLayout>

class SoundBoard : public QWidget{
public:
    static const int BUTTON_COUNT = 8*3;
    static const QChar ids[BUTTON_COUNT];

private:
    SoundButton* buttons[BUTTON_COUNT];
    QGridLayout* grid;
    QString name;

public:
    explicit SoundBoard(QString name);
     ~SoundBoard()
    {
        for(int i=0; i<BUTTON_COUNT; i++)
            delete buttons[i];
        delete grid;
    }


    SoundButton* getButton(int i);

    QString getName();

    void pressKey(QChar c);

    void releaseKey(QChar c);

    void handleCloseTab(int i)
    {

    }
};

#endif // SOUNDBOARD_H
