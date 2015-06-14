#ifndef GESTIONFRAMEINDISPOS_H
#define GESTIONFRAMEINDISPOS_H

#include "igestionframecalendar.h"
#include "iabonnegestioncalendar.h"

class GestionFrameIndispos : public IGestionFrameCalendar
{
public:
    GestionFrameIndispos();
    virtual void DoOnSelectDay (int aIdDay);
    virtual void CompleterInfosBlocDay (int aIdPers, sInfosBloc &aInfos);
    virtual void Actualiser() {}
};

#endif // GESTIONFRAMEINDISPOS_H
