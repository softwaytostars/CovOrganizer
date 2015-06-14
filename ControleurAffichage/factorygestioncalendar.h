#ifndef FACTORYGESTIONCALENDAR_H
#define FACTORYGESTIONCALENDAR_H

#include "igestionframecalendar.h"
#include "ClientGraphique/Types/TypeAffichageCalendar.hpp"

class FactoryGestionCalendar
{
public:
    static IGestionFrameCalendar* FournirGestionnaire (eTypeAffichageCalendar aType);
private:
    FactoryGestionCalendar();
};

#endif // FACTORYGESTIONCALENDAR_H
