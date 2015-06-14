#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Algo/organizer.h"

#include <gecode/gist/mainwindow.hh>
#include <gecode/driver.hh>
#include <gecode/search.hh>
#include "ClientGraphique/framelisteetcalendar.h"
#include "ClientGraphique/frameentreedata.h"

//#include "ClientGraphique/covcalendarview.h"
#include "Data/databasemanager.h"
//#include "ControleurAffichage/gestionframeindispos.h"
#include "ClientGraphique/Types/TypeAffichageCalendar.hpp"

using namespace Gecode;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mainToolBar->setVisible(false);
    ui->tabWidget->setFont(QFont ("Arial", 10));
    ui->tabWidget->clear();
    ui->tabWidget->setElideMode(Qt::ElideNone);

    _FrameEntreeData = new FrameEntreeData (this);
    ui->tabWidget->addTab(_FrameEntreeData, tr("Données"));

    _FrameChoixData = new FrameListeEtCalendar (eAffichageIndispo, this);
    ui->tabWidget->addTab(_FrameChoixData, tr("Indispos"));

    _FrameSolution = new FrameListeEtCalendar (eAffichageSolution, this);
    ui->tabWidget->addTab(_FrameSolution, tr("Solution"));


   // opt.iterations(10);


//    MinimizeScript::run<Organizer,BAB,Gecode::Options>(opt);


//    Gecode::Gist::Options optGist;
//    Gecode::Gist::Print<Organizer> *p1 = new Gecode::Gist::Print<Organizer>("My Gist print");
//    optGist.inspect.click (p1);
//    Organizer *lSolver = new Organizer (opt);
//    bool lUseBAB = true;
//    new Gecode::Gist::GistMainWindow ((Gecode::Space*)lSolver, lUseBAB, optGist);



    //this->setCentralWidget(new CovCalendarView(this));

    connect (ui->tabWidget, SIGNAL (currentChanged(int)), this, SLOT(OnOngletChanged (int)));
    //connecte les sous onglets
    for (int i=0; i < ui->tabWidget->count(); i++) {
        if (dynamic_cast<QTabWidget*> (ui->tabWidget->widget(i)))
            connect (ui->tabWidget->widget(i), SIGNAL (currentChanged (QWidget*)), this, SLOT (OnOngletChanged(QWidget*)));
    }


    setCentralWidget(ui->tabWidget);

}

void MainWindow::OnOngletChanged (int) {

    ActualiserAffichageCourant ();
}

void MainWindow::ActualiserAffichageCourant () {

    IActualisable* lFrame = dynamic_cast<IActualisable*> (getCurrentWidget (ui->tabWidget));
    if (lFrame)
        lFrame->Actualiser();
}

QWidget * MainWindow::getCurrentWidget(QTabWidget* atab) {

    QWidget *lWidget = atab->currentWidget();
    QTabWidget* lsubTab =0;
    if (lWidget && (lsubTab=dynamic_cast <QTabWidget*> (lWidget)))
        return (getCurrentWidget (lsubTab));
    return lWidget;
}

MainWindow::~MainWindow()
{
    DataBaseManager::killManager();
    delete ui;
}
