#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QTabWidget>
#include "SoundBoard.h"
#include "VersionDialog.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>

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

private:
    Ui::MainWindow *ui;
    QVector<SoundBoard*> soundboards;
    QTabWidget* the_tabs;
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    VersionDialog* m_versionDialog;

public slots:
    void newSoundboard();
    void openSoundboard();
    bool saveSoundboard();  //true if it was saved, false otherwise
    void closeSoundboard() { closeSoundboard(the_tabs->currentIndex()); }
    void closeSoundboard(int);
    void showAbout();
    void showLicense();
    void nextTab();
    void prevTab();
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
