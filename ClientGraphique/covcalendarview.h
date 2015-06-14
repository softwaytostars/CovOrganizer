#ifndef COVCALENDARVIEW_H
#define COVCALENDARVIEW_H

#include <QGraphicsView>
#include <QMap>

#include "ClientGraphique/covcalendarscene.h"
#include "iabonnegestioncalendar.h"
#include "Types/TypeInfosBloc.hpp"
#include "ClientGraphique/Types/TypeAffichageCalendar.hpp"

class QGraphicsGridLayout;
class WidgetSceneCalendar;
class IGestionFrameCalendar;

class CovCalendarView : public QGraphicsView, public IAbonneGestionCalendar
{
public:
    CovCalendarView(eTypeAffichageCalendar aTypeAffichage, QWidget *parent);
    ~CovCalendarView();

    void Actualiser(int aPers, int aMonth, int aYear, bool aActualiserGestion=true);
    void setPers  (int aIdPers);
    void setMonth (int aMonth);
    void setYear  (int aYear);

    //IAbonneGestionCalendar
    virtual void DoOnChangeIdDay (int aIdDay);
    virtual void DoOnChangePers (int aIdPers);

protected:
    virtual void resizeEvent (QResizeEvent *event) ;
private:
    CovCalendarScene* _scene;
    QDate getPreviousDate (int ayear, int amonth , int aday, int anBreJoursAvant) const;
    QDate getNextDate (int ayear, int amonth , int aday, int anBreJoursApres) const;
    void AjouterBlocTitre (QGraphicsGridLayout* alayout,
                            const QString& aTitle,
                            int aRow,
                            int aCol);
    void AjouterBlocDay (QGraphicsGridLayout* alayout,
                                          const QDate& aDate,
                                          bool aIsNotFake, int aPers,
                                         int aRow,
                                         int aCol);
    void ConstructDayBlocks (int aPers, int aMonth, int aYear);
    QMap<int, WidgetSceneCalendar*> _mapWidgetsDay;/* map entre le IdDay et l'objet bloc day correspondant */
    bool _GererAffichageDispos;
    int  _Month, _Year;
    IGestionFrameCalendar* _GestionCalendar;

};

#endif // COVCALENDARVIEW_H
