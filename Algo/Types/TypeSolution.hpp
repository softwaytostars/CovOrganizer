#ifndef TYPESOLUTION_HPP
#define TYPESOLUTION_HPP

#include <QVector>
#include "Algo/Types/TypePropositionJour.hpp"

struct sTypeSolution {

    QVector<sTypePropositionJour> _PropositionsJours;
    sTypeSolution () {}
    sTypeSolution (int aNbreJours) {
        _PropositionsJours.reserve(aNbreJours);
    }

    void AjouterPropositionJour (const sTypePropositionJour& aPropositionJour) {
        _PropositionsJours.push_back(aPropositionJour);
    }

};

#endif // TYPESOLUTION_HPP
