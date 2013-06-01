#ifndef QLABELWRAPELLIP_H
#define QLABELWRAPELLIP_H

#include <QLabel>
#include <QResizeEvent>
#include <QString>
#include <QGridLayout>
#include <iostream>
#include <QSizePolicy>

class QLabelWrapEllip : public QLabel
{
private:
    QString fullText;
    int maxWidth, maxHeight;
public:

    QLabelWrapEllip(QString) : maxWidth(0), maxHeight(0), measured(false)
    {
        QLabel(QString);
    }


    void setText(const QString &q)
    {
        fullText = q;

        int left, right, top, bottom;
        parentWidget()->getContentsMargins(&left, &top, &right, &bottom);
        maxWidth = this->parentWidget()->width() - left - right - 10;
        maxHeight = this->parentWidget()->height() - top - bottom;


        QFontMetrics metrics(font());
        QString ename = q;

        std::cout << metrics.width(fullText) << ">=" << maxWidth <<  "  " << this->width() << std::endl;
        if(metrics.width(fullText)>=maxWidth        )// && metrics.height())
        {
            //ename = metrics.elidedText(ename, Qt::ElideRight, maxWidth);
        }
        QLabel::setText(ename);
    }

    bool measured;
    void resizeEvent(QResizeEvent* event)
    {
        if(this->text().length()>0)
        std::cout << event->size().width() << "vs" << maxWidth
                  << "  " << this->parentWidget()->width()
                  << " " << this->text().toStdString() <<  std::endl;
        event->accept();
        if(measured)
            return;
        else
        {
            setMinimumSize(event->size());
        }
        measured=false;

        if((maxWidth>0 && event->size().width()>=maxWidth)
                ||(maxHeight>0 && event->size().height()>maxHeight))
        {
            QString qname = fullText;

            QFontMetrics metrics(font());
            qname = metrics.elidedText(qname, Qt::ElideRight, maxWidth);

            QLabel::setText(qname);
        }

    }
};

#endif // QLABELWRAPELLIP_H
