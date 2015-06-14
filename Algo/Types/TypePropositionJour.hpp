#ifndef TYPEPROPOSITION_HPP
#define TYPEPROPOSITION_HPP

#include <QList>

struct sTypePropositionJour {

    int _IdCOND;//le ID de la personne qui conduit
    QList<int> _LesCovoitureurs;//les ID des covoitureurs
    sTypePropositionJour () :
        _IdCOND   (-1) {}
    sTypePropositionJour (const sTypePropositionJour& aProposition) :
        _IdCOND (aProposition._IdCOND),
        _LesCovoitureurs (aProposition._LesCovoitureurs)
    {}

    void SetConducteur (int aIdCOND) {_IdCOND = aIdCOND;}
    void addCovoitureur (int aIdPers) {_LesCovoitureurs.push_back(aIdPers);}

};

#endif // TYPEPROPOSITION_HPP
