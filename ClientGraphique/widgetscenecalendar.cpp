#include "widgetscenecalendar.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QTextDocument>
#include <QTextOption>
#include <QDebug>
#include "ControleurAffichage/igestionframecalendar.h"

qreal WidgetSceneCalendar::_WidthHint = 0;

WidgetSceneCalendar::WidgetSceneCalendar(IGestionFrameCalendar *aManagerBehavior, const sInfosBloc& aInfosBloc,
                                         QWidget *parent,
                                         bool aGererAffichageDispo) :
    QGraphicsRectItem (QRectF(-25,0,50.,50.)),
    QGraphicsLayoutItem(0),
    _InfosBloc (aInfosBloc),
    _GererAffichageDispo(aGererAffichageDispo),
    _ManagerBehavior(aManagerBehavior)
{

    this->setFlag(QGraphicsItem::ItemIsMovable,    false);
   // this->setFlag(QGraphicsItem::ItemIsSelectable, true);
  //  this->setFlag(QGraphicsItem::ItemIsFocusable);
    if (_InfosBloc._IsTitle) {
        //setPlainText(_InfosBloc._InfoBloc);
        _Height = 20;//this->document()->size().height();
    }
    else
        _Height = 50;

    _WidthHint = 50;//qMax<int> (_WidthHint,
                    //        this->document()->size().width()+1);

}

QSizeF WidgetSceneCalendar::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {

    qreal lwidth  = _WidthHint;
    if (constraint.width()> 0)
        lwidth = qMin<qreal>(lwidth, constraint.width());

    qreal lheight = _Height;
    if (constraint.height()> 0)
        lheight = qMin<qreal>(lheight, constraint.height());

    return QSizeF (_WidthHint, _Height);
}

void WidgetSceneCalendar::setGeometry(const QRectF &rect) {

    //setTextWidth(rect.width());
    setPos (rect.center());
}

//QRectF WidgetSceneCalendar::boundingRect() const {

//    qDebug() << "dim "<<_WidthHint<<" "<<_Height;
//    return QRectF (0, 0, _WidthHint, _Height);
//}

void WidgetSceneCalendar::paint (QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QWidget *widget ) {

    QRectF lBoundingRect = boundingRect();
    QPen oldpen = painter->pen();
    QPen pen;
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawRect(lBoundingRect);

    painter->setPen(oldpen);

   if (!_InfosBloc._IsTitle) {



      if (_InfosBloc._IsEnabled) {

          if (_GererAffichageDispo) {
                if (!_InfosBloc._IsDispo)
                  painter->fillRect(lBoundingRect, Qt::red);
                else
                  painter->fillRect(lBoundingRect, Qt::green);
            } else
              painter->fillRect(lBoundingRect, Qt::white);

       } else
           painter->fillRect(lBoundingRect, Qt::lightGray);

        QRectF lRectDay = lBoundingRect;
        lRectDay.setRight(lRectDay.right()- (2.0/3*lRectDay.right()));
        lRectDay.setBottom(lRectDay.bottom()-(2.0/3*lRectDay.bottom()));
        painter->drawText (lRectDay, _InfosBloc._InfoDay);


        QTextOption lOptions;
        lOptions.setAlignment(Qt::AlignCenter);
        QRectF lRectInfos = lBoundingRect;
        lRectInfos.setTop(lRectDay.bottom()+1);
        painter->drawText (lRectInfos, _InfosBloc._InfoBloc, lOptions);
        QGraphicsRectItem::paint(painter, option, widget);

   } else
        painter->drawText (boundingRect(), _InfosBloc._InfoBloc);
}

void WidgetSceneCalendar::mousePressEvent(QGraphicsSceneMouseEvent *event) {

     if (_InfosBloc._IsTitle) return;
    qDebug()<< "event mousePressEvent "<< _InfosBloc._IdDay;
    if (_ManagerBehavior)
        _ManagerBehavior->DoOnSelectDay (_InfosBloc._IdDay);
}


//void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
//void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
