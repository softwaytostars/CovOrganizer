#include "igestionframecalendar.h"
#include "iabonnegestioncalendar.h"

IGestionFrameCalendar::IGestionFrameCalendar():
    _IdPers(-1),
    _IAffichage(0)
{
}

void IGestionFrameCalendar::setPers (int aIdPers) {

    if (_IdPers == aIdPers) return;

    _IdPers = aIdPers;

     /* Notifie l'affichage du changement */
    if (_IAffichage)
        _IAffichage->DoOnChangePers(aIdPers);

}
