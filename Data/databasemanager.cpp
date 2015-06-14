#include "databasemanager.h"
#include <QDir>
#include <QDebug>
#include <QDate>
#include <QSqlTableModel>


DataBaseManager* DataBaseManager::_Manager =0;

DataBaseManager* DataBaseManager::getManager() {

    if(!_Manager)
        _Manager = new DataBaseManager();

    return _Manager;

}

void DataBaseManager::killManager() {

    if (_Manager)
        delete _Manager;

    _Manager = 0;
}

DataBaseManager::DataBaseManager():
    _nameDB ("datas.sqlite")
{

//    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString _DBFilePath(QDir::home().path());
    _DBFilePath.append(QDir::separator()).append(_nameDB);
    _DBFilePath = QDir::toNativeSeparators(_DBFilePath);
//    #else
//    // NOTE: File exists in the application private folder
//    __DBFilePath = _nameDB;
//    #endif


    QFileInfo info (_DBFilePath);
    //if file exists
    bool lOK = false;
    if (QFile::exists(_DBFilePath))
    {
        lOK = openDB ();

    } else {
        lOK = createDB ();
    }

    //test

    if (lOK && insertPerson ("toto", "to", "x11888ee") >=0) {
        insertInfos (QDate::currentDate().toString(), QDate::currentDate().addDays(345).toString());
        if (insertIndispo (1, 737125))
             qDebug() << "insert indispo";
        QSqlQuery query1("select * from person");
        if (query1.isValid()) {

            while (query1.next()) {
                bool lOk = false;
                qDebug() <<"indispo";
                qDebug() << query1.value(0).toInt(&lOk);
                qDebug() << query1.value(1).toInt(&lOk);

            }
        }
        query1.finish();

        PersonData lPers;
        if (getPersonFromName ("toto", lPers))
            qDebug() << lPers._ShortName;
    }

}

DataBaseManager::~DataBaseManager() {

    _db.close();
}

bool DataBaseManager::openDB() {
    // Find QSLite driver
    _db = QSqlDatabase::addDatabase("QSQLITE");

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(_nameDB);
    path = QDir::toNativeSeparators(path);
    _db.setDatabaseName(path);
    #else
    // NOTE: File exists in the application private folder
    _db.setDatabaseName(_nameDB);
    #endif

    // Open database
    return _db.open();
}

bool DataBaseManager::createDB () {

    return (openDB() && createPersonTable()
                     && createIndisposTable()
                     && createInfosTable ()
                     && createCovoiturageTable ());
}


int DataBaseManager::insertPerson (const QString& aName,
                                    const QString& aShortName,
                                    const QString& aColor) {

    int newId = -1;

    if (_db.isOpen())
        {
        //http://www.sqlite.org/autoinc.html
        // NULL = is the keyword for the autoincrement to generate next value

        QSqlQuery query;
        bool ret = query.exec(QString("insert into person values(NULL,'%1','%2','%3')")
        .arg(aName).arg(aShortName).arg(aColor));

        // Get database given autoincrement value
        if (ret)
            {
            // http://www.sqlite.org/c3ref/last_insert_rowid.html
            newId = query.lastInsertId().toInt();
            }
        query.finish();

        }
    return newId;

}


bool DataBaseManager::insertIndispo (int aIdPers, int aIdDay) {

    bool ret = false;
    if (_db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec(QString("insert into indispos values(NULL,%1,%2)").arg(aIdPers).arg(aIdDay));
        query.finish();
    }

    return ret;
}

bool DataBaseManager::DeleteIndispo(int aIdPers, int aIdDay) {

    bool ret = false;
    if (_db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec(QString("delete from indispos where idpers=%1 and idday=%2").arg(aIdPers).arg(aIdDay));
        query.finish();
    }

    return ret;
}


void DataBaseManager::insertInfos (const QString &aDateDebut, const QString &aDateFin) {


    if (_db.isOpen())
    {
        QSqlQuery query;
        query.exec(QString("insert into infos values('%1','%2')").arg(aDateDebut).arg(aDateFin));
        query.finish();
    }

}

bool DataBaseManager::createPersonTable() {

    // Create table "person"
    bool ret = false;
    if (_db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec("create table person "
                  "(id integer primary key, "
                  "name varchar(20), "
                  "shortname varchar(5), "
                  "color varchar (10))") and query.exec ("create unique index personByName "
                                                         "on person (name)");
        query.finish();

    }
    return ret;
}

bool DataBaseManager::createIndisposTable() {

    bool ret = false;
    if (_db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec("create table indispos "
                  "(id integer primary key, "
                  "idpers integer, "
                  "idday integer)") and query.exec ("create unique index indisposByIdPersIdJour "
                                                   "on indispos(idpers, idday)");
        query.finish();


    }
    return ret;
}

bool DataBaseManager::createInfosTable() {

    bool ret = false;
    if (_db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec("create table infos "
                  "(datedebut varchar(20), "
                  "datefin varchar(20))");
        query.finish();


    }
    return ret;
}

bool DataBaseManager::createCovoiturageTable() {

    bool ret = false;
    if (_db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec("create table covoiturage "
                  "(id integer primary key, "
                  "idpers integer, "
                  "idday integer,"
                  "int numsol,"
                  "estcond boolean)") and query.exec ("create unique index CovByIdPersIdJourNumSol "
                                                      "on covoiturage(numSol, idpers, idday)");
        query.finish();


    }
    return ret;

}


bool DataBaseManager::LaPersonneEstIndispo (int aPers, int aIdDay) const {

    //cherche indispo
    QString lStrQuery = QString("select count(*) from indispos where idpers=%1  and idday= %2 ").arg(aPers).arg(aIdDay);
    QSqlQuery query (lStrQuery);
    if (query.next()) {

        bool lOk = false;
        int lNbre = query.value(0).toInt(&lOk);
        if (lOk && (lNbre > 0))
            return true;

    }

    return false;
}

bool DataBaseManager::getDateDebutEtFin (QDate& adateDebut, QDate& adateFin) const {

    QSqlQuery query ("select datedebut, datefin from infos");
    if (query.next()) {
        adateDebut = QDate::fromString(query.value(0).toString());
        adateFin   = QDate::fromString(query.value(1).toString());
        return (adateDebut.isValid() && adateFin.isValid());
    }
    return false;
}

bool DataBaseManager::getPersonFromName(const QString& aName, PersonData& person) const
    {

    if (!_db.isOpen()) return false;

    bool ret = false;

    QSqlQuery query(QString("select * from person where name='%1';").arg(aName));
    if (query.isValid() && query.next())
        {
        person._Id        = query.value(0).toInt();
        person._Name      = query.value(1).toString();
        person._ShortName = query.value(2).toString();
        person._Color     = query.value(3).toString();
        ret = true;
        }
    query.finish();

    return ret;
}

bool DataBaseManager::getPersonFromId(int aIdPers, PersonData& person) const {

    if (!_db.isOpen()) return false;

    bool ret = false;

    QSqlQuery query(QString("select * from person where id=%1;").arg(aIdPers));
    if (query.isValid() && query.next())
        {
        person._Id        = query.value(0).toInt();
        person._Name      = query.value(1).toString();
        person._ShortName = query.value(2).toString();
        person._Color     = query.value(3).toString();
        ret = true;
        }
    query.finish();

    return ret;

}

int DataBaseManager::getIdPersFromName (const QString &aNamePers) const {

    QSqlQuery query (QString("select id from person where name ='%1'").arg(aNamePers));
    if (query.next()) {
        bool lOk = false;
        int lIdPers = query.value(0).toInt(&lOk);
        if (lOk)
            return lIdPers;

     }

    return -1;
}

bool DataBaseManager::getIdPersons (QVector<int>& aIdPersons) const {

    aIdPersons.clear();

    QSqlQuery query (QString("select id from person;"));
    if (query.next()) {

        bool lOk = false;
        int lIdPers = query.value(0).toInt(&lOk);
        if (lOk)
            aIdPersons.push_back(lIdPers);

     }

    return (aIdPersons.count()>0);

}

void DataBaseManager::FillTablePerson (QSqlTableModel* aTableModel) const {

    aTableModel->setTable("person");
    aTableModel->select();
}

bool DataBaseManager::insertCovoiturage(int aIdPers, int aIdDay, bool aEstCond, int aNumSol) {

    bool ret = false;
    if (_db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec(QString("insert into covoiturage values(NULL,%1,%2,%3,%4)").arg(aIdPers).arg(aIdDay).arg(aNumSol).arg((int) aEstCond));
        query.finish();
    }

    return ret;

}

bool DataBaseManager::DeleteCovoiturage(int aIdPers, int aIdDay, int aNumSol) {

    bool ret = false;
    if (_db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec(QString("delete from covoiturage where idpers=%1 and idday=%2 and numsol=%3").arg(aIdPers).arg(aIdDay).arg(aNumSol));
        query.finish();
    }

    return ret;

}

bool DataBaseManager::getCovoiturage (int aIdPers, int aIdDay, bool& aEstCond, int aNumSol) const {

    if (!_db.isOpen()) return false;

    bool ret = false;
    aEstCond =  false;

    QSqlQuery query(QString("select estcond from covoiturage where idpers=%1 and idday=%2 and numsol=%3;").arg(aIdPers).arg(aIdDay).arg(aNumSol));
    if (query.isValid() && query.next())
    {
        aEstCond  = query.value(0).toBool();
        ret = true;
    }
    query.finish();

    return ret;
}

void DataBaseManager::ClearSolutions () {

    if (_db.isOpen())
    {
        QSqlQuery query;
        query.exec(QString("delete * from covoiturage;"));
        query.finish();
    }

}

