#include "dataalgo.h"
#include <QDate>
#include "Data/databasemanager.h"
#include "utilitaires.h"
#include <QVector>

using namespace Gecode;

DataAlgo::DataAlgoO::DataAlgoO():
    _NbreJours (0),
    _NbrePers  (0)
{

    QDate lDateDebut, lDateFin;
    if (DataBaseManager::getManager()->getDateDebutEtFin (lDateDebut, lDateFin)) {

        _NbreJours = lDateDebut.daysTo(lDateFin);

        if (_NbreJours >=0) {

            //map
            QVector<int> lIdPersons;

            if (!DataBaseManager::getManager()->getIdPersons(lIdPersons))
                return;

            for (int i=0; i < lIdPersons.count(); i++) {

                int lIdPers = lIdPersons [i];
                _MapPersAlgoIdPers [i] = lIdPers;//les utilisateurs seront representes par un chiffre de 0 à N-1

            }

            QDate lDate = lDateDebut;
            int NumJourAlgo = 0;

            while (lDate < lDateFin) {

                int lIdDay = Utilitaires::CalculateIdDayFromDate (lDate);
                _MapJourAlgoIdDay [NumJourAlgo] = lIdDay;

                std::vector<int> lDomPersDispos;
                for (int i=0; i < lIdPersons.count(); i++) {

                    int lIdPers = lIdPersons [i];

                    if (!DataBaseManager::getManager()->LaPersonneEstIndispo (lIdPers, lIdDay))
                        lDomPersDispos.push_back(i);

                }//for

                _MapJourAlgoPersPossibles [NumJourAlgo] = lDomPersDispos;

                NumJourAlgo++;
                lDate = lDate.addDays(1);
            }

            _NbrePers = _MapPersAlgoIdPers.count();
        }

    }


}

DataAlgo::DataAlgoO::DataAlgoO(const DataAlgoO& aDataAlgoO) :
    _NbreJours (aDataAlgoO._NbreJours),
    _NbrePers (aDataAlgoO._NbrePers),
    _MapPersAlgoIdPers (aDataAlgoO._MapPersAlgoIdPers),
    _MapJourAlgoIdDay (aDataAlgoO._MapJourAlgoIdDay),
    _MapJourAlgoPersPossibles(aDataAlgoO._MapJourAlgoPersPossibles)
{

}



DataAlgo::DataAlgo() : SharedHandle (new DataAlgoO()) {

}
