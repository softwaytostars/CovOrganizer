#ifndef ORGANIZER_H
#define ORGANIZER_H

#include <gecode/int.hh>
#include <gecode/set.hh>
#include <gecode/minimodel.hh>
#include <gecode/driver.hh>
#include "Algo/Types/TypeSolution.hpp"
#include "Algo/Types/TypePropositionJour.hpp"
#include "Algo/SharedObjects/dataalgo.h"

class GECODE_DRIVER_EXPORT Organizer : public Gecode::MinimizeScript {
public:
    Organizer(const Gecode::Options &);
    virtual Gecode::IntVar cost(void) const;
    /// Constructor for cloning \a s
    Organizer(bool share, Organizer& s);
    /// Copy during cloning
    virtual Space* copy(bool share) {
      return new Organizer(share, *this);
    }
    /// Print solution
    virtual void print(std::ostream& os) const;
    void FillDBSolution (int aNumSol) const;

private:
    DataAlgo _DataAlgo;
    Gecode::IntVarArray  _PersCond;/* tableau des variables des conducteurs sur les jours à considerer ayant pour domaine les personnes à considerer */
    Gecode::SetVarArray  _PersCov; /* tableau des variables des covoitureurs sur les jours à considerer ayant pour domaine les personnes à considerer */
    Gecode::BoolVarArray _PenaliteMemeCond; /* tableau de variables par jour pour une penalite si on a un meme conducteur d'un jour sur l'autre */
    Gecode::IntVarArray  _NbreFoisConducteur; /* tableau de variables par personne pour compter le nbre de fois où chacune d'entre elle conduit */
    Gecode::IntVarArray  _PenaliteEquiteCond; /* variables pour une penalite si des personnes conduisent plus que d'autres */
    Gecode::BoolVarArray _PenalitePasDeCond; /* tableau de variables par jour  pour une penalite si pas de conducteur */
    Gecode::IntVar       _Cost;

};

#endif // ORGANIZER_H
