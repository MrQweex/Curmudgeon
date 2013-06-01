#include "SoundButton.h"

const QString
              SoundButton::CLR_ENABLED = "ddd",
              SoundButton::CLR_DISABLED = "eee",
              SoundButton::CLR_PRESSED = "aaa",
              SoundButton::CLR_PLAYING ="74C365",
              SoundButton::CLR_NOT_SET = "E6E0CF",
              SoundButton::CLR_TXT_ENABLED = "000",
              SoundButton::CLR_TXT_DISABLED = "888",
              SoundButton::CLR_DRAG = "BEE6B8";

#ifdef _WIN32 || __MAC__
const QString SoundButton::FILETYPES = "(*.wav *.mp3)";
#else
const QString SoundButton::FILETYPES = "(*.wav *.mp3 *.aif)";
#endif

SoundButton::SoundButton(QChar id)
{
    ID = id;
    init();
    volume->setValue(100);
}

SoundButton::SoundButton(QChar id,
                         QString path,
                         int vol,
                         QString nick
                         //,doneAction, releaseAction, repressAction
                         )
{
    ID = id;
    init();
    setMedia(path);
    volume_level = vol;
    volume->setValue(vol);
    nickname = nick;
    name->setText(nickname);
}

#include <QLayout>
#include <QSizePolicy>
void SoundButton::init()
{
    name = new QLabelWrapEllip(QString(""));
    name->setStyleSheet("background:none;");
    name->setAlignment(Qt::AlignTop);
    name->setWordWrap(true);

    id = new QLabel(QString(QChar(ID)));
    id->setStyleSheet("border: 1px solid #" + CLR_TXT_DISABLED + "; color: #" + CLR_TXT_DISABLED);

    volume = new QSliderFixedSize();
    volume->setMaximum(10);
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


#ifdef _WIN32
    this->createWinId();
#endif

    player = NULL;
    boards_ini_file_path = NULL;
}

void SoundButton::destroy()
{
    this->setStyleSheet("background-color: #" + CLR_DISABLED);
    name->setText(tr(""));
    volume->setEnabled(false);
    volume->setValue(100);
    volume_level=100;
    delete player;
    player = NULL;
    sound_file_path="";
    nickname="";
    if(boards_ini_file_path)
        saveToFile(boards_ini_file_path);   //should delete the section
}

void SoundButton::rename()
{
    bool ok;
    QString newname = QInputDialog::getText(this, tr("Rename"), QString(), QLineEdit::Normal, nickname, &ok);
    if(!ok)
        return;
    nickname = newname;
    name->setText(nickname);
    if(boards_ini_file_path)
        saveNickname(boards_ini_file_path);
}

void SoundButton::volumeChanged(int newvol)
{
    volume_level = newvol;
    if(player)
        player->setVolume(newvol);
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
    QString fileSelect = QFileDialog::getOpenFileName(this->window(), tr("Choose sound for: ").append(ID),
                                                     "",
                                                      tr("Sounds ").append(FILETYPES));
   if(fileSelect.length()>0)
    {
       setMedia(fileSelect);
    }
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
    {
        this->setStyleSheet("background-color: #" + CLR_DISABLED);
        return;
    }
    //if(remain_depressed)
    //  player.stop();
}

void SoundButton::playingFinished(){
    std::cout << "Playing Finished" << std::endl;

    //TODO: This needs to be moved inside the APOsx class
#ifdef __MAC__
    player->seek(0);
#endif

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

#include <QFontMetrics>

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

    if(nickname.length()==0)
    {
        QString qname(fileInfo.completeBaseName());
        name->setText(qname);
    }

    player->setVolume(volume_level);
    volume->setEnabled(true);

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
    saveNickname(filePath);
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

void SoundButton::saveNickname(QString* filePath)
{
    const char c[] = { ID.toAscii(), '\0' };
    if(nickname.length()>0)
        CIniFile::SetValue("nick",
                           nickname.toStdString(),
                           std::string(c),
                           filePath->toStdString());
    else
        CIniFile::DeleteRecord("nick",
                               std::string(c),
                               filePath->toStdString());
}
