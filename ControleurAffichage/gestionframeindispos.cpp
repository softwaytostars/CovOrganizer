#include "gestionframeindispos.h"
#include "Data/databasemanager.h"

GestionFrameIndispos::GestionFrameIndispos()  : IGestionFrameCalendar ()
{
}


void GestionFrameIndispos::DoOnSelectDay (int aIdDay) {

    /* Modifie la base pour changer dispo */
    if (DataBaseManager::getManager()->LaPersonneEstIndispo(getIdPers(), aIdDay))
        DataBaseManager::getManager()->DeleteIndispo (getIdPers(), aIdDay);
    else
        DataBaseManager::getManager()->insertIndispo(getIdPers(), aIdDay);

    /* Notifie l'affichage du changement */
    if (getAbonneGestionCalendar())
        getAbonneGestionCalendar()->DoOnChangeIdDay(aIdDay);

}

void GestionFrameIndispos::CompleterInfosBlocDay (int aIdPers, sInfosBloc &aInfos) {

    aInfos._IsDispo  = false;
    aInfos._InfoBloc = "";

    if (aIdPers >=0 )
      aInfos._IsDispo = !DataBaseManager::getManager()->LaPersonneEstIndispo (aIdPers, aInfos._IdDay);


}
