#include "framelisteetcalendar.h"
#include "ui_framelisteetcalendar.h"

#include <QSplitter>
#include <QListWidgetItem>
#include <QSqlQuery>
#include <QDate>

#include "ClientGraphique/covcalendarview.h"
#include "ClientGraphique/framecalendar.h"
#include "Data/databasemanager.h"


FrameListeEtCalendar::FrameListeEtCalendar(eTypeAffichageCalendar aTypeAffichage, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameListeEtCalendar)
{
    ui->setupUi(this);


    this->setLayout(ui->MainHLayout);

    _splitter         = new QSplitter (Qt::Horizontal, this);

    //cree la liste et l'ajoute dans le splitter
    _listeDatas       = new ListeViewPers (this);
    _listeDatas->setSelectionMode(QAbstractItemView::SingleSelection);
   // _listeDatas->selectionModel()->setCurrentIndex(_listeDatas->model()->index(0,0), QItemSelectionModel::SelectCurrent);
    _listeDatas->setCurrentIndex(_listeDatas->model()->index(0,0));//selectionne le premier (todo a voir en fonction de l'enumere affichage)

    _splitter->addWidget(_listeDatas);

    _FrameCalendar = new FrameCalendar (aTypeAffichage, this);
    _splitter->addWidget(_FrameCalendar);

    ui->MainHLayout->addWidget(_splitter);

//    ChoixSelectionCI::getInstance(_ControleurAffichage->getMainControleur())->Abonner(this);

//    _listeDatas->setToolTip(tr("Effectuez une sÃ©lection"));

    connect(_listeDatas, SIGNAL(clicked(const QModelIndex&)), this, SLOT(OnClickListeData(const QModelIndex&)));


}

FrameListeEtCalendar::~FrameListeEtCalendar()
{
    delete ui;
}

void FrameListeEtCalendar::OnClickListeData(const QModelIndex& aIndex) {

    if (_FrameCalendar)
        _FrameCalendar->setPers (DataBaseManager::getManager()->getIdPersFromName (aIndex.data(Qt::DisplayRole).toString()));

}

void FrameListeEtCalendar::Actualiser() {

    //trouve les dates choisies et cree la frame du calendrier
    QDate lDateDebut, lDateFin;
    if (DataBaseManager::getManager()->getDateDebutEtFin (lDateDebut, lDateFin)) {

        int lIdPers = DataBaseManager::getManager()->getIdPersFromName (_listeDatas->currentIndex().data().toString());
        _FrameCalendar->Actualiser(lIdPers, lDateDebut, lDateFin);

    }

}

