#include "SoundButton.h"

const QString
              SoundButton::CLR_ENABLED = "ddd",
              SoundButton::CLR_DISABLED = "eee",
              SoundButton::CLR_PRESSED = "aaa",
              SoundButton::CLR_PLAYING ="74C365",
              SoundButton::CLR_TXT_ENABLED = "000",
              SoundButton::CLR_TXT_DISABLED = "888",
              SoundButton::CLR_DRAG = "BEE6B8";

const QString SoundButton::FILETYPES = "(*.wav;*.mp3)";

SoundButton::SoundButton(QChar id)
{
    ID = id;
    init();
}

SoundButton::SoundButton(QChar id,
                         QString path,
                         int vol
                         //,doneAction, releaseAction, repressAction
                         )
{
    ID = id;
    init();
    setMedia(path);
}

void SoundButton::init()
{
    layout = new QGridLayout();
    name = new QLabel(QString(""));
    id = new QLabel(QString(QChar(ID)));
    id->setStyleSheet("border: 1px solid #" + CLR_TXT_DISABLED + "; color: #" + CLR_TXT_DISABLED);
    name->setStyleSheet("background:none;");
    name->setAlignment(Qt::AlignTop);
    name->setWordWrap(true);

    layout->addWidget(name,0,0);
    layout->addWidget(id,1,1);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setColumnStretch(0,100);
    layout->setRowStretch(0,100);
    this->setLayout(layout);
    this->setStyleSheet("background-color: #" + CLR_DISABLED);
    this->setAcceptDrops(true);
#ifdef _WIN32
         this->createWinId();
#endif

    player = NULL;
    boards_ini_file_path = NULL;
}

void SoundButton::mousePressEvent(QMouseEvent * event)
{
    if(!player || !player->isPlaying())
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
    }
    //Return if the user dragged away from the button
    if(click.x()<0 || click.x()>this->geometry().width() || click.y()<0 || click.y()>this->geometry().height())
        return;

    if(sound_file_path.length()==0)
    {
        QString fileSelect = QFileDialog::getOpenFileName(this, tr("Choose sound for: ").append(ID),
                                                         "",
                                                          tr("Sounds ").append(FILETYPES));
       if(fileSelect.length()>0)
        {
           setMedia(fileSelect);
        }
    }
    else
    {
        pressKey();
    }
}

void SoundButton::pressKey()
{
    if(sound_file_path.length()==0 || !player)
        return;
    if(player->isPlaying())
    {
    //   if(restart)
    //      player.seekTo(0);
    //   else
        {
            turnOff();
            player->pause();
        }
    }
    else
    {
        this->setStyleSheet("background-color: #" + CLR_PLAYING);
        player->play();
    }
    //loop is taken care of by 'setLoop()'
}

void SoundButton::releaseKey()
{
    if(sound_file_path.length()==0)
        return;
    //if(remain_depressed)
    //  player.stop();
}

void SoundButton::playingFinished(){
    std::cout << "Playing Finished" << std::endl;
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
    player = AudioPlayerFactory::createFromFile(newFile.toStdString().c_str());
    if(player==NULL)
    {
         std::cerr << "ERROR: could not create AudioPlayer for: "  << newFile.toStdString() << std::endl;
         return;
    }
    player->setFinishListener(this);
    this->setStyleSheet("background-color: #" + CLR_ENABLED);
    sound_file_path = newFile;
    QFileInfo fileInfo(sound_file_path);
    QString qname(fileInfo.baseName());
    name->setText(qname);

#ifdef _WIN32
     ((AudioPlayerWin*)player)->setFinishListenerHWND(this->winId());
     //((AudioPlayerWin*)player)->setAlias()
#endif
}

void SoundButton::saveToFile(QString* filePath)
{
    boards_ini_file_path = filePath;
    const char c[] = { ID.toAscii(), '\0' };
    if(sound_file_path.length()==0)
    {
        CIniFile::DeleteSection(std::string(c),filePath->toStdString());
        return;
    }
    CIniFile::SetValue("path",
                       sound_file_path.toStdString(),
                       std::string(c),
                       filePath->toStdString());
    saveVolume(filePath);
    //saveName
    //saveDoneAction
    //saveReleaseAction
    //saveRepressAction
}

void SoundButton::saveVolume(QString* filePath)
{
    const char c[] = { ID.toAscii(), '\0' };
    CIniFile::SetValue("vol",
                       QString("%1").arg(this->volume_level).toStdString(),
                       std::string(c),
                       filePath->toStdString());
}
