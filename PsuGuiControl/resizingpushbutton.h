#ifndef RESIZINGPUSHBUTTON_H
#define RESIZINGPUSHBUTTON_H

#include <QPushButton>
#include <QResizeEvent>

class ResizingPushButton : public QPushButton
{
    Q_OBJECT

public:
    ResizingPushButton(QWidget *parent = nullptr);

protected:
    void    resizeEvent(QResizeEvent* rEvent);

private:
    qreal   startLength;
    int     startPoints;
    bool    firstTime;
};

#endif // RESIZINGPUSHBUTTON_H
