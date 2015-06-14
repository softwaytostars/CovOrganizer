#include "organizer.h"
#include <QVector>
#include <QDate>

#include <iostream>
#include <QDebug>
#include "Data/databasemanager.h"

using namespace Gecode;

struct sChoixImpose {

    int _NumPers;
    int _Jour;
    sChoixImpose () :
        _NumPers (0),
        _Jour (0) {}
    sChoixImpose (int aJour, int aNumPers) :
        _NumPers (aNumPers),
        _Jour (aJour) {}
};

Organizer::Organizer(const Gecode::Options&)
{

    bool lOptionCovoiturageMaxi = false;


    if (_DataAlgo.getNbreJours() <=0) {

        this->fail();
        return;
    }

    if (_DataAlgo.getNbrePers() <=1) {

        this->fail();
        return;
    }


    _PersCond = IntVarArray (*this, _DataAlgo.getNbreJours());
    _PersCov  = SetVarArray (*this, _DataAlgo.getNbreJours());

    _NbreFoisConducteur = IntVarArray  (*this,  _DataAlgo.getNbrePers(), 0, _DataAlgo.getNbreJours());//une personne pourra etre conducteur au maxi _DataAlgo.getNbreJours()
    _PenaliteMemeCond   = BoolVarArray (*this, _DataAlgo.getNbreJours(), 0, 1);
    _PenaliteEquiteCond = IntVarArray (*this, _DataAlgo.getNbrePers(),   0, _DataAlgo.getNbreJours());//la penalite correspond à la difference entre cette pers et la pers suivante
    _PenalitePasDeCond  = BoolVarArray (*this, _DataAlgo.getNbreJours(), 0, 1);



    /* Initialiser avec datas _PersCond et _PersCov des personnes dispos*/
    QVector<sChoixImpose> lChoixImposes;

    for (int J = 0; J < _DataAlgo.getNbrePers(); J++) {

//        IntArgs lDomPersSansBidons;// = IntArgs(2, 0, 1);


//        if (J==0) {
//            lDomPersSansBidons << 0;
//            lDomPersSansBidons << 1;
//        }

//        if (J==1) {
//            lDomPersSansBidons << 0;
//            lDomPersSansBidons << 2;
//        }
//        if (J==2) {
//        }
//        if (J==3) {
//            lDomPersSansBidons << 0;
//            lDomPersSansBidons << 1;
//            lDomPersSansBidons << 2;
//        }
//        if (J==4) {
//            lDomPersSansBidons << 1;
//        }

        if (!_DataAlgo.getMapJourAlgoPersPossibles().contains(J)) {
            this->fail();
            return;
        }

        IntArgs lDomPers (_DataAlgo.getMapJourAlgoPersPossibles()[J]);
        lDomPers << -(J+1);//Ajoute une personne bidon qui sera differente pour chaque jour

        if ((lChoixImposes.count() == 0) && (_DataAlgo.getMapJourAlgoPersPossibles()[J].size() > 0) ) {
            lChoixImposes.push_back(sChoixImpose (J, _DataAlgo.getMapJourAlgoPersPossibles()[J] [0]));
        }

        IntSet lSetPers  = IntSet (lDomPers);

        IntSet lSetPersSansBidons  = IntSet (IntArgs(_DataAlgo.getMapJourAlgoPersPossibles()[J]));
        _PersCond [J] = IntVar (*this, lSetPers);
        _PersCov  [J] = SetVar (*this, IntSet::empty, lSetPersSansBidons, 0, _DataAlgo.getNbrePers());//faire sans bidon
    }


    //imposer une personne sur un jour donne (reduit le nbre de solutions)
    for (int i=0; i < lChoixImposes.count(); i++) {
        rel (*this, _PersCond [lChoixImposes[i]._Jour], IRT_EQ, lChoixImposes[i]._NumPers);
    }


    /********** Ajout des contraintes ***********/

    //contraint avec les disponibilites

    //Pour chaque journee, le conducteur choisi doit faire parti des covoitureurs (si conducteur dispo)

    for (int J= 0; J < _DataAlgo.getNbreJours(); J++) {
        BoolVar UnConducteurDispo = expr (*this, _PersCond [J] >= 0);
        rel (*this, _PersCov[J], SRT_SUP, _PersCond[J], imp(UnConducteurDispo));
    }

    //Contraintes pour compter le nbre de fois où chaque personne conduit
    //iPers represente le numero de la personne, _NbreFoisConducteur [iPers] est le nbre de fois où conduit iPers sur tous les jours
    for (int iPers = 0; iPers < _DataAlgo.getNbrePers(); iPers++) {
        count (*this, _PersCond, iPers, IRT_EQ, _NbreFoisConducteur [iPers]);
    }


    //Ajout contrainte (_NbreFoisConducteur [iPers] != _NbreFoisConducteur [iPers+1]) => (_PenaliteEquiteCond [iPers] = 1)
    for (int iPers = 0; iPers < _DataAlgo.getNbrePers()-1; iPers++) {

           if (_DataAlgo.getNbreJours() > 1) {

                if (lOptionCovoiturageMaxi) {

                   // for (int iAutrePers =1; iAutrePers < _DataAlgo.getNbrePers(); iAutrePers++) {

                    //    if (iAutrePers <= iPers) continue;

//                        BoolVar b = expr (*this, _NbreFoisConducteur [iPers] != _NbreFoisConducteur [iPers+1]);
                        BoolVar MetPenalite = expr (*this, _PenaliteEquiteCond [iPers] == (abs (_NbreFoisConducteur [iPers+1]-_NbreFoisConducteur [iPers])));
                        rel (*this, MetPenalite, IRT_EQ, 1);

                  //  }



                } else {

                    rel (*this, _NbreFoisConducteur [iPers], IRT_EQ, _NbreFoisConducteur [iPers+1]);
                    rel (*this, _PenaliteEquiteCond [iPers], IRT_EQ, 0);
                }

            } else
                rel (*this, _PenaliteEquiteCond [iPers], IRT_EQ, 0);
    }
    //on met rien sur le dernier
    rel (*this, _PenaliteEquiteCond [_DataAlgo.getNbrePers()-1], IRT_EQ, 0);



    //Ajout contrainte  (_PersCond [J] == _PersCond [J+1])  => (_PenaliteMemeCond [J] = 1)
    if (_DataAlgo.getNbreJours() > 1) {
        for (int J= 0; J < _DataAlgo.getNbreJours()-1; J++) {

            if (lOptionCovoiturageMaxi) {

                BoolVar b = expr (*this, _PersCond [J] == _PersCond [J+1]);
                rel (*this, _PenaliteMemeCond [J], IRT_EQ, 1, b);

            } else {

                rel (*this, _PersCond [J], IRT_NQ, _PersCond [J+1]);
                rel (*this, _PenaliteMemeCond, IRT_EQ, 0);
            }

        }
        //on met rien sur le dernier jour
        rel (*this, _PenaliteMemeCond [_DataAlgo.getNbreJours()-1], IRT_EQ, 0);
    } else
        rel (*this, _PenaliteMemeCond [0], IRT_EQ, 0);



    //Ajout contrainte |_PersCov[J]| < 2 => _PersCond [J] = -(J+1) : PasAsserDeMonde => PasDeConducteur
    // _PersCond [J] = -(J+1) <=> |_PersCov[J]| = {0} : PasDeCovoiturage <=> PasDeConducteur
    // PasDeConducteur <=> PenalitePasDeConducteur

    for (int J= 0; J < _DataAlgo.getNbreJours(); J++) {

        BoolVar PasDeCovoiturage        = expr (*this, cardinality (_PersCov[J]) == 0);
        BoolVar PasDeConducteur         = expr (*this, _PersCond [J] == -(J+1));
        BoolVar PasAsserDeMonde         = expr (*this, cardinality (_PersCov[J]) < 2);
        BoolVar PenalitePasDeConducteur = expr (*this, _PenalitePasDeCond [J] == 1);
        rel (*this, PasAsserDeMonde,  IRT_EQ, 1, pmi (PasDeConducteur));
        rel (*this, PasDeCovoiturage, IRT_EQ, PasDeConducteur);
        rel (*this, PasDeConducteur,  IRT_EQ, PenalitePasDeConducteur);

    }


    //calcul du coût de la solution

    const int CPoidsAlternerConducteur = 3;
    const int CPoidsEquiteCond         = 6;//par personne
    const int CPoidsJourSansCov        = 9;

    _Cost = expr(*this, CPoidsEquiteCond*sum (_PenaliteEquiteCond)
                        + CPoidsAlternerConducteur*sum (_PenaliteMemeCond)
                        + CPoidsJourSansCov*sum (_PenalitePasDeCond));


    branch(*this, _PersCond, INT_VAR_SIZE_MIN(), INT_VAL_MAX());//faire un brancher pour aller + vite (chercher les personnes pas prises les N-1 jours precedents)

}

Organizer::Organizer(bool share, Organizer& s) : MinimizeScript(share,s)
{

    _DataAlgo.update(*this, share, s._DataAlgo);

    _PersCond.update(*this, share, s._PersCond);
    _PersCov.update(*this, share, s._PersCov);
    _PenaliteMemeCond.update(*this, share, s._PenaliteMemeCond);
    _NbreFoisConducteur.update(*this, share, s._NbreFoisConducteur);
    _PenaliteEquiteCond.update(*this, share, s._PenaliteEquiteCond);
    _PenalitePasDeCond.update(*this, share, s._PenalitePasDeCond);
    _Cost.update(*this, share, s._Cost);

}

Gecode::IntVar Organizer::cost(void) const {


    return _Cost;
}

void Organizer::print(std::ostream& os) const {

    qDebug() <<"print";

    os << "------------------------------"  << std::endl;

   // if (!_PersCond [0].assigned() || (_PersCond [0].val() != 0)) return;

    for (int J= 0; J < _DataAlgo.getNbreJours(); J++) {

        os << "\t Pour Jour J=" << J            << std::endl;
        qDebug() <<"Pour Jour J=" << J;
        os << "\t _PersCond "  << _PersCond [J] << std::endl;
        if (_PersCond [J].assigned())
            qDebug() << "cond trouve ="<<_PersCond [J].val();
        os << "\t les covoitureurs "  << _PersCov [J] << std::endl;
        for (SetVarGlbValues i(_PersCov [J]); i(); ++i) {
            os << i.val() << " ";
            qDebug() << "covoitureur ="<<i.val();
        }
        os << std::endl << "t1"<<std::endl;
        for (SetVarLubRanges i(_PersCov [J]); i(); ++i) {
            os << i.min() << " " << i.max()<<" ";
        }
        os << std::endl << "t2 " << std::endl;
        for (SetVarLubRanges i(_PersCov [J]); i(); ++i) {
            os << i.min() << " " << i.max()<<" ";
        }
        SetVar l = _PersCov [J];
        os << std::endl << l.glbSize()<<" "<<l.lubSize()<<" " << l.cardMin()<<" "<<l.cardMax()<<" "<<l.glbMin()<<" "<<l.glbMax()<< " "<<l.lubMin()<<" "<<l.lubMax()<<std::endl;

        os << "\t _PenaliteMemeCond "  << _PenaliteMemeCond [J] << std::endl;
        os << "\t " << std::endl;
    }

    for (int iPers = 0; iPers < _DataAlgo.getNbrePers(); iPers++) {

         os << "\t Pour Pers iPers=" << iPers    << std::endl;
         os << "\t _NbreFoisConducteur" << _NbreFoisConducteur [iPers]    << std::endl;
         os << "\t _PenaliteEquiteCond" << _PenaliteEquiteCond [iPers]    << std::endl;
         os << "\t " << std::endl;
    }

    os << "penalite meme cond" << _PenaliteMemeCond <<std::endl;
    os << "PenaliteEquiteCond" << _PenaliteEquiteCond <<std::endl;
    os << "PenalitePasDeConducteur" << _PenalitePasDeCond<<std::endl;
    os << "Cout Solution "<<_Cost <<std::endl;

}

void Organizer::FillDBSolution (int aNumSol) const {

    for (int J= 0; J < _DataAlgo.getNbreJours(); J++) {

        int lIdDay = _DataAlgo.getMapJourAlgoIdDay()[J];
        //si il y a un conducteur sur cette journee
        if (_PersCond [J].assigned() && (_PersCond [J].val() >=0) ) {

            for (SetVarGlbValues i(_PersCov [J]); i(); ++i) {

                int lIdPers = _DataAlgo.getMapPersAlgoIdPers()[i.val()];
                bool lEstCond = (_PersCond [J].val() == i.val() );
                DataBaseManager::getManager()->insertCovoiturage(lIdPers, lIdDay, lEstCond, aNumSol);
            }
        }//:if (_PersCond [J].assigned() && (_PersCond [J].val() >=0) ) {
    }//for (int J= 0; J < _DataAlgo.getNbreJours(); J++)
}
