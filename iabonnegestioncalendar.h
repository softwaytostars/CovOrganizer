#ifndef IABONNEGESTIONCALENDAR_H
#define IABONNEGESTIONCALENDAR_H

class IAbonneGestionCalendar
{
public:
    IAbonneGestionCalendar();
    virtual void DoOnChangeIdDay (int aIdDay)=0;
    virtual void DoOnChangePers (int aIdPers)=0;
};

#endif // IABONNEGESTIONCALENDAR_H
