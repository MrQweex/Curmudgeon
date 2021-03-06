#include "SoundButton.h"
#include "SoundBoard.h"

#ifdef _WIN32
const QString SoundButton::FILETYPES = formatFiletypes(AudioPlayerWin::FILETYPES, AudioPlayerWin::FILETYPE_COUNT);
#elif defined __APPLE__
const QString SoundButton::FILETYPES = formatFiletypes(AudioPlayerOsx::FILETYPES, AudioPlayerOsx::FILETYPE_COUNT);
#else
const QString SoundButton::FILETYPES = formatFiletypes(AudioPlayerGnu::FILETYPES, AudioPlayerGnu::FILETYPE_COUNT);
#endif


const QString
          SoundButton::CLR_ENABLED = "ddd",
          SoundButton::CLR_DISABLED = "eee",
          SoundButton::CLR_PRESSED = "aaa",
          SoundButton::CLR_PLAYING ="74C365",
          SoundButton::CLR_NOT_SET = "E6E0CF",
          SoundButton::CLR_TXT_ENABLED = "000",
          SoundButton::CLR_TXT_DISABLED = "888",
          SoundButton::CLR_DRAG = "BEE6B8",
          SoundButton::CLR_ERROR = "E01B1B";


QString SoundButton::formatFiletypes(const char* filetypes[], const int &num)
{
    QString result = "(";
    for(int i=0; i<num; i++)
    {
        result.append("*.").append(filetypes[i]);
        if(i!=num-1)
            result.append(" ");
    }
    return result.append(")");
}

SoundButton::SoundButton(QWidget* Parent,
                         QString* ini_path,
                         int* bvol,
                         QChar id,
                         _DONE_ACTION  doneAction,
                         _RELEASED_ACTION releaseAction,
                         _REPRESSED_ACTION repressAction) : board_volume_level(bvol)
{
    this->setParent(Parent);
    ID = id;
    this->setBoardFile(ini_path);
    this->doneAction = doneAction;
    this->releasedAction = releaseAction;
    this->repressedAction = repressAction;
    init();
    volume->setValue(100);
}

SoundButton::SoundButton(QWidget* Parent,
                         QString* ini_path,
                         int* bvol,
                         QChar id,
                         QString path,
                         int vol,
                         QString nick,
                         _DONE_ACTION  doneAction,
                         _RELEASED_ACTION releaseAction,
                         _REPRESSED_ACTION repressAction
                         ) : board_volume_level(bvol)
{
    this->setParent(Parent);
    ID = id;
    this->setBoardFile(ini_path);
    nickname = nick;
    init();
    setMedia(path);
    volume_level = vol;
    volume->setValue(vol);
}

#include <QLayout>
#include <QSizePolicy>
void SoundButton::init()
{
    name = new QLabelWrapEllip(QString(""));
    name->setStyleSheet("background:none;");
    name->setAlignment(Qt::AlignTop);
    #ifdef _WIN32
        name->setMargin(5);
    #elif __APPLE__
        name->setMargin(0);
    #else
        name->setMargin(5);
    #endif

    id = new QLabel(QString(QChar(ID)));
    id->setStyleSheet("border: 1px solid #" + CLR_TXT_DISABLED + "; color: #" + CLR_TXT_DISABLED);

    volume = new QSliderFixedSize();
    volume->setStyleSheet("background:none;");
    volume->setMaximum(100);
    volume->setEnabled(false);
    connect(volume,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged(int)));
    connect(volume,SIGNAL(sliderReleased()),this,SLOT(saveVolume()));

    layout = new QGridLayout();
    layout->addWidget(name,0,0);
    layout->addWidget(id,1,1);
    layout->addWidget(volume,0,1);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setColumnStretch(0,100);
    layout->setRowStretch(0,100);
    this->setLayout(layout);
    this->setStyleSheet("background-color: #" + CLR_DISABLED);
    this->setAcceptDrops(true);

    QAction* rename = new QAction("Rename", this);
    QAction* change = new QAction("Change", this);
    QAction* destroy = new QAction("Destroy", this);
    popupMenu = new QMenu();
    popupMenu->addAction(rename);
    popupMenu->addAction(change);
    popupMenu->addAction(destroy);
    connect(rename,SIGNAL(triggered()),this,SLOT(rename()));
    connect(change,SIGNAL(triggered()),this,SLOT(changeSound()));
    connect(destroy,SIGNAL(triggered()),this,SLOT(destroy()));

    popupMenu->addSeparator();

    {
        QMenu* doneMenu = new QMenu("When finished:");
        QAction* stop = new QAction("Stop", this);
        QAction* loop = new QAction("Loop", this);
        stop->setCheckable(true);
        loop->setCheckable(true);
        doneGroup = new QActionGroup(this);
        doneGroup->setExclusive(true);
        doneGroup->addAction(stop);
        doneGroup->addAction(loop);
        doneMenu->addAction(stop);
        doneMenu->addAction(loop);
        popupMenu->addMenu(doneMenu);
        connect(stop, SIGNAL(triggered()), this, SLOT(menu_doneStop()));
        connect(loop, SIGNAL(triggered()), this, SLOT(menu_doneLoop()));

        if(doneAction==DONE_STOP)
            stop->setChecked(true);
        else
            loop->setChecked(true);
    }

    {
        QMenu* releasedMenu = new QMenu("When released:");
        QAction* stop = new QAction("Stop", this);
        QAction* continue_ = new QAction("Continue", this);
        stop->setCheckable(true);
        continue_->setCheckable(true);
        releasedGroup = new QActionGroup(this);
        releasedGroup->addAction(stop);
        releasedGroup->addAction(continue_);
        releasedMenu->addAction(stop);
        releasedMenu->addAction(continue_);
        popupMenu->addMenu(releasedMenu);
        connect(stop, SIGNAL(triggered()), this, SLOT(menu_releasedStop()));
        connect(continue_, SIGNAL(triggered()), this, SLOT(menu_releasedContinue()));

        if(releasedAction==RELEASED_STOP)
            stop->setChecked(true);
        else
            continue_->setChecked(true);
    }

    {
        QMenu* repressedMenu = new QMenu("When re-pressed:");
        QAction* stop = new QAction("Stop", this);
        QAction* restart = new QAction("Restart", this);
        stop->setCheckable(true);
        restart->setCheckable(true);
        repressedGroup = new QActionGroup(this);
        repressedGroup->addAction(stop);
        repressedGroup->addAction(restart);
        repressedMenu->addAction(stop);
        repressedMenu->addAction(restart);
        popupMenu->addMenu(repressedMenu);
        connect(stop, SIGNAL(triggered()), this, SLOT(menu_repressedStop()));
        connect(restart, SIGNAL(triggered()), this, SLOT(menu_repressedRestart()));

        if(repressedAction==REPRESSED_STOP)
            stop->setChecked(true);
        else
            restart->setChecked(true);
    }

#ifdef _WIN32
    this->createWinId();
#endif

    player = NULL;
}

void SoundButton::destroy()
{
    this->stop();
    this->setStyleSheet("background-color: #" + CLR_DISABLED);
    name->setText(tr(""));
    volume->setEnabled(false);
    volume->setValue(100);
    volume_level=100;
    delete player;
    player = NULL;
    sound_file_path="";
    nickname="";
}

void SoundButton::rename()
{
    bool ok;
    QString newname = QInputDialog::getText(this, tr("Rename"), QString(), QLineEdit::Normal, nickname, &ok);
    if(!ok)
        return;
    nickname = newname;
    std::cout << "Renaming: " << nickname.toStdString() << std::endl;
    name->setText(nickname);
    saveNickname();
}

void SoundButton::volumeChanged(int newvol)
{
    volume_level = newvol;
    if(player)
        player->setVolume(newvol*(*board_volume_level)/100);
}

void SoundButton::refreshVolume()
{
    if(player)
        player->setVolume(volume_level*(*board_volume_level)/100);
}

void SoundButton::setBalance(int b)
{
    if(player)
        player->setBalance(b);
}

void SoundButton::resizeEvent(QResizeEvent* event)
{
    QGroupBox::resizeEvent(event);
    event->accept();

    if(measured)
        return;
    measured = true;
    this->setMaximumWidth((int)event->size().width());
    this->setMaximumHeight((int)event->size().height());
}

void SoundButton::mousePressEvent(QMouseEvent * event)
{
    if(sound_file_path.length()>0 && id->geometry().contains(event->pos()))
        id->setStyleSheet("background-color: #" + CLR_PRESSED + "; "
                          + "border: 1px solid #" + CLR_TXT_DISABLED + "; color: #" + CLR_TXT_DISABLED);
    else if(!player || !player->isPlaying())
        this->setStyleSheet("background-color: #" + CLR_PRESSED);
}

void SoundButton::mouseReleaseEvent(QMouseEvent * event)
{
    const QPoint click = event->pos();
    //Change color back
    if(!player || !player->isPlaying())
    {
        if(sound_file_path.length()==0)
            this->setStyleSheet("background-color: #" + CLR_DISABLED);
        else
            this->setStyleSheet("background-color: #" + CLR_ENABLED);
        id->setStyleSheet("background-color: none; \
                          border: 1px solid #" + CLR_TXT_DISABLED + "; color: #" + CLR_TXT_DISABLED);
    }
    //Return if the user dragged away from the button
    if(click.x()<0 || click.x()>this->geometry().width() || click.y()<0 || click.y()>this->geometry().height())
        return;

    if(sound_file_path.length()==0)
    {
        changeSound();
    }
    else
    {
        //Pressing ID
        if(id->geometry().contains(click))
            popupMenu->exec(mapToGlobal(event->pos()));
        else
            pressKey();
    }
}

void  SoundButton::changeSound()
{
    QString fileSelect = QFileDialog::getOpenFileName(this->window(), tr("Choose sound for: ").append(ID), "",
                                                      tr("Sounds ").append(SoundButton::FILETYPES));
    if(fileSelect.length()>0)
        setMedia(fileSelect);
}

void SoundButton::pressKey()
{
    if(sound_file_path.length()==0 || !player)
    {
        this->setStyleSheet("background-color: #" + CLR_NOT_SET);
        return;
    }
    if(player->isPlaying())
    {
       if(repressedAction==REPRESSED_RESTART)
          player->seek(0);
       else
        {
            turnOff();
            player->pause();
        }
    }
    else
    {
        this->setStyleSheet("background-color: #" + CLR_PLAYING);
        if(player->play())
            turnError();
    }
}

void SoundButton::releaseKey()
{
    if(sound_file_path.length()==0)
    {
        this->setStyleSheet("background-color: #" + CLR_DISABLED);
        return;
    }
    if(releasedAction==RELEASED_STOP)
    {
      player->pause();
      player->seek(0);
    }
}

void SoundButton::playingFinished()
{
    std::cout << "Playing Finished: " << std::endl;
    if(doneAction==DONE_LOOP)
        pressKey();
    else
        turnOff();
}

void SoundButton::dragEnterEvent(QDragEnterEvent *event)
{
    this->setStyleSheet("background-color: #" + CLR_DRAG);
    event->accept();
}

void SoundButton::dragLeaveEvent(QDragLeaveEvent *event)
{
    if(player)
    {
        if(player->isPlaying())
            this->setStyleSheet("background-color: #" + CLR_PLAYING);
        else
            this->setStyleSheet("background-color: #" + CLR_ENABLED);
    }
    else
        this->setStyleSheet("background-color: #" + CLR_DISABLED);
}

#include <iostream>
void SoundButton::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->urls().length()!=1)
    {
        event->ignore();
        return;
    }

    QString path = event->mimeData()->urls().first().path();
#ifdef _WIN32
    if(path.startsWith("/"))
        path = path.mid(1);
#endif
    QFileInfo fileInfo(path);
    QString ext = fileInfo.suffix();

    if(FILETYPES.contains("." + ext.toLower()))
    {
        event->accept();
        setMedia(path);
    }
    else
        event->ignore();
}

void SoundButton::setMedia(QString newFile)
{
    QString temp = newFile.toUtf8();
    player = AudioPlayerFactory::createFromFile(temp.toStdString().c_str());

    if(player==NULL)
    {
         std::cerr << "ERROR: could not create AudioPlayer for: "  << newFile.toStdString() << std::endl;
         turnError();
         return;
    }
    this->setStyleSheet("background-color: #" + CLR_ENABLED);
    sound_file_path = newFile;

    std::cout << "FILE: " << boards_ini_file_path << std::endl;
    saveToFile();

    QFileInfo fileInfo(sound_file_path);

    if(nickname.length()==0)
    {
        QString qname(fileInfo.completeBaseName());
        name->setText(qname);
    }

    player->setVolume(volume_level);
    player->setFinishListener(this);
    volume->setEnabled(true);

    ((SoundBoard*)this->parentWidget())->breakVirgin();

#ifdef _WIN32
     ((AudioPlayerWin*)player)->setFinishListenerHWND((HWND)this->winId());
#endif
}


void SoundButton::saveToFile()
{
    if(!boards_ini_file_path)
    {
        ((SoundBoard*)this->parentWidget())->breakVirgin();
        return;
    }
    std::cout << "FILE: " << boards_ini_file_path->toStdString() << std::endl;

    const char c[] = { ID.toLatin1(), '\0' };
    if(sound_file_path.length()==0)
    {
        CIniFile::DeleteSection(std::string(c),boards_ini_file_path->toStdString());
        return;
    }
    CIniFile::SetValue("path",
                       sound_file_path.toStdString(),
                       std::string(c),
                       boards_ini_file_path->toStdString());
    saveVolume();
    saveNickname();
    saveDoneAction();
    saveReleaseAction();
    saveRepressAction();
}

void SoundButton::saveVolume()
{
    if(!boards_ini_file_path)
    {   //This should not happen
        ((SoundBoard*)this->parentWidget())->breakVirgin();
        return;
    }

    const char c[] = { ID.toLatin1(), '\0' };
    CIniFile::SetValue("vol",
                       QString("%1").arg(this->volume_level).toStdString(),
                       std::string(c),
                       boards_ini_file_path->toStdString());
}

void SoundButton::saveNickname()
{
    if(!boards_ini_file_path)
    {   //This should not happen
        ((SoundBoard*)this->parentWidget())->breakVirgin();
        return;
    }

    const char c[] = { ID.toLatin1(), '\0' };
    if(nickname.length()>0)
        CIniFile::SetValue("nick",
                           nickname.toStdString(),
                           std::string(c),
                           boards_ini_file_path->toStdString());
    else
        CIniFile::DeleteRecord("nick",
                               std::string(c),
                               boards_ini_file_path->toStdString());
}

void SoundButton::saveDoneAction()
{
    if(!boards_ini_file_path)
    {   //This should not happen
        ((SoundBoard*)this->parentWidget())->breakVirgin();
        return;
    }

    const char c[] = { ID.toLatin1(), '\0' };
    std::string out;
    switch(doneAction)
    {
    case DONE_STOP:
        out = "stop";
        break;
    case DONE_LOOP:
        out = "loop";
        break;
    }
    CIniFile::SetValue("done",
                       out,
                       std::string(c),
                       boards_ini_file_path->toStdString());
}

void SoundButton::saveReleaseAction()
{
    if(!boards_ini_file_path)
    {   //This should not happen
        ((SoundBoard*)this->parentWidget())->breakVirgin();
        return;
    }

    const char c[] = { ID.toLatin1(), '\0' };
    std::string out;
    switch(releasedAction)
    {
    case RELEASED_STOP:
        out = "stop";
        break;
    case RELEASED_CONTINUE:
        out = "continue";
        break;
    }
    CIniFile::SetValue("released",
                       out,
                       std::string(c),
                       boards_ini_file_path->toStdString());
}

void SoundButton::saveRepressAction()
{
    if(!boards_ini_file_path)
    {
        ((SoundBoard*)this->parentWidget())->breakVirgin();
        return;
    }

    const char c[] = { ID.toLatin1(), '\0' };
    std::string out;
    switch(repressedAction)
    {
    case REPRESSED_STOP:
        out = "stop";
        break;
    case REPRESSED_RESTART:
        out = "restart";
        break;
    }
    CIniFile::SetValue("repressed",
                       out,
                       std::string(c),
                       boards_ini_file_path->toStdString());
}

void SoundButton::menu_doneStop()
{
    doneGroup->actions().at(0)->setChecked(true);
    doneAction = DONE_STOP;
}

void SoundButton::menu_doneLoop()
{
    doneGroup->actions().at(1)->setChecked(true);
    doneAction = DONE_LOOP;
}

void SoundButton::menu_releasedStop()
{
    releasedGroup->actions().at(0)->setChecked(true);
    releasedAction = RELEASED_STOP;
}

void SoundButton::menu_releasedContinue()
{
    releasedGroup->actions().at(1)->setChecked(true);
    releasedAction = RELEASED_CONTINUE;
}

void SoundButton::menu_repressedStop()
{
    repressedGroup->actions().at(0)->setCheckable(true);
    repressedAction = REPRESSED_STOP;
}

void SoundButton::menu_repressedRestart()
{
    repressedGroup->actions().at(0)->setChecked(true);
    repressedAction = REPRESSED_RESTART;
}
