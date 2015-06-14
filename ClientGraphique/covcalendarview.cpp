#include "covcalendarview.h"

#include <QTextEdit>
#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>
#include <QSqlQuery>
#include <qmath.h>
#include <QDate>
#include <QDebug>

#include "ClientGraphique/widgetscenecalendar.h"
#include "utilitaires.h"
#include "Data/databasemanager.h"
#include "ControleurAffichage/factorygestioncalendar.h"

CovCalendarView::CovCalendarView(eTypeAffichageCalendar aTypeAffichage,
                                 QWidget *parent) : QGraphicsView (parent),
    _Month (-1),
    _Year (-1)
{

    setRenderHint(QPainter::HighQualityAntialiasing, true);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);

    _GererAffichageDispos = (aTypeAffichage==eAffichageIndispo);

    //cree la scene
    _scene = new CovCalendarScene (this);
    setScene(_scene);

    //cree le gestionnaire
    _GestionCalendar = FactoryGestionCalendar::FournirGestionnaire(aTypeAffichage);
    _GestionCalendar->Abonner (this);


}

CovCalendarView::~CovCalendarView() {

    _scene->clear();
    delete _GestionCalendar;
}

void CovCalendarView::resizeEvent (QResizeEvent *event) {


    QGraphicsView::resizeEvent(event);
//    //pour ajuster la taille de la scene Ã  la vue
    ensureVisible (sceneRect());
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

QDate CovCalendarView::getPreviousDate (int ayear, int amonth , int aday, int anBreJoursAvant) const {

    return QDate (ayear, amonth, aday).addDays(-anBreJoursAvant);
}

QDate CovCalendarView::getNextDate (int ayear, int amonth , int aday, int anBreJoursApres) const {

    return QDate(ayear, amonth, aday).addDays(anBreJoursApres);
}

void CovCalendarView::AjouterBlocDay (QGraphicsGridLayout* alayout,
                                      const QDate& aDate,
                                      bool aIsNotFake,
                                      int aPers,
                                     int aRow,
                                     int aCol) {

    sInfosBloc lInfos (false, aIsNotFake, QString("%1").arg (aDate.day()), "");
    int lIdDay = Utilitaires::CalculateIdDayFromDate (aDate);
    lInfos._IdDay = lIdDay;

    _GestionCalendar->CompleterInfosBlocDay (aPers, lInfos);

    WidgetSceneCalendar *day = new WidgetSceneCalendar(_GestionCalendar, lInfos, this, _GererAffichageDispos);
    _mapWidgetsDay [lIdDay] = day;

    _scene->addItem(day);
    alayout->addItem(day,  aRow, aCol);
}



void CovCalendarView::AjouterBlocTitre (QGraphicsGridLayout* alayout,
                                       const QString& aTitle,
                                       int aRow,
                                       int aCol) {

    sInfosBloc lInfos (true, true, "", aTitle);

    WidgetSceneCalendar *day = new WidgetSceneCalendar(_GestionCalendar, lInfos, this);

    _scene->addItem(day);
    alayout->addItem(day,  aRow, aCol);
}


void CovCalendarView::Actualiser(int aPers, int aMonth, int aYear, bool aActualiserGestion) {

    if (!_GestionCalendar) return;

    //Si date change, reconstruit les blocs calendrier
    if ( (_Month != aMonth) || (_Year != aYear)) {
        _Month = aMonth;
        _Year  = aYear;
        ConstructDayBlocks (aPers, _Month, _Year);
    }

    if (aActualiserGestion)
        _GestionCalendar->Actualiser();

    if (_GestionCalendar->getIdPers() != aPers)
         _GestionCalendar->setPers(aPers);

}

void CovCalendarView::setPers (int aIdPers) {

    Actualiser (aIdPers, _Month, _Year, false);
}

void CovCalendarView::setMonth (int aMonth) {

    if (!_GestionCalendar) return;
    Actualiser (_GestionCalendar->getIdPers(), aMonth, _Year, false);
}

void CovCalendarView::setYear (int aYear) {

    if (!_GestionCalendar) return;
    Actualiser (_GestionCalendar->getIdPers(), _Month, aYear, false);
}


void CovCalendarView::ConstructDayBlocks (int aPers, int aMonth, int aYear) {

    _scene->clear();
    _mapWidgetsDay.clear();

    QVector<QString> lNomsJours;
    lNomsJours.push_back(tr("Lundi"));
    lNomsJours.push_back(tr("Mardi"));
    lNomsJours.push_back(tr("Mercredi"));
    lNomsJours.push_back(tr("Jeudi"));
    lNomsJours.push_back(tr("Vendredi"));

    //cree le layout gerant la disposition des blocs
    QGraphicsGridLayout* layout = new QGraphicsGridLayout ();


    //ajout les blocs titres dans la scene et dans le layout
    for (int i= 1; i <= lNomsJours.count(); i++) {

        AjouterBlocTitre (layout, lNomsJours [i-1], 0, i-1);
        //elimine espace horizontaux
        layout->setColumnSpacing(i-1, 0);
    }


    for (int J=1; J <= 31; J++) {

        if (!QDate::isValid(aYear, aMonth, J)) continue;

        QDate date (aYear, aMonth, J);
        int lDayOfWeek = date.dayOfWeek() ;

        if (lDayOfWeek > lNomsJours.count()) continue;


        if ((_mapWidgetsDay.count() == 0) && (lDayOfWeek != 1)) {

            //Ajout des blocs disabled au debut correspondant à jours precedent

            for (int i= 1; i <  lDayOfWeek; i++) {

                int lLigne  = 1+ (_mapWidgetsDay.count()/lNomsJours.count());
                int lCol    = i -1;
                QDate lPreviousdate = getPreviousDate (aYear, aMonth, J, lDayOfWeek -i);
                AjouterBlocDay (layout, lPreviousdate, false, aPers, lLigne,lCol);

            }
        }


        int lLigne  = 1+ (_mapWidgetsDay.count()/lNomsJours.count());
        int lCol    = lDayOfWeek -1;

        AjouterBlocDay (layout, date, true, aPers, lLigne,lCol);

    }


    //si dernier jour et pas grille complete
    int lLastDayOfWeekTaken = 1 + (_mapWidgetsDay.count()-1)%lNomsJours.count();
    if (lLastDayOfWeekTaken < lNomsJours.count()) {

        //Ajout des blocs disabled au debut correspondant à jours precedent
        for (int i= lLastDayOfWeekTaken+1; i <=  lNomsJours.count(); i++) {

            int lLigne  = 1+ (_mapWidgetsDay.count()/lNomsJours.count());
            int lCol    = i -1;

            QDate lNextdate = getNextDate (aYear, aMonth, 31, i - lLastDayOfWeekTaken);
            AjouterBlocDay (layout, lNextdate, false, aPers, lLigne,lCol);

        }

    }

    for (int i=0; i < layout->rowCount(); i++){
        layout->setRowSpacing(i, 0);
    }

    QGraphicsWidget *form = new QGraphicsWidget();
    form->setLayout(layout);
    _scene->addItem(form);


}

void CovCalendarView::DoOnChangeIdDay (int aIdDay) {

    if (!_GestionCalendar) return;

    if (_mapWidgetsDay.contains(aIdDay)) {

        WidgetSceneCalendar* lBloc = _mapWidgetsDay [aIdDay];
        if (lBloc) {
            lBloc->ModifierDispo(!DataBaseManager::getManager()->LaPersonneEstIndispo (_GestionCalendar->getIdPers(), aIdDay));

            lBloc->update();
        }

        _scene->update();
    }

}

void CovCalendarView::DoOnChangePers (int aIdPers) {

    if (_mapWidgetsDay.count() > 0) {

        QMapIterator<int, WidgetSceneCalendar*> it (_mapWidgetsDay);
        while (it.hasNext()) {

           it.next();
           //gestion affichage dispos
           WidgetSceneCalendar *lBlocDay = it.value();
           if (_GererAffichageDispos && (aIdPers >=0)) {

               lBlocDay->ModifierAffichageDispo(true);
               lBlocDay->ModifierDispo(!DataBaseManager::getManager()->LaPersonneEstIndispo (aIdPers, it.key()));

           }
           else
               lBlocDay->ModifierAffichageDispo(false);

           //Gestion affichage texte
           sInfosBloc lInfos = lBlocDay->getInfosBloc();
           _GestionCalendar->CompleterInfosBlocDay (aIdPers, lInfos);
           lBlocDay->set_InfosBloc(lInfos);
           lBlocDay->update();
        }
    } else
        ConstructDayBlocks (aIdPers, _Month, _Year);


}
