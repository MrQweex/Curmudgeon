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

#define SETTINGS_FILE "config.ini"

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
    static bool loadLast;

    static void init()
    {
        if(!settings)
        {
            //settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Qweex","Curmudgeon");
            settings = new QSettings(QApplication::applicationDirPath().append(QDir::separator()).append(SETTINGS_FILE) , QSettings::IniFormat);
            std::cout << QApplication::applicationDirPath().append(QDir::separator()).append(SETTINGS_FILE).toStdString() << std::endl;
//            QApplication::applicationDirPath() + tr("/") + tr(SETTINGS_FILE), QSettings::NativeFormat);
            defaultDoneAction = settings->value("finish","stop")=="loop" ? SoundButton::DONE_LOOP : SoundButton::DONE_STOP;
            defaultReleasedAction = settings->value("released","continue")=="stop" ? SoundButton::RELEASED_STOP : SoundButton::RELEASED_CONTINUE;
            defaultRepressedAction = settings->value("repressed","stop")=="restart" ? SoundButton::REPRESSED_RESTART : SoundButton::REPRESSED_STOP;
            for(int i=0, j=0; i<6; i++)
            {
                recentFiles[j] = settings->value(QString("recent").append(QString("%1").arg(i))).toString();
                if(recentFiles[j].length()>0)
                    j++;
            }
            loadLast = settings->value("loadLast","true")!="false";
        }
    }

    static void writeRecent(QString* newRecent)
    {
        //If it already exists in the list, bump it to the front
        for(int i=Options::recentCount-1; i>=0; i--)
        {
            std::cout << recentFiles[i].toStdString() << "==" << newRecent->toStdString() << std::endl;
            if(recentFiles[i]==*newRecent)
            {
                std::cout << "FOUND" << std::endl;
                recentFiles[i] = recentFiles[i-1];
                recentFiles[i-1] = *newRecent;
                std::cout << "FOUND" << std::endl;
                if(i==1)
                    return;
            }
        }
        //Otherwise bump all of them off
        for(int i=Options::recentCount-1; i>=1; i--)
        {
            recentFiles[i] = recentFiles[i-1];
            settings->setValue(QString("recent%1").arg(i), recentFiles[i]);
        }
        recentFiles[0] = *newRecent;
        settings->setValue(QString("recent0"), recentFiles[0]);
    }


    Options(QWidget *parent) : QDialog(parent)
    {
        setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        QVBoxLayout* layout = new QVBoxLayout();\
        layout->setSizeConstraint(QLayout::SetFixedSize);

        int id=0;
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
            group->setId(checkA, id++);
            group->setId(checkB, id++);
            connect(group,SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(radioSelect(QAbstractButton*)));

            radios->addWidget(checkA);
            radios->addWidget(checkB);
            defaults_layout->addWidget(label);
            defaults_layout->addLayout(radios);

            finish_stop = checkA;
            finish_loop = checkB;
            std::cout << defaultDoneAction << "==" << SoundButton::DONE_LOOP << std::endl;
            if(defaultDoneAction==SoundButton::DONE_LOOP)
                finish_loop->setChecked(true);
            else
                finish_stop->setChecked(true);
        }
        {
            QHBoxLayout *radios = new QHBoxLayout();
            QLabel *label = new QLabel("When key is released:",defaults_);
            QRadioButton* checkA = new QRadioButton("Stop");
            QRadioButton* checkB = new QRadioButton("Continue");
            QButtonGroup *group = new QButtonGroup();
            group->addButton(checkA);
            group->addButton(checkB);
            group->setId(checkA, id++);
            group->setId(checkB, id++);
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
            group->setId(checkA, id++);
            group->setId(checkB, id++);
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
        connect(autoOpen,SIGNAL(toggled(bool)),this,SLOT(loadLastToggle(bool)));
        layout->addWidget(autoOpen);


        layout->setContentsMargins(20,20,20,20);
        layout->setSpacing(10);
        this->setLayout(layout);
        this->setWindowTitle("Config");
        this->exec();
    }

public slots:
    void radioSelect(QAbstractButton* select)
    {
        if(select==finish_stop)
        {
            std::cout << "derp" << std::endl;
            defaultDoneAction = SoundButton::DONE_STOP;
            settings->setValue(tr("finish"), "stop");
            return;
        }
        if(select==finish_loop)
        {
            settings->setValue(tr("finish"), "loop");
            defaultDoneAction = SoundButton::DONE_LOOP;
            return;
        }

        if(select==released_stop)
        {
            settings->setValue(tr("released"), "stop");
            defaultReleasedAction = SoundButton::RELEASED_STOP;
            return;
        }
        if(select==released_continue)
        {
            settings->setValue(tr("released"), "continue");
            defaultReleasedAction = SoundButton::RELEASED_CONTINUE;
            return;
        }

        if(select==repressed_stop)
        {
            settings->setValue(tr("repressed"), "stop");
            defaultRepressedAction = SoundButton::REPRESSED_STOP;
            return;
        }
        if(select==repressed_restart)
        {
            settings->setValue(tr("repressed"), "restart");
            defaultRepressedAction = SoundButton::REPRESSED_RESTART;
            return;
        }
    }
    void loadLastToggle(bool x)
    {
        settings->setValue(tr("loadLast"), x ? "true" : "false");
    }
};

#endif // OPTIONS_H
