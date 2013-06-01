#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "SoundButton.h"

#include <QObjectList>
#include <iostream>

#include <QAction>
#include <QMessageBox>
     //Needed for if(typeid(...)==typeid(...))
#include <typeinfo>

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
    this->setWindowIcon(QIcon());
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
            && !soundboards.at(the_tabs->currentIndex())->getModified()
            && soundboards.at(the_tabs->currentIndex())->isVirgin())
        the_tabs->removeTab(0);

    AddSoundboard(new SoundBoard(path));
}

void MainWindow::saveSoundboard()
{
    QString fileSelect = QFileDialog::getSaveFileName(this,"Select filename",QString(),"Curmudgeon files (*.crmdgn)");
    if(fileSelect.length()==0)
        return;
    std::cout << fileSelect.toStdString() << std::endl;
    (soundboards.at(the_tabs->currentIndex()))->saveToFile(fileSelect);
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



/*TODO:
 *  -SoundButon:
 *      -"Set Label"
 *      -"Change Sound"
 *      -"Remove"
 *      -Volume Slider
 *      -"Stop when done" vs "Loop when done"
 *      -"Stop when released" vs "Continue when released"
 *      -"Restart when re-pressed" vs "Stop when re-pressed"
 *
 *  -SoundBoard:
 *      -Close
 *          -"Are you sure you want to close?
 *      -Save to file
 *
 *  -Menu:
 *      -File
 *          -Open Soundboard
 *          -Save Soundboard
 *      -View
 *          -Next Soundboard
 *          -Previous Soundboard
 *      -Open
 *      -Dock menu?
 *          -qt_mac_set_dock_menu(QMenu *)
 *
 *  -Window: resize
 */

/*Tabs for each different soundboard
 *  -What happens when change tabs? do sounds keep playing?
 *
 *FOR EACH BUTTON
 *  If it is not set:
 *  (1) Press to browse
 *  (2) Dragndrop
 *  ~(3) Record?
 *  If it is set:
 *  (1) Play/Pause
 *
 *  _____________
 * | NAME      / |
 * |           / |  <- volume
 * |             |
 * | time     ID |
 * _______________
 *
 *CONFIG:
 *  -Duck
 *      -%
 *      -Duration
 *  -Should sounds...
 *      -Loop?
 *      -Only play when depressed?
 *      -Stop or restart when re-pressed?
 *  -Fade Out
 *
 *
 *MAIN WINDOW:
 *  -Duck
 *  -Play/Pause
 *  -Main Volume
 *  -Balance
 *
 *~Android Remote
*/
