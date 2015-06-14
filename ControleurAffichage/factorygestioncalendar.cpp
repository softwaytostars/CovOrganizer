#include "factorygestioncalendar.h"
#include "ControleurAffichage/gestionframeindispos.h"
#include "ControleurAffichage/gestionframesolution.h"

FactoryGestionCalendar::FactoryGestionCalendar()
{
}

IGestionFrameCalendar* FactoryGestionCalendar::FournirGestionnaire (eTypeAffichageCalendar aType) {

    switch (aType) {
        case eAffichageIndispo  : return new GestionFrameIndispos ();
        case eAffichageSolution : return new GestionFrameSolution ();
    }
    return 0;
}
