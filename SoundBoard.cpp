#include "SoundBoard.h"

const QChar SoundBoard::ids[BUTTON_COUNT] = {'1','2','3','4','5','6','7','8',
                               'A','S','D','F','G','H','J','K',
                               'Z','X','C','V','B','N','M',','};

//
SoundBoard::SoundBoard() : name("New Soundboard"), virgin(true), hasBeenModified(false)
{
    init();
    for(int i=0; i<BUTTON_COUNT; i++)
    {
        buttons[i] = new SoundButton(ids[i]);
        grid->addWidget(buttons[i],i/8,i%8);
    }
    initLower();
}

SoundBoard::SoundBoard(QString pathToIniFile) : virgin(false), hasBeenModified(false)
{
    init();
    ini_file_path = pathToIniFile;
    name = QFileInfo(ini_file_path).baseName();

    std::string path, nick;
    int vol;

    for(int i=0; i<BUTTON_COUNT; i++)
    {
        path = CIniFile::GetValue(std::string("path"), std::string(1,ids[i].toAscii()),ini_file_path.toStdString().c_str());
        if(path.length()>0)
        {
            //TODO: parse more reliably
            vol = atoi(CIniFile::GetValue("vol", std::string(1,ids[i].toAscii()), ini_file_path.toStdString()).c_str());
            nick = CIniFile::GetValue(std::string("nick"), std::string(1,ids[i].toAscii()),ini_file_path.toStdString().c_str());
            //,doneAction, releaseAction, repressAction
            buttons[i] = new SoundButton(ids[i],QString(path.c_str()), vol, QString(nick.c_str()));
        } else
            buttons[i] = new SoundButton(ids[i]);
        grid->addWidget(buttons[i],i/8,i%8);
    }
    initLower();
}

void SoundBoard::init()
{
    grid = new QGridLayout();
    grid->setSpacing(20);
    grid->setMargin(10);
    this->setLayout(grid);
}

void SoundBoard::initLower()
{
    QGroupBox* lower = new QGroupBox();
    grid->addWidget(lower,BUTTON_COUNT/8+1,0,1,8);
}

SoundButton* SoundBoard::getButton(int i)//is needed?
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

#include <QTabWidget>
void SoundBoard::saveToFile(QString path)
{
    ini_file_path = path;
    if(!CIniFile::Create(path.toStdString()))
    {
        std::cerr << "ERROR: cannot create " << path.toStdString() << std::endl;
        return;
    }
    for(int i=0; i<BUTTON_COUNT; i++)
        buttons[i]->saveToFile(&ini_file_path);
    hasBeenModified = false;

    //Rename the tab
    QFileInfo fi(path);
    QTabWidget* qtw = this->window()->findChild<QTabWidget*>("mainTabs");
    qtw->setTabText(qtw->currentIndex(), fi.baseName());
}

