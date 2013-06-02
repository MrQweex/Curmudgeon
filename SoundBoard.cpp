#include "SoundBoard.h"

const QChar SoundBoard::ids[BUTTON_COUNT] = {'1','2','3','4','5','6','7','8',
                               'A','S','D','F','G','H','J','K',
                               'Z','X','C','V','B','N','M',','};

//
SoundBoard::SoundBoard() : name("New Soundboard"), virgin(true), boardVolume(100)
{
    init();
    for(int i=0; i<BUTTON_COUNT; i++)
    {
        buttons[i] = new SoundButton(&boardVolume, ids[i], Options::defaultDoneAction, Options::defaultReleasedAction, Options::defaultRepressedAction);
        grid->addWidget(buttons[i],i/8,i%8);
    }
    initLower();
}

SoundBoard::SoundBoard(QString pathToIniFile) : virgin(false), boardVolume(100)
{
    init();
    ini_file_path = pathToIniFile;
    name = QFileInfo(ini_file_path).baseName();

    std::string path, nick;
    int vol;
    SoundButton::_DONE_ACTION done;
    SoundButton::_RELEASED_ACTION released;
    SoundButton::_REPRESSED_ACTION repressed;
    std::string temp;

    for(int i=0; i<BUTTON_COUNT; i++)
    {
        path = CIniFile::GetValue(std::string("path"), std::string(1,ids[i].toAscii()),ini_file_path.toStdString().c_str());
        if(path.length()>0)
        {
            //TODO: parse more reliably
            vol = atoi(CIniFile::GetValue("vol", std::string(1,ids[i].toAscii()), ini_file_path.toStdString()).c_str());
            nick = CIniFile::GetValue(std::string("nick"), std::string(1,ids[i].toAscii()),ini_file_path.toStdString().c_str());

            temp = CIniFile::GetValue(std::string("done"), std::string(1,ids[i].toAscii()),ini_file_path.toStdString().c_str());
            if(temp=="stop")
                done = SoundButton::DONE_STOP;
            else if(temp=="loop")
                done = SoundButton::DONE_LOOP;
            else
                done = Options::defaultDoneAction;

            temp = CIniFile::GetValue(std::string("released"), std::string(1,ids[i].toAscii()),ini_file_path.toStdString().c_str());
            if(temp=="stop")
                released = SoundButton::RELEASED_STOP;
            else if(temp=="continue")
                released = SoundButton::RELEASED_CONTINUE;
            else
                released = Options::defaultReleasedAction;

            temp = CIniFile::GetValue(std::string("repressed"), std::string(1,ids[i].toAscii()),ini_file_path.toStdString().c_str());
            if(temp=="stop")
                repressed = SoundButton::REPRESSED_STOP;
            else if(temp=="restart")
                repressed = SoundButton::REPRESSED_RESTART;
            else
                repressed = Options::defaultRepressedAction;

            buttons[i] = new SoundButton(&boardVolume, ids[i],QString(path.c_str()), vol, QString(nick.c_str()),
                                         done, released, repressed);
        } else
            buttons[i] = new SoundButton(&boardVolume, ids[i], Options::defaultDoneAction, Options::defaultReleasedAction, Options::defaultRepressedAction);
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
    QGridLayout* g = new QGridLayout();
    g->setColumnStretch(1,100);
    g->setRowStretch(0,1);
    g->setRowStretch(1,1);
    g->setContentsMargins(60,0,60,0);

    volSlider = new QSlider(Qt::Horizontal,lower);
    volSlider->setMaximum(100);
    volSlider->setValue(100);
    panSlider = new QSlider(Qt::Horizontal,lower);
    panSlider->setMaximum(100);
    panSlider->setMinimum(-100);
    panSlider->setValue(0);
    connect(volSlider,SIGNAL(valueChanged(int)),this,SLOT(trackVolume(int)));
    connect(panSlider,SIGNAL(valueChanged(int)),this,SLOT(trackBalance(int)));

    QLabel *down, *up;
    down= new QLabel;
    up=new QLabel;
    down->setPixmap(QPixmap(":res/images/vol-down.png"));
    up->setPixmap(QPixmap(":res/images/vol-up.png"));
    g->addWidget(down,0,0);
    g->addWidget(volSlider, 0, 1);
    g->addWidget(up,0,2);

    QLabel *l, *r;
    l= new QLabel;
    r=new QLabel;
    l->setPixmap(QPixmap(":res/images/l.png"));
    r->setPixmap(QPixmap(":res/images/r.png"));
    g->addWidget(l,1,0);
    g->addWidget(panSlider, 1, 1);
    g->addWidget(r,1,2);

    lower->setLayout(g);
    grid->addWidget(lower,BUTTON_COUNT/8+1,0,1,8);
}

void SoundBoard::trackVolume(int i)
{
    QFontMetrics metrics(QToolTip::font());
    QPoint p = QCursor::pos();
    QRect widgetRect = volSlider->geometry();
    widgetRect.moveTopLeft(volSlider->parentWidget()->mapToGlobal(widgetRect.topLeft()));
    p.setY(widgetRect.y()- volSlider->geometry().height() - metrics.height());
    p.setX(p.x() - metrics.width("69%")/2);
    QToolTip::showText(p,QString("%1").arg(i).append("%"));

    boardVolume = i;

    for(int i=0; i<BUTTON_COUNT; i++)
    {
        buttons[i]->refreshVolume();
    }
}

void SoundBoard::trackBalance(int i)
{
    QFontMetrics metrics(QToolTip::font());
    QPoint p = QCursor::pos();
    QRect widgetRect = panSlider->geometry();
    widgetRect.moveTopLeft(panSlider->parentWidget()->mapToGlobal(widgetRect.topLeft()));
    p.setY(widgetRect.y()- panSlider->geometry().height() - metrics.height());
    p.setX(p.x() - metrics.width("69")/2);
    QToolTip::showText(p,QString("%1").arg(i));

    boardBalance = i;

    for(int i=0; i<BUTTON_COUNT; i++)
    {
        buttons[i]->setBalance(boardBalance);
    }
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

    //Rename the tab
    QFileInfo fi(path);
    QTabWidget* qtw = this->window()->findChild<QTabWidget*>("mainTabs");
    qtw->setTabText(qtw->currentIndex(), fi.baseName());
}

#include "MainWindow.h"
void SoundBoard::breakVirgin()
{
    std::cout << "DERP2" << std::endl;
    virgin = false;
    ((MainWindow*)this->window())->modifiedFile(this);
}
