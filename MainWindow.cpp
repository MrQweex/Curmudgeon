#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "SoundButton.h"

#include <QObjectList>
#include <iostream>

#include <QAction>
#include <QMessageBox>
     //Needed for if(typeid(...)==typeid(...))
#include <typeinfo>
#include <QSizeGrip>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_versionDialog(new VersionDialog(this))
{
    ui->setupUi(this);
    the_tabs = this->findChild<QTabWidget*>("mainTabs");
    the_tabs->removeTab(0);
    this->setWindowTitle("Curmudgeon");
    this->setAcceptDrops(true);
    this->setUnifiedTitleAndToolBarOnMac(true);
#ifdef __MAC__
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
            QAction* buy = new QAction(tr("Buy"),Help);
            //connect(zoom,SIGNAL(triggered()),this,SLOT(nextTab()));
            Help->addAction(buy);
        }
        this->menuBar()->addMenu(Help);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
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
    SoundBoard* current = (SoundBoard*)the_tabs->currentWidget();
    if(typeid(*current)==typeid(SoundBoard))
        (current)->pressKey(QChar(e->key()));
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
}

void MainWindow::openSoundboard()
{
    QString path = QFileDialog::getOpenFileName(this,"Select file",QString(),"Curmudgeon files (*.crmdgn)");
    if(path.length()==0)
        return;

    //If there is a new, blank soundboard open, close it
    if(the_tabs->count()==1
            && !soundboards.at(the_tabs->currentIndex())->getModified())
        the_tabs->removeTab(0);

    AddSoundboard(new SoundBoard(path));
}

bool MainWindow::saveSoundboard()
{
    QString fileSelect = QFileDialog::getSaveFileName(this,"Select filename",QString(),"Curmudgeon files (*.crmdgn)");
    if(fileSelect.length()==0)
        return false;
    (soundboards.at(the_tabs->currentIndex()))->saveToFile(fileSelect);
    return true;
}

void MainWindow::closeSoundboard(int index)
{
    if (soundboards.at(index)->getModified())
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

#include <QMessageBox>

void MainWindow::showAbout()
{
    QString title = tr("About Curmudgeon ");
    title.append(APP_VERSION);
    //QMessageBox::about(NULL, title, "BLAAAAAAH");

    if (!m_versionDialog) {
        m_versionDialog = new VersionDialog(this);
        connect(m_versionDialog, SIGNAL(finished(int)),
                this, SLOT(destroyVersionDialog()));
    }
    m_versionDialog->show();
    //*/
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    for(int i=0; i<soundboards.size(); i++)
    {
        if(soundboards[i]->getModified())
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
    event->accept();
}
