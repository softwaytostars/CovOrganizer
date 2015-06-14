#include "covcalendarscene.h"

#include <QPainter>
#include <QDebug>

CovCalendarScene::CovCalendarScene(QObject *aparent) : QGraphicsScene (aparent)
{
}


void CovCalendarScene::drawBackground ( QPainter * painter, const QRectF &aRect)
{
    //qDebug() << aRect.width() << aRect.height() <<width()<< height();
    painter->drawRect(0, 0, width(), height());
    //QGraphicsScene::drawBackground(painter, aRect);
}
