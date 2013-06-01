#ifndef QSLIDERFIXEDSIZE_H
#define QSLIDERFIXEDSIZE_H

#include <QSlider>

class QSliderFixedSize : public QSlider
{
public:
    QSliderFixedSize() : tempValue(0) { QSlider(); measured = false;}
    int* tempValue;
private:
    bool measured;

    void setText(int i)
    {
        if(!measured)
            tempValue = new int(i);
        else
            QSlider:setValue(i);
    }
public:
    void resizeEvent(QResizeEvent* event)
    {
        QSlider::resizeEvent(event);
        event->accept();

        if(measured)
            return;
        measured = true;
        this->setMaximumWidth((int)event->size().width());
        this->setMaximumHeight((int)event->size().height());
        if(tempValue)
        {
            QSlider:setValue(*tempValue);
            delete tempValue;
        }
    }
};

#endif // QSLIDERFIXEDSIZE_H
