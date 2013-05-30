#include "SoundBoard.h"

const QChar SoundBoard::ids[BUTTON_COUNT] = {'1','2','3','4','5','6','7','8',
                               'A','S','D','F','G','H','J','K',
                               'Z','X','C','V','B','N','M',','};

SoundBoard::SoundBoard(QString name) : name(name)
{
    grid = new QGridLayout();

    grid->setSpacing(20);
    grid->setMargin(10);

    for(int i=0; i<BUTTON_COUNT; i++)
    {
        buttons[i] = new SoundButton(ids[i]);
        grid->addWidget(buttons[i],i/8,i%8);
    }
    QGroupBox* lower = new QGroupBox();
    grid->addWidget(lower,BUTTON_COUNT/8+1,0,1,8);
    this->setLayout(grid);
};

SoundButton* SoundBoard::getButton(int i)
{
    //if(i<0 || 8*4<i) error...

    return buttons[i];
}

QString SoundBoard::getName()
{
    return name;
}

void SoundBoard::pressKey(QChar c)
{
    for(int i=0; i<BUTTON_COUNT; i++)
        if(c.toAscii()==ids[i].toAscii())
        {
            buttons[i]->pressKey();
            break;
        }
}

void SoundBoard::releaseKey(QChar c)
{
    for(int i=0; i<BUTTON_COUNT; i++)
        if(c.toAscii()==ids[i].toAscii())
        {
            buttons[i]->releaseKey();
            break;
        }
}
