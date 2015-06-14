#ifndef COVCALENDARSCENE_H
#define COVCALENDARSCENE_H

#include <QGraphicsScene>

class CovCalendarScene : public QGraphicsScene
{
public:
    CovCalendarScene(QObject *aparent);
protected:
    virtual void drawBackground (QPainter * painter, const QRectF &aRect);
};

#endif // COVCALENDARSCENE_H
