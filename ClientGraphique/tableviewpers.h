#ifndef TABLEVIEWPERS_H
#define TABLEVIEWPERS_H

#include <QTableView>
#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include <QModelIndex>

class QStandardItemModel;
class QSqlTableModel;


class TableViewPers : public QTableView
{
    Q_OBJECT

    public:
        TableViewPers(QWidget *parent);
    protected:
//        virtual void selectionChanged(const QItemSelection &selected,
//                              const QItemSelection &deselected);
        virtual void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint);
//        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    private :
        QModelIndex _defaultIndex;
        /* Pour gérer l'edition de la liste */
        class TablePersDelegate : public QItemDelegate
        {

        public:
            TablePersDelegate(QObject *parent = 0);

            QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;

            void setEditorData(QWidget *editor, const QModelIndex &index) const;

            void setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const;

            void updateEditorGeometry(QWidget *editor,
                const QStyleOptionViewItem &option, const QModelIndex &index) const;

            virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        };

        TablePersDelegate  *_EditorDelegate;

        QSqlTableModel *_SourceModel;

        /* */
        class ProxyModelForTablePers : public QSortFilterProxyModel {
        public :
                ProxyModelForTablePers(QObject* parent = 0);
                virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
                virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
                virtual QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const;
                virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
                virtual Qt::ItemFlags flags(const QModelIndex &index) const;
                virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
                virtual QModelIndex mapFromSource(const QModelIndex & source) const;
                virtual QModelIndex mapToSource(const QModelIndex & proxy) const;
                virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
                virtual QModelIndex 	parent ( const QModelIndex & child ) const;
                virtual QModelIndex sibling(int row, int column, const QModelIndex &idx) const;
                virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
//                virtual QItemSelection mapSelectionToSource(const QItemSelection &proxySelection) const;
//                virtual QItemSelection mapSelectionFromSource(const QItemSelection &sourceSelection) const;

        public:
            void RemoveProxyRow (const QModelIndex &aProxyIndex);

        private:
                QVector<QModelIndex> _ExtraItems;
                //cash des nouvelles valeurs
                QString _NewName;

        };

        ProxyModelForTablePers *_ProxyModel;

        void FillSourceModelViewPers();
        int GetNextColForEdition(int aCurrentCol);

    private slots:
        void OnClick(const QModelIndex &aindex);
        void ShowContextMenu (const QPoint& aPoint);
        void DeleteViewSelection ();
};



#endif // TABLEVIEWPERS_H
