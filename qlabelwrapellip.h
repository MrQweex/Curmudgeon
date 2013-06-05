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
//        setWordWrap(true);
    }


    void setText(const QString &q)
    {
        fullText = q;

        int left, right, top, bottom;
        parentWidget()->getContentsMargins(&left, &top, &right, &bottom);
//        maxWidth = this->parentWidget()->width() - left - right - 15;
//        maxHeight = this->parentWidget()->height() - top - bottom;
        maxWidth = this->size().width();
        maxHeight = this->size().height();


        QFontMetrics metrics(font());
        QString ename = q;

        int currentChar = 0, w=0, h=1;
        int lastX = -1;
        while(currentChar<ename.length())
        {
            w = 0;
            while(w<maxWidth && currentChar<ename.length())
            {
                currentChar++;
                w+=metrics.width(ename[currentChar]);
            }
            w-=metrics.width(ename[currentChar]);
            currentChar--;

            int x = ename.lastIndexOf(" ", currentChar);
            if(x<=lastX || x==-1)
            {
                w+=metrics.width(ename[currentChar]);
                ename = ename.insert(currentChar,"-");
                x = currentChar+1;
                //while(ename[currentChar]!=' ' && currentChar<q.length())
                //    x=++currentChar;
            }

            if(currentChar==ename.length()-1)
                break;

            h++;
            if(h*metrics.height()>maxHeight)
            {
                ename = ename.left(currentChar);
                int y = metrics.width(QString::fromUtf8("\u2026"));
                while((w+y)>maxWidth)
                {
                    currentChar--;
                    w-= metrics.width(ename[currentChar]);
                    ename = ename.left(currentChar);
                }
                ename = ename.append(QString::fromUtf8("\u2026"));  //… 8230
                break;
            }
            ename = ename.replace(x,1,'\n');
            lastX = x;
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
