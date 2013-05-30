#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QTabWidget>
#include "SoundBoard.h"
#include "versiondialog.h"

#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void AddSoundboard(SoundBoard *sb);


private:
    Ui::MainWindow *ui;
    QVector<SoundBoard*> soundboards;
    QTabWidget* the_tabs;
    void keyPressEvents(QKeyEvent *);
    void keyReleaseEvents(QKeyEvent *);
    VersionDialog* m_versionDialog;

public slots:
    void newSoundboard();
    void openSoundboard();
    void closeSoundboard() { closeSoundboard(the_tabs->currentIndex()); }
    void closeSoundboard(int);
    void showAbout();

protected:
    bool winEvent ( MSG * message, long * result )
    {
        switch(message->message)
        {
        case 0x03b9:    //MM_MCINOTIFY
        case 0x01:      //MCI_NOTIFY_SUCCESS
        case 0x02:      //MCI_NOTIFY_SUPERSEDED
        case 0x04:      //MCI_NOTIFY_ABORTED
        case 0x05:      //MCI_NOTIFY_FAILURE
            SoundButton* apc = (SoundButton*) the_tabs->find(message->hwnd);
            apc->playingFinished();
            break;
        }
        //*/
        return QMainWindow::winEvent(message,result);
    }
};

#endif // MAINWINDOW_H
