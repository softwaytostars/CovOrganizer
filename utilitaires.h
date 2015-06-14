#ifndef UTILITAIRES_H
#define UTILITAIRES_H

#include <QDate>

class Utilitaires
{
public:
    Utilitaires();
    static int CalculateIdDayFromDate (const QDate& aDate) { return (aDate.year()*366 + aDate.dayOfYear());}
};

#endif // UTILITAIRES_H
