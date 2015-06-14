#include "tableviewpers.h"

#include <QColorDialog>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QLineEdit>
#include <QPainter>
#include <QDebug>
#include <QMenu>
#include <QApplication>

#include "Data/databasemanager.h"

static const int CNumColId     = 0;
static const int CNumColName   = 1;
static const int CNumColAbrev  = 2;
static const int CNumColColor  = 3;
static const int CNumColIconTrash  = 4;

TableViewPers::TableViewPers (QWidget *parent)
    : QTableView (parent)
{

    this->setEditTriggers(QAbstractItemView::AllEditTriggers);
    //permet le tri sur clic des titres
    this->setSortingEnabled(true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
   // this->setContextMenuPolicy(Qt::CustomContextMenu);
   // connect (this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT (ShowContextMenu(const QPoint&)));
    //this->setSelectionMode(QAbstractItemView::ContiguousSelection);
//    MultiSelection,
//    ExtendedSelection,
//    ContiguousSelection

    //creation du model source
    _SourceModel = new QSqlTableModel (this, DataBaseManager::getManager()->getDataBase());//base par defaut//new QStandardItemModel(this);

    _ProxyModel = new ProxyModelForTablePers (this);

    QSortFilterProxyModel* ltest =new QSortFilterProxyModel(this);
    ltest->setSourceModel(_SourceModel);
    _ProxyModel->setSourceModel(_SourceModel);

    this->setModel(_ProxyModel);

   // this->resizeColumnsToContents();

    _EditorDelegate = new TablePersDelegate(this);
    this->setItemDelegate(_EditorDelegate);

    //remplit les donnees
    FillSourceModelViewPers();

    connect (this, SIGNAL(clicked(const QModelIndex &)), this, SLOT (OnClick(const QModelIndex &)));
}



void TableViewPers::OnClick(const QModelIndex &aindex) {

    if ((aindex.row()>0) && (aindex.column() == CNumColColor ))
    {
        QColor color = QColorDialog::getColor();
        if (color.isValid())
            this->model()->setData(aindex, color);
    }

    if ((aindex.row()>0) && (aindex.column() == CNumColIconTrash)) {

        //this->selectionModel()->select();
        QModelIndex lIndex = this->selectionModel()->currentIndex();
        _ProxyModel->RemoveProxyRow(lIndex);
        FillSourceModelViewPers();
    }
}

void TableViewPers::ShowContextMenu (const QPoint& aPoint) {

    //QModelIndex lIndex = this->indexAt(aPoint);
    QMenu menu;
    QAction *ActionDelete = menu.addAction (tr("Delete the line"));
    connect (ActionDelete, SIGNAL (triggered()), this, SLOT (DeleteViewSelection()));
    menu.exec(QCursor::pos());

}

void TableViewPers::DeleteViewSelection () {

    QModelIndex lIndex = this->selectionModel()->currentIndex();
    //_SourceModel->removeRow(lIndex.row());
    //_ProxyModel->removeRow(lIndex.row())
}

void TableViewPers::closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint) {

    int lNextCol = GetNextColForEdition(this->currentIndex().column());
    int lRow = this->currentIndex().row();
    QTableView::closeEditor(editor, hint);


    if (lNextCol < 0) return;

    this->setCurrentIndex(this->model()->index(lRow, lNextCol));
    this->edit(this->model()->index(lRow, lNextCol));
}

//void TableViewPers::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {


//}


int TableViewPers::GetNextColForEdition(int aCurrentCol) {

    if (aCurrentCol == CNumColName)
        return CNumColAbrev;

    if (aCurrentCol == CNumColAbrev)
        return CNumColColor;

    return -1;
}

void TableViewPers::FillSourceModelViewPers() {


    _SourceModel->clear();
    _SourceModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    //titre des colonnes
//    _SourceModel->setHeaderData(CNumColId,   Qt::Horizontal, tr("Id"));
//    _SourceModel->setHeaderData(CNumColName,   Qt::Horizontal, tr("Nom"));
//    _SourceModel->setHeaderData(CNumColAbrev,  Qt::Horizontal, tr("Abrev"));
//    _SourceModel->setHeaderData(CNumColColor,  Qt::Horizontal, tr("Couleur"));

    DataBaseManager::getManager()->FillTablePerson (_SourceModel);

    this->setColumnHidden(0, true);//cacher la colonne ID

}


TableViewPers::TablePersDelegate::TablePersDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *TableViewPers::TablePersDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex & index ) const
{
    int lCol = index.column();

    if (lCol == CNumColColor) {

        return 0;
//           QColorDialog *colorDialog = new QColorDialog(parent);
           /* define it as a Qt::Widget (SubWindow would also work) instead of a dialog */
          // colorDialog->setWindowFlags(Qt::Widget);
           /* a few options that we must set for it to work nicely */
////           colorDialog->setOptions(
////                       /* do not use native dialog */
////                       QColorDialog::DontUseNativeDialog
////                       /* you don't need to set it, but if you don't set this
////                           the "OK" and "Cancel" buttons will show up, I don't
////                           think you'd want that. */
////                       | QColorDialog::NoButtons
////           );
//           return colorDialog;
    }

    QLineEdit* editor = new QLineEdit(parent);
    //maximum 4 caracteres possibles pour l'abreviation
    if (lCol == CNumColAbrev)
        editor->setMaxLength(4);
    return editor;

}

void TableViewPers::TablePersDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int lCol = index.column();
    if (lCol == CNumColColor) {

        QItemDelegate::setEditorData(editor, index);
        return;
    }

    QString value = index.model()->data(index, Qt::EditRole).toString();
    //QString ltest = index.data().toString();
    QLineEdit* line = static_cast<QLineEdit*>(editor);
    line->setText (value);
}

void TableViewPers::TablePersDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    int lCol = index.column();
    if (lCol == CNumColColor) {

        QItemDelegate::setModelData(editor, model, index);
        return;
    }

   // model->setData(index, value, Qt::EditRole);
    QLineEdit* line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    model->setData(index, value);

}

void TableViewPers::TablePersDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


void TableViewPers::TablePersDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {



    QStyleOptionViewItem opt(option);

   if (index.column() == CNumColColor)
   {
       opt.palette.setColor(
       QPalette::Highlight,
       QColor(index.model()->data(index, Qt::BackgroundRole).toString()));

       opt.palette.setColor(
       QPalette::Window,
       QColor(index.model()->data(index, Qt::BackgroundRole).toString()));
   }

   return QItemDelegate::paint(painter, opt, index);

}



TableViewPers::ProxyModelForTablePers::ProxyModelForTablePers (QObject* parent)
    : QSortFilterProxyModel (parent) {

    _ExtraItems.push_back(this->createIndex(0, 0));
    _ExtraItems.push_back(this->createIndex(0, 1));
    _ExtraItems.push_back(this->createIndex(0, 2));
    _ExtraItems.push_back(this->createIndex(0, 3));

}

int TableViewPers::ProxyModelForTablePers::rowCount(const QModelIndex &parent) const {

    if (this->sourceModel())
        return (this->sourceModel()->rowCount()+1);
    else return 1;
}

QVariant TableViewPers::ProxyModelForTablePers::data(const QModelIndex &proxyIndex, int role) const {

    if (!proxyIndex.isValid()) return QVariant();

    if (proxyIndex.column() == CNumColIconTrash) {

        if ((proxyIndex.row() > 0) && (role == Qt::DecorationRole))
            return QVariant (QApplication::style()->standardIcon(QStyle::SP_TrashIcon));
        else return QVariant ();
    }

    switch (role) {
        case Qt::DisplayRole : { if (proxyIndex.row() == 0) {
                                        switch (proxyIndex.column()) {
                                            case CNumColName : { if (_NewName.size() > 0)
                                                        return QVariant (_NewName);
                                                      else
                                                        return QVariant (tr("new"));
                                                     }

                                           default : return QVariant("");
                                        }

                                    } else if (proxyIndex.column() != CNumColColor)
                                        return QSortFilterProxyModel::data(proxyIndex, role);
                                    else return QVariant();
                                 }

        case Qt::EditRole : { if (proxyIndex.row() == 0)
                                return QVariant ("");
                              else
                                return QSortFilterProxyModel::data(proxyIndex, role);
                             }
        case Qt::ToolTipRole : return QVariant ("");
        case Qt::BackgroundRole : { if ((proxyIndex.row()>0) && (proxyIndex.column() == CNumColColor) &&
                                             (QSortFilterProxyModel::data(proxyIndex, Qt::DisplayRole).toString().size() > 0))
                                            return QVariant (QColor (QSortFilterProxyModel::data(proxyIndex, Qt::DisplayRole).toString()));
                                        else return QVariant(QColor (Qt::white));
                                         }
        default : return QVariant ();


    }
}

bool TableViewPers::ProxyModelForTablePers::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (value.toString().size() == 0) return false;
    if (index.column() == CNumColIconTrash) return false;

    if (index.row() == 0) {

        switch (index.column()) {
            case 0 : {  return true;

                     }
            case 1 : {_NewName = value.toString();
                         return true;

                      }
            case 2 : {if (!this->sourceModel()->insertRows (this->sourceModel()->rowCount(), 1)) {_NewName = ""; return false;}
                      bool lOk = this->sourceModel()->setData (this->sourceModel()->index(this->sourceModel()->rowCount()-1, 1),_NewName, role);
                      if (lOk)
                          lOk = this->sourceModel()->setData (this->sourceModel()->index(this->sourceModel()->rowCount()-1, 2), value, role);
                      _NewName = "";
                      return lOk;
                     }
        }
    }
    else
        return this->sourceModel()->setData (this->mapToSource(index), value, role);
}

Qt::ItemFlags TableViewPers::ProxyModelForTablePers::flags(const QModelIndex &index) const {

    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    if (index.column() != CNumColIconTrash)
        flags = flags | Qt::ItemIsEditable;
    return flags;
}

QModelIndex TableViewPers::ProxyModelForTablePers::index(int row, int column, const QModelIndex &parent) const {

    if ((row <0 ) || (column<0))
        return QModelIndex();

    if (column == CNumColIconTrash)
        return this->createIndex(row, column);

    if (row == 0) {
        if ((column >=0) && (column < _ExtraItems.count()))
            return _ExtraItems [column];
        else
            return QModelIndex();
    } else {
        return this->createIndex(row, column, this->sourceModel()->index(row-1, column).internalPointer());
    }


}

QModelIndex TableViewPers::ProxyModelForTablePers::mapFromSource(const QModelIndex & source) const
{
    if (!sourceModel())
        return QModelIndex();

    if (!source.isValid())
        return QModelIndex();

    if (source.column() == CNumColIconTrash)
       return QModelIndex();


    return index(source.row(), source.column());
}

QModelIndex TableViewPers::ProxyModelForTablePers::mapToSource(const QModelIndex & proxy) const
{

    if (!sourceModel())
        return QModelIndex();

    if (proxy.row()==0)
        return QModelIndex();

    if (proxy.column() == CNumColIconTrash)
        return QModelIndex();

    return this->sourceModel()->index(proxy.row()-1, proxy.column());
}

QVariant TableViewPers::ProxyModelForTablePers::headerData(int section, Qt::Orientation orientation, int role) const {

    //return QSortFilterProxyModel::headerData (section, orientation, role);
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical) {

        if (section == 0)
            return QVariant("");

        return QVariant(section);
    }

    switch(section) {
        case CNumColId : return QVariant (tr("Id"));
        case CNumColName : return QVariant (tr("Nom"));
        case CNumColAbrev : return QVariant (tr("Abrev"));
        case CNumColColor : return QVariant (tr("Couleur"));
    }
    return QVariant ("");

}

int TableViewPers::ProxyModelForTablePers::columnCount ( const QModelIndex & parent ) const {

    if (sourceModel())
        return (sourceModel()->columnCount()+1);//les colonnes du sourcemodel + 1 colonne Trash
    else return 5;
}

QModelIndex TableViewPers::ProxyModelForTablePers::parent ( const QModelIndex & child ) const {

    return QModelIndex();
}

QModelIndex TableViewPers::ProxyModelForTablePers::sibling(int row, int column, const QModelIndex &idx) const {

   return QModelIndex();
}

bool TableViewPers::ProxyModelForTablePers::removeRows(int row, int count, const QModelIndex &parent) {

    bool lOk = this->sourceModel()->removeRows (row-1, count);
    return lOk;
}

void TableViewPers::ProxyModelForTablePers::RemoveProxyRow (const QModelIndex &aProxyIndex) {

    beginRemoveRows(QModelIndex(), aProxyIndex.row(), aProxyIndex.row());
    this->removeRows(aProxyIndex.row(), 1);
    endRemoveRows();
}

//QItemSelection TableViewPers::ProxyModelForTablePers::mapSelectionToSource ( const QItemSelection & proxySelection ) const {

//    QItemSelection res;

//    QModelIndexList lListe = proxySelection.indexes();
//    qDebug() << lListe.count();

//    for (int i=0; i < lListe.count();i++) {

//        res.append(QItemSelectionRange (mapToSource (lListe.at(i))));

//   }
//    return res;

//}

//QItemSelection 	TableViewPers::ProxyModelForTablePers::mapSelectionFromSource ( const QItemSelection & sourceSelection ) const {


// QItemSelection res;
// return res;
//}


