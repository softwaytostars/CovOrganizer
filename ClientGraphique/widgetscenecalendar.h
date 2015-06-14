#ifndef WIDGETSCENECALENDAR_H
#define WIDGETSCENECALENDAR_H

#include <QWidget>
#include <QGraphicsTextItem>
#include <QGraphicsLayoutItem>
#include "Types/TypeInfosBloc.hpp"

class IGestionFrameCalendar;

class WidgetSceneCalendar : public QGraphicsRectItem, public QGraphicsLayoutItem
{
    //Q_OBJECT
public:
    explicit WidgetSceneCalendar(IGestionFrameCalendar* aManagerBehavior,
                                 const sInfosBloc& aInfosBloc,
                                 QWidget *parent = 0,
                                 bool aGererAffichageDispo=false);
    virtual void paint (QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget );
    void ModifierDispo (bool aDispo) {_InfosBloc._IsDispo = aDispo;}
    void ModifierAffichageDispo (bool aAfficher) {_GererAffichageDispo = aAfficher;}
    const sInfosBloc& getInfosBloc () const { return _InfosBloc;}
    void set_InfosBloc (const sInfosBloc& aInfosBloc) {_InfosBloc = aInfosBloc;}

protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setGeometry(const QRectF &rect);
//    virtual QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    qreal  _Height;
    bool  _IsTitle;
    static qreal _WidthHint;//car largeur dependra du plus grand
    sInfosBloc _InfosBloc;
    bool _GererAffichageDispo;
    IGestionFrameCalendar* _ManagerBehavior;



signals:
    
public slots:
    
};

#endif // WIDGETSCENECALENDAR_H
