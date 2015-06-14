#include "listeviewpers.h"
#include "Data/databasemanager.h"

ListeViewPers::ListeViewPers(QWidget *parent) :
    QListView(parent)
{

    _SourceModel = new QSqlQueryModel (this);
    //remplit avec les personnel
    _SourceModel->setQuery (DataBaseManager::getManager()->getQueryPersons());
    this->setModel(_SourceModel);
    this->setResizeMode(Adjust);
}
