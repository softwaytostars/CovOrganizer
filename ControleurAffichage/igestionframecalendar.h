#ifndef IGESTIONFRAMECALENDAR_H
#define IGESTIONFRAMECALENDAR_H

#include <QString>
#include "ClientGraphique/Types/TypeInfosBloc.hpp"

class IAbonneGestionCalendar;

class IGestionFrameCalendar
{
public:
    IGestionFrameCalendar();
    void setPers (int aIdPers);
    int getIdPers () const {return _IdPers;}
    void Abonner (IAbonneGestionCalendar* aObservateur) {_IAffichage = aObservateur;}
public:
    virtual void DoOnSelectDay (int aIdDay)=0;
    virtual void CompleterInfosBlocDay (int aIdPers, sInfosBloc &aInfos)=0;
    virtual void Actualiser()=0;
protected :
        IAbonneGestionCalendar *  getAbonneGestionCalendar() const { return _IAffichage;}
private:
    int _IdPers;//si < 0, on affichera pour tous
    IAbonneGestionCalendar* _IAffichage;
};

#endif // IGESTIONFRAMECALENDAR_H
