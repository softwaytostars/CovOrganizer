#ifndef DATAALGO_H
#define DATAALGO_H

#include <QMap>
#include <vector>
#include <gecode/int.hh>

/* Classe shared between search spaces */

class DataAlgo : public Gecode::SharedHandle
{
private:
    class DataAlgoO : public Gecode::SharedHandle::Object {
    public:
        int _NbreJours;
        int _NbrePers;
        QMap <int, int> _MapPersAlgoIdPers;//map le id pers et son numero algo
        QMap <int, int> _MapJourAlgoIdDay; //map entre le idday et son numero algo
        QMap <int, std::vector < int > > _MapJourAlgoPersPossibles;//map entre le jouralgo et les personnes dispos

        DataAlgoO ();
        DataAlgoO (const DataAlgoO& aDataAlgoO);
        virtual Object* copy(void) const {
            return new DataAlgoO (*this);
        }
        virtual ~DataAlgoO (void) {}

    };
public:
    DataAlgo();
    DataAlgo(const DataAlgo& aDataAlgo)
        : Gecode::SharedHandle (aDataAlgo) {}
    DataAlgo& operator =(const DataAlgo&  adataAlgo) {
        return static_cast<DataAlgo&> (Gecode::SharedHandle::operator =(adataAlgo));
    }

    inline const QMap <int, int>&  getMapPersAlgoIdPers () const { return static_cast<DataAlgoO*>(object())->_MapPersAlgoIdPers;}
    inline const QMap <int, int>&  getMapJourAlgoIdDay  () const { return static_cast<DataAlgoO*>(object())->_MapJourAlgoIdDay;}
    inline const QMap <int, std::vector < int > > & getMapJourAlgoPersPossibles () const { return static_cast<DataAlgoO*>(object())->_MapJourAlgoPersPossibles;}
    inline int getNbreJours () const {return static_cast<DataAlgoO*>(object())->_NbreJours;}
    inline int getNbrePers () const  {return static_cast<DataAlgoO*>(object())->_NbrePers;}

};

#endif // DATAALGO_H
