#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QTabWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopServices>
#include "SoundBoard.h"
#include "VersionDialog.h"

#ifdef SUPPORT_THE_DEV
    #include "Purchase/Purchase.h"
#endif

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
    void modifiedFile(SoundBoard* x)
    {
        the_tabs->setTabText(the_tabs->currentIndex(),
                             QString("*").append
                                (the_tabs->tabText(the_tabs->currentIndex()))
                             );
    }
    void updateRecentMenu()
    {
        recentMenu->clear();

        for(int i=0; i<Options::recentCount-1; i++)
        {
            if(Options::recentFiles[i].length()==0)
                break;
            QAction* r = new QAction(Options::recentFiles[i],recentMenu);
            recentMenu->addAction(r);
        }
        QAction* clear = new QAction("Clear", recentMenu);
        recentMenu->addAction(clear);

        connect(recentMenu,SIGNAL(triggered(QAction*)),this,SLOT(openRecentFile(QAction*)));
    }

private:
    Ui::MainWindow *ui;
    QVector<SoundBoard*> soundboards;
    QTabWidget* the_tabs;
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    VersionDialog* m_versionDialog;
    QMenu* recentMenu;

public slots:
    void newSoundboard();
    void openSoundboard();
    bool saveSoundboard();  //true if it was saved, false otherwise
    void closeSoundboard() { closeSoundboard(the_tabs->currentIndex()); }
    void closeSoundboard(int);
    void showAbout();
    void showLicense();
    void showPreferences() { new Options(this); }
    void nextTab();
    void prevTab();
    void closeEvent(QCloseEvent *event);
    void openRecentFile(QAction*);
#ifdef SUPPORT_THE_DEV
    void buy()
    {
        QDesktopServices::openUrl(QUrl("http://curmudgeon-app.com/buy"));
    }
    void licenseKey()
    {
        Purchase::showLicenseDialog(this);
    }

#endif

};

#endif // MAINWINDOW_H
