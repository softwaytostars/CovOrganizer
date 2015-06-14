#include "gestionframesolution.h"

#include "Algo/organizer.h"

#include <gecode/gist/mainwindow.hh>
#include <gecode/driver.hh>
#include <gecode/search.hh>

#include "Data/databasemanager.h"



GestionFrameSolution::GestionFrameSolution() : IGestionFrameCalendar ()
{
}


void GestionFrameSolution::DoOnSelectDay (int aIdDay) {


}

void GestionFrameSolution::CompleterInfosBlocDay (int aIdPers, sInfosBloc &aInfos) {

    aInfos._InfoBloc = "";

    if (aIdPers >=0) {

        bool lEstCond = false;
        if (DataBaseManager::getManager()->getCovoiturage (aIdPers, aInfos._IdDay, lEstCond)) {

            PersonData lPerson;
            if (DataBaseManager::getManager()->getPersonFromId(aIdPers, lPerson )) {
                aInfos._InfoBloc = lPerson._ShortName;
                if (lEstCond)
                    aInfos._InfoBloc.append("(COND)");
            }

        }
    }
    else {
        QVector<int> lIdPersons;
        if (DataBaseManager::getManager()->getIdPersons(lIdPersons)) {

            for (int i=0; i< lIdPersons.count();i++) {

                int lIdPers = lIdPersons[i];
                bool lEstCond = false;
                if (DataBaseManager::getManager()->getCovoiturage (lIdPers, aInfos._IdDay, lEstCond)) {
                    PersonData lPerson;
                    if (DataBaseManager::getManager()->getPersonFromId(lIdPers, lPerson )) {

                        if (aInfos._InfoBloc.size() > 0)
                            aInfos._InfoBloc += "-";
                        aInfos._InfoBloc += lPerson._ShortName;
                        if (lEstCond)
                            aInfos._InfoBloc.append("<b>bold</b>");
                    }
                }
            }
        }//if (DataBaseManager::getManager()->getIdPersons(lIdPersons))
    }
}

void GestionFrameSolution::Actualiser() {


    /* Clear the solutions in DB */
    DataBaseManager::getManager()->ClearSolutions();

    Gecode::Options opt ("Organizer");
    opt.solutions(0);


    Gecode::Search::Options optsearch;
    Gecode::Search::TimeStop * lTimeStop = new Gecode::Search::TimeStop(60000);//maximum 1 minute!
    optsearch.stop = lTimeStop;

    Organizer* lAlgo = new Organizer(opt);
    Gecode::BAB<Organizer> *lSearchEngine = new Gecode::BAB<Organizer> (lAlgo, optsearch);
    Organizer *s = 0;
    int lNbreSolSouhaitees = 3;
    bool lContinue = true;
    //Gecode::Support::Timer timer;
    //timer.start();
    int lNumSol=1;
    do {
        s = lSearchEngine->next();
        if (s) {
            //s->print(std::cout);
            s->FillDBSolution(lNumSol);/* Fill the DB with the solution found */
            lNbreSolSouhaitees--;
            lNumSol++;
            lContinue = (lNbreSolSouhaitees > 0);
            delete s;
        } else lContinue = false;
    } while (lContinue);

    //optsearch.stop = 0;
    delete lSearchEngine;
    delete lAlgo;
    delete lTimeStop;

}
