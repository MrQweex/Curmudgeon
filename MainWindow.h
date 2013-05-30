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
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    VersionDialog* m_versionDialog;

public slots:
    void newSoundboard();
    void openSoundboard();
    void saveSoundboard();
    void closeSoundboard() { closeSoundboard(the_tabs->currentIndex()); }
    void closeSoundboard(int);
    void showAbout();
    void nextTab();
    void prevTab();

};

#endif // MAINWINDOW_H
