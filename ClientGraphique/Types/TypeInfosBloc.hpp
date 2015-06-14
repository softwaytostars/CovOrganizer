#ifndef TYPEINFOSBLOC_HPP
#define TYPEINFOSBLOC_HPP

#include <QString>

struct sInfosBloc {
    bool _IsTitle;
    bool _IsEnabled;
    QString _InfoDay;
    QString _InfoBloc;
    bool _IsDispo;
    int _IdDay; /* Identifiant of the day */
    sInfosBloc (bool aIsTitle,
                bool aIsEnabled,
                const QString& aInfoDay,
                const QString& aInfoBloc) :
        _IsTitle   (aIsTitle),
        _IsEnabled (aIsEnabled),
        _InfoDay   (aInfoDay),
        _InfoBloc  (aInfoBloc),
        _IsDispo(true){}
};

#endif // TYPEINFOSBLOC_HPP
