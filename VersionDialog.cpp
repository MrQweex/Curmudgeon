/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "VersionDialog.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include <iostream>


VersionDialog::VersionDialog(QWidget *parent) : QDialog(parent)
{
    QString iconpath = ":/logo/images/logo_192.png";

    // We need to set the window icon explicitly here since for some reason the
    // application icon isn't used when the size of the dialog is fixed (at least not on X11/GNOME)
    setWindowIcon(QIcon(iconpath));

    setWindowTitle(versionString);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

     const QString description = tr(
        "<h3>%1</h3>"
        "Based on Qt %2 (%3-bit)<br/>"
        "<br/>"
        "Built on %4 %5<br />"
        "<br/>"
        "Copyright 2013-%6 %7. All rights reserved.<br/>"
        "<br/>"
        "The program released under the <a href='http://opensource.org/licenses/QPL-1.0'>Q Public License</a>.<br/>")
        .arg(versionString,
             QLatin1String(qVersion()),
             QString::number(QSysInfo::WordSize),
             QLatin1String(__DATE__), QLatin1String(__TIME__),
             copyrightYear,
             copyrightHolder);

    QLabel *copyRightLabel = new QLabel(description);
    copyRightLabel->setWordWrap(true);
    copyRightLabel->setOpenExternalLinks(true);
    copyRightLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
    //QTC_CHECK(cl  oseButton);
    buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
    connect(buttonBox , SIGNAL(rejected()), this, SLOT(reject()));

    QLabel *logoLabel = new QLabel;
    logoLabel->setPixmap(QPixmap(iconpath));
    layout->addWidget(logoLabel , 0, 0, 1, 1);
    layout->addWidget(copyRightLabel, 0, 1, 4, 4);
    layout->addWidget(buttonBox, 4, 0, 1, 5);
    //*/
}

#include <QTextEdit>
#include <QFile>
#include <QVBoxLayout>
#include <QFontMetrics>

void VersionDialog::License()
{
    QDialog* license = new QDialog();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetFixedSize);

    //Title
    QLabel* title = new QLabel("Q Public License 1.0", license);
    QFont font = title->font();
    font.setPointSize(20);
    font.setBold(true);
    title->setFont(font);

    //Derp
    QTextEdit* text = new QTextEdit();
    QFile data(":/res/LICENSE.QPL");
    data.open(QIODevice::ReadOnly);
    text->setText(data.readAll());
    QFontMetrics qfm(text->font());
    text->setMinimumSize(
                qfm.width("The intent of this license is to establish freedom to share and change the")+10,
                qfm.height()*30);
    //text->setMinimumSize(500,400);
    text->setReadOnly(true);

    //Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
    buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
    connect(buttonBox , SIGNAL(rejected()), license, SLOT(reject()));

    layout->addWidget(title);
    layout->addWidget(text, 4);
    layout->addWidget(buttonBox, 0, Qt::AlignRight);

    //license->setGeometry(300,400,800,600);
    license->setLayout(layout);
    license->show();
}
