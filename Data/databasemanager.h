#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include "Types/TypeData.h"
#include <QVector>

class QSqlTableModel;


class DataBaseManager : public QObject
{
public:
     QSqlDatabase& getDataBase() {return _db;}

    int insertPerson (const QString& aName,
                      const QString& aShortName,
                      const QString& aColor);
    bool insertIndispo(int aIdPers, int aIdDay);
    bool DeleteIndispo(int aIdPers, int aIdDay);
    void insertInfos (const QString& aDateDebut, const QString& aDateFin);
    //table covoiturage
    bool insertCovoiturage (int aIdPers, int aIdDay, bool aEstCond, int aNumSol=1);
    bool DeleteCovoiturage(int aIdPers, int aIdDay, int aNumSol=1);

    static DataBaseManager* getManager();
    static void killManager();

    //accesseurs infos database
    bool LaPersonneEstIndispo (int aPers, int aIdDay) const;
    bool getDateDebutEtFin (QDate& adateDebut, QDate& adateFin) const;
    bool getPersonFromName(const QString &aName, PersonData& person) const;
    bool getPersonFromId(int aIdPers, PersonData& person) const;
    int  getIdPersFromName (const QString &aNamePers) const;
    bool getIdPersons (QVector<int>& aIdPersons) const;
    QString getQueryPersons () const { return QString("select name from person");}
    void FillTablePerson (QSqlTableModel* aTableModel) const;
    /* Pour solutions */
    bool getCovoiturage (int aIdPers, int aIdDay, bool& aEstCond, int aNumSol=1) const;
    void ClearSolutions ();
private :
    DataBaseManager();
    ~DataBaseManager();
    QSqlDatabase _db;
    QString _nameDB;
    QString _DBFilePath;
    bool openDB();
    bool createDB();
    bool createPersonTable();
    bool createIndisposTable();
    bool createInfosTable();
    bool createCovoiturageTable();
    static DataBaseManager* _Manager;
};

#endif // DATABASEMANAGER_H
