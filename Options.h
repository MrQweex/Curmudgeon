#ifndef OPTIONS_H
#define OPTIONS_H

#include "SoundButton.h"
#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QButtonGroup>
#include <iostream>
#include <QSettings>
#include <QApplication>

#define SETTINGS_FILE "CurmudgeonSettings"

class Options : public QDialog
{
    Q_OBJECT

private:
    QRadioButton *finish_stop, *finish_loop,
                 *released_stop, *released_continue,
                 *repressed_stop, *repressed_restart;

public:
    static SoundButton::_DONE_ACTION defaultDoneAction;
    static SoundButton::_RELEASED_ACTION defaultReleasedAction;
    static SoundButton::_REPRESSED_ACTION defaultRepressedAction;
    static QSettings* settings;
    static const int recentCount;
    static QString recentFiles[];

    static bool init_returnLoadLast()
    {
        if(!settings)
        {
            settings = new QSettings(QApplication::applicationDirPath() + tr(SETTINGS_FILE),
                             QSettings::NativeFormat);
            defaultDoneAction = settings->value("finish","stop")=="loop" ? SoundButton::DONE_STOP : SoundButton::DONE_LOOP;
            defaultReleasedAction = settings->value("released","continue")=="stop" ? SoundButton::RELEASED_STOP : SoundButton::RELEASED_CONTINUE;
            defaultRepressedAction = settings->value("repressed","stop")=="restart" ? SoundButton::REPRESSED_RESTART : SoundButton::REPRESSED_STOP;
            for(int i=0, j=0; i<6; i++)
            {
                recentFiles[j] = settings->value(QString("recent").append(QString("%1").arg(i))).toString();
                if(recentFiles[j].length()>0)
                    j;
            }
            return settings->value("loadLast","true")=="true";
        }
        return false;
    }

    static void writeRecent(QString* newRecent)
    {
        //If it already exists in the list, bump it to the front
        for(int i=Options::recentCount-1; i>=1; i--)
        {
            if(recentFiles[i]==*newRecent)
            {
                recentFiles[i] = recentFiles[i-1];
                recentFiles[i-1] = *newRecent;
                if(i==1)
                    return;
            }
        }
        //Otherwise bump all of them off
        for(int i=Options::recentCount-1; i>=1; i--)
        {
            recentFiles[i] = recentFiles[i-1];
        }
        recentFiles[0] = *newRecent;
    }


    Options(QWidget *parent) : QDialog(parent)
    {
        setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        setFixedSize(this->size());
        QVBoxLayout* layout = new QVBoxLayout();

        QGroupBox* defaults_ = new QGroupBox("Sound Defaults");
        QVBoxLayout* defaults_layout = new QVBoxLayout();
        {
            QHBoxLayout *radios = new QHBoxLayout();
            QLabel *label = new QLabel("When sounds finish:",defaults_);
            QRadioButton* checkA = new QRadioButton("Stop");
            QRadioButton* checkB = new QRadioButton("Loop");
            QButtonGroup *group = new QButtonGroup();
            group->addButton(checkA);
            group->addButton(checkB);
            connect(group,SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(radioSelect(QAbstractButton*)));

            radios->addWidget(checkA);
            radios->addWidget(checkB);
            defaults_layout->addWidget(label);
            defaults_layout->addLayout(radios);

            finish_stop = checkA;
            finish_loop = checkB;
            if(defaultDoneAction==SoundButton::DONE_STOP)
                finish_stop->setChecked(true);
            else
                finish_loop->setChecked(true);
        }
        {
            QHBoxLayout *radios = new QHBoxLayout();
            QLabel *label = new QLabel("When key is released:",defaults_);
            QRadioButton* checkA = new QRadioButton("Stop");
            QRadioButton* checkB = new QRadioButton("Continue");
            QButtonGroup *group = new QButtonGroup();
            group->addButton(checkA);
            group->addButton(checkB);
            connect(group,SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(radioSelect(QAbstractButton*)));

            radios->addWidget(checkA);
            radios->addWidget(checkB);
            defaults_layout->addWidget(label);
            defaults_layout->addLayout(radios);

            released_stop = checkA;
            released_continue = checkB;
            if(defaultReleasedAction==SoundButton::RELEASED_STOP)
                released_stop->setChecked(true);
            else
                released_continue->setChecked(true);
        }
        {
            QHBoxLayout *radios = new QHBoxLayout();
            QLabel *label = new QLabel("When key is re-pressed:",defaults_);
            QRadioButton* checkA = new QRadioButton("Stop");
            QRadioButton* checkB = new QRadioButton("Restart");
            QButtonGroup *group = new QButtonGroup();
            group->addButton(checkA);
            group->addButton(checkB);
            connect(group,SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(radioSelect(QAbstractButton*)));

            radios->addWidget(checkA);
            radios->addWidget(checkB);
            defaults_layout->addWidget(label);
            defaults_layout->addLayout(radios);

            repressed_stop = checkA;
            repressed_restart = checkB;
            if(defaultRepressedAction==SoundButton::REPRESSED_STOP)
                repressed_stop->setChecked(true);
            else
                repressed_restart->setChecked(true);
        }
        defaults_->setLayout(defaults_layout);
        layout->addWidget(defaults_);

        QCheckBox* autoOpen = new QCheckBox("Open soundboards from last session on start");
        autoOpen->setChecked(settings->value("loadLast","true")=="true");

        layout->addWidget(autoOpen);


        layout->setContentsMargins(20,20,20,20);
        layout->setSpacing(10);
        this->setLayout(layout);
        this->show();
    }

public slots:
    void radioSelect(QAbstractButton* select)
    {
        std::cout << "radio" << std::endl;
        if(select==finish_stop)
        {
            settings->setValue(tr("finish"), "stop");
            return;
        }
        if(select==finish_loop)
        {
            settings->setValue(tr("finish"), "loop");
            return;
        }

        if(select==released_stop)
        {
            settings->setValue(tr("released"), "stop");
            return;
        }
        if(select==released_continue)
        {
            settings->setValue(tr("released"), "continue");
            return;
        }

        if(select==repressed_stop)
        {
            settings->setValue(tr("repressed"), "stop");
            return;
        }
        if(select==repressed_restart)
        {
            settings->setValue(tr("repressed"), "restart");
            return;
        }
    }
};

#endif // OPTIONS_H
