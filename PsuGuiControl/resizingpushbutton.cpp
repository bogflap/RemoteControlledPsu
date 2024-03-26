#include <QtMath>

#include "resizingpushbutton.h"

ResizingPushButton::ResizingPushButton(QWidget *parent)
    : QPushButton(parent)
    , firstTime(true)
{
}

void ResizingPushButton::resizeEvent(QResizeEvent *rEvent)
{
    Q_UNUSED(rEvent);

    QFont   f;
    QSize   hs;
    qreal   fLength;
    int     points;

    f = font();

    if (firstTime)
    {
        // Need to do this here because point size and etc. is not set until
        // after the constructor is called in the ui initialisation
        startLength = size().width();
        startPoints = f.pointSize();
        firstTime = false;
    }

    QSize   thisSize(rEvent->size());
    QSize   lastSize(rEvent->oldSize());

    if (thisSize != lastSize)
    {
        // In theory any previous change in point size should not have
        // changed the actual button size
        hs = rEvent->size();
        fLength = (qreal) hs.width();
        points = qFloor(((qreal) startPoints) * (fLength / startLength));
        f.setPointSize(points);
        setFont(f);
    }
}
