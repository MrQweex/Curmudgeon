#include <QObjectList>
#include <QAction>
#include <QMessageBox>
#include <typeinfo>
#include <QSizeGrip>
#include <QMenuBar>

#include <iostream>
#include <typeinfo>

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "SoundButton.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_versionDialog(new VersionDialog(this))
{
    ui->setupUi(this);
    the_tabs = this->findChild<QTabWidget*>("mainTabs");
    the_tabs->setDocumentMode(true);
    the_tabs->removeTab(0);
    this->setWindowTitle("Curmudgeon");
    this->setAcceptDrops(true);
    this->setUnifiedTitleAndToolBarOnMac(true);
#ifdef __APPLE__
    this->setWindowIcon(QIcon());
#endif
    this->setFixedSize(size());
    this->statusBar()->setSizeGripEnabled(false);


    {
        QMenu* File = new QMenu("File");
        {
            QAction* newS = new QAction(tr("New Soundboard"),File);
            newS->setShortcut(tr("Ctrl+N"));
            connect(newS,SIGNAL(triggered()),this,SLOT(newSoundboard()));
            File->addAction(newS);
        }
        {
            QAction* open = new QAction(tr("Open Soundboard"),File);
            open->setShortcut(tr("Ctrl+O"));
            connect(open,SIGNAL(triggered()),this,SLOT(openSoundboard()));
            File->addAction(open);
        }
        {
            QAction* save = new QAction(tr("Save Soundboard"),File);
            save->setShortcut(tr("Ctrl+N"));
            connect(save,SIGNAL(triggered()),this,SLOT(saveSoundboard()));
            File->addAction(save);
        }
        {
            QAction* close = new QAction(tr("Close Soundboard"),File);
            close->setShortcut(tr("Ctrl+W"));
            connect(close,SIGNAL(triggered()),this,SLOT(closeSoundboard()));
            File->addAction(close);
        }
        File->addSeparator();
        {
            recentMenu = new QMenu("Open Recent");
            updateRecentMenu();
            File->addMenu(recentMenu);
        }
        {
            QAction* preferences = new QAction(tr("Preferences"),File);
            connect(preferences,SIGNAL(triggered()),this,SLOT(showPreferences()));
            File->addAction(preferences);
        }
        {
            QAction* quit = new QAction(tr("Quit Soundboard"),File);
            quit->setShortcut(tr("Ctrl+Q"));
            connect(quit,SIGNAL(triggered()),this,SLOT(close()));
            File->addAction(quit);
        }
        this->menuBar()->addMenu(File);
    }
    {
        QMenu* View = new QMenu("View");
        {
            QAction* next = new QAction(tr("Next Soundboard"),View);
            next->setShortcut(tr("Ctrl+Tab"));
            connect(next,SIGNAL(triggered()),this,SLOT(prevTab()));
            View->addAction(next);
        }
        {
            QAction* previous = new QAction(tr("Previous Soundboard"),View);
            previous->setShortcut(tr("Ctrl+Shift+Tab"));
            connect(previous,SIGNAL(triggered()),this,SLOT(nextTab()));
            View->addAction(previous);
        }
        this->menuBar()->addMenu(View);
    }
#ifdef __MAC__
    {
        QMenu* Window = new QMenu("Window");
        {
            QAction* minimize = new QAction(tr("Minimize"),Window);
            minimize->setShortcut(tr("Ctrl+M"));
            connect(minimize,SIGNAL(triggered()),this,SLOT(showMinimized());
            Window->addAction(minimize);
        }
        {
            QAction* zoom = new QAction(tr("Zoom"),Window;
            //previous->setShortcut(tr("Ctrl+Shift+Tab"));
            //connect(zoom,SIGNAL(triggered()),this,SLOT(nextTab()));
            Window->addAction(zoom);
        }
        this->menuBar()->addMenu(Window);
    }
#endif
    {
        QMenu* Help = new QMenu("Help");
        {
            QAction* about = new QAction(tr("About"),Help);
            connect(about,SIGNAL(triggered()),this,SLOT(showAbout()));
            Help->addAction(about);
        }
        {
            QAction* license = new QAction(tr("License"),Help);
            connect(license,SIGNAL(triggered()),this,SLOT(showLicense()));
            Help->addAction(license);
        }
        this->menuBar()->addMenu(Help);

#ifdef SUPPORT_THE_DEV
        if(Purchase::keyIsValid())
        {
            QAction* key = new QAction(tr("View License Info"),Help);
            connect(key,SIGNAL(triggered()),this,SLOT(licenseKey()));
            Help->addAction(key);
        }
        else
        {
            QMenu* dev = new QMenu("Support the Dev");
            {
                QAction* buy = new QAction("Buy",dev);
                connect(buy,SIGNAL(triggered()),this,SLOT(buy()));
                dev->addAction(buy);
            }
            {
                QAction* key = new QAction("Enter License Key",dev);
                connect(key,SIGNAL(triggered()),this,SLOT(licenseKey()));
                dev->addAction(key);
            }
            this->menuBar()->addMenu(dev);
        }
#endif
    }
}

MainWindow::~MainWindow()
{
    std::cout << "Destroy: Window" << std::endl;
    //delete ui;
}

void MainWindow::AddSoundboard(SoundBoard *newTab)
{
    the_tabs->addTab(newTab, newTab->getName());
    soundboards << newTab;
}

#include <iostream>
void MainWindow::keyPressEvent(QKeyEvent * e)
{
    e->accept();
    if(!e->isAutoRepeat())
    {
        SoundBoard* current = (SoundBoard*)the_tabs->currentWidget();
        if(typeid(*current)==typeid(SoundBoard))
            (current)->pressKey(QChar(e->key()));
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent * e)
{
    e->accept();
    SoundBoard* current = (SoundBoard*)the_tabs->currentWidget();
    if(typeid(*current)==typeid(SoundBoard))
        (current)->releaseKey(QChar(e->key()));
}

void MainWindow::newSoundboard()
{
    AddSoundboard(new SoundBoard());
    the_tabs->setCurrentIndex(the_tabs->count()-1);
}

void MainWindow::openSoundboard()
{
    QString path = QFileDialog::getOpenFileName(this,"Select file",QString(),"Curmudgeon files (*.crmdgn)");
    if(path.length()==0)
        return;
    Options::writeRecent(&path);
    updateRecentMenu();
    for(int i=0; i<soundboards.count(); i++)
    {
        if(soundboards[i]->getPath()==path)
        {
            the_tabs->setCurrentIndex(i);
            return;
        }
    }

    //If there is a new, blank soundboard open, close it
    if(the_tabs->count()==1
            && soundboards.first()->isVirgin())
    {
        std::cout << "DERP" << soundboards.at(the_tabs->currentIndex())->shouldPromptToSave() << std::endl;
        the_tabs->removeTab(0);
    }

    AddSoundboard(new SoundBoard(path));
    the_tabs->setCurrentIndex(the_tabs->count()-1);
}

bool MainWindow::saveSoundboard()
{
    QString fileSelect = QFileDialog::getSaveFileName(this,"Select filename",QString(),"Curmudgeon files (*.crmdgn)");
    if(fileSelect.length()==0)
        return false;
    (soundboards.at(the_tabs->currentIndex()))->saveToFile(fileSelect);
    Options::writeRecent(&fileSelect);
    updateRecentMenu();
    return true;
}

void MainWindow::closeSoundboard(int index)
{
    if (soundboards.at(index)->shouldPromptToSave())
    {
        int userAnswer = QMessageBox::question(this, "Unsaved Soundboard", "Would you like to save before closing?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(QMessageBox::Yes == userAnswer)
            saveSoundboard();
        else if(QMessageBox::Cancel == userAnswer)
            return;
    }
    the_tabs->removeTab(index);
    if(the_tabs->count()==0)
        newSoundboard();
    soundboards.remove(index);
}

void MainWindow::nextTab()
{
    int i= the_tabs->currentIndex()+1;
    if(i==the_tabs->count())
        i=0;
    the_tabs->setCurrentIndex(i);
}

void MainWindow::prevTab()
{
    int i= the_tabs->currentIndex()-1;
    if(i<0)
        i=the_tabs->count()-1;
    the_tabs->setCurrentIndex(i);
}

void MainWindow::showAbout()
{
    QString title = tr("About Curmudgeon ");
    title.append(APP_VERSION);

    if (!m_versionDialog) {
        m_versionDialog = new VersionDialog(this);
        connect(m_versionDialog, SIGNAL(finished(int)),
                this, SLOT(destroyVersionDialog()));
    }
    m_versionDialog->show();
    //*/
}

void MainWindow::showLicense()
{
    VersionDialog::License();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for(int i=0; i<soundboards.size(); i++)
    {
        if(soundboards[i]->shouldPromptToSave())
        {
            the_tabs->setCurrentIndex(i);
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this, QApplication::applicationName(),
                                          soundboards[i]->getName().append(" is not saved.\n\n")
                                          + tr("Would you like to save it?"),
                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes)
            {
                if(!this->saveSoundboard())
                {
                    event->ignore();
                    return;
                }
            }
            else if (reply == QMessageBox::Cancel)
            {
                event->ignore();
                return;
            }
        }
    }
    //Delete the old saved tabs
    {
        int count = Options::settings->value("lastCount").toInt();
        for(int i=0; i<count; i++)
            Options::settings->remove(QString("last%1").arg(i));
        Options::settings->remove("lastCount");
    }

    //Save open tabs
    {
        int count = 0;
        int countDup = soundboards.size();
        for(int i=0; i<countDup; i++)
        {
            if(soundboards[i]->getPath().length()>0)
            {
                Options::settings->setValue(QString("last%1").arg(count++), soundboards[i]->getPath());
            }
        }
        Options::settings->setValue("lastCount", QString("%1").arg(count));
    }
    event->accept();
}

void MainWindow::openRecentFile(QAction* a)
{
    if(a->text()=="Clear")
    {
        for(int i=0; i<Options::recentCount; i++)
        {
            Options::recentFiles[i] = "";
            Options::settings->setValue(QString("recent%1").arg(i), Options::recentFiles[i]);
        }
        updateRecentMenu();
        return;
    }

    if(the_tabs->count()==1
            && soundboards.at(the_tabs->currentIndex())->isVirgin())
        the_tabs->removeTab(0);

    QString* fucktwit = new QString(a->text());
    Options::writeRecent(fucktwit);
    delete fucktwit;
    updateRecentMenu();
    std::cout << "dsadsadsD" << std::endl;

    //Check if it is already open
    for(int i=0; i<soundboards.size(); i++)
    {
        if(soundboards[i]->getPath()==a->text())
        {
            the_tabs->setCurrentIndex(i);
            return;
        }
    }
    AddSoundboard(new SoundBoard(a->text()));
}
