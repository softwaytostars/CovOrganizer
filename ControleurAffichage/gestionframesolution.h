#ifndef GESTIONFRAMESOLUTION_H
#define GESTIONFRAMESOLUTION_H

#include "igestionframecalendar.h"
#include "iabonnegestioncalendar.h"


class GestionFrameSolution : public IGestionFrameCalendar
{
public:
    GestionFrameSolution();
    virtual void DoOnSelectDay (int aIdDay);
    virtual void CompleterInfosBlocDay (int aIdPers, sInfosBloc &aInfos);
    virtual void Actualiser();
};

#endif // GESTIONFRAMESOLUTION_H
