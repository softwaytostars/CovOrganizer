#ifndef DATA_H
#define DATA_H

#include <QDate>

class Data
{
public:
    Data();
    int getNbreJours () const { return _nbreJours;}
private:
    int _nbreJours;//A prendre en compte pour covoiturage = _DateDebut.daysto (_DateFin)+1;
    QDate _DateDebut;//de prise en comptecovoiturage
    QDate _DateFin;//de prise en comptecovoiturage
};

#endif // DATA_H
