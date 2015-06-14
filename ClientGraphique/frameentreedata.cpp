#include "frameentreedata.h"
#include "ui_frameentreedata.h"


#include "tableviewpers.h"



//void FrameEntreeData::TablePersDelegate::closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint = NoHint) {

//        static_cast<QTableView*>(this->parent())->edit(model->index(0, CNumColAbrev));
//}

//class GamesSortProxyModel : public QSortFilterProxyModel {
//public:
//    GamesSortProxyModel (QObject *parent = 0);
//protected:
//     virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
//};

//GamesSortProxyModel::GamesSortProxyModel (QObject *parent)
//     : QSortFilterProxyModel(parent) {
//}


//bool GamesSortProxyModel::lessThan(const QModelIndex &left,
//                                      const QModelIndex &right) const
//{
//    QVariant leftData = sourceModel()->data(left);
//    QVariant rightData = sourceModel()->data(right);

//    QString leftString = leftData.toString();
//    QString rightString = rightData.toString();

//    return QString::localeAwareCompare(leftString, rightString) < 0;
//}

FrameEntreeData::FrameEntreeData(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameEntreeData)
{
    ui->setupUi(this);
    this->setLayout(ui->MainVLayout);
    ui->groupBoxDates->setLayout(ui->DatesLayout);
    ui->groupBoxDates->setTitle(tr("Dates de début et fin covoiturage"));
    ui->dateDebut->setDisplayFormat("dd.MM.yyyy");
    ui->dateDebut->setDate(QDate::currentDate());
    ui->dateDebut->setMinimumDate(QDate::currentDate().addDays(-5));
    ui->dateDebut->setMaximumDate(QDate::currentDate().addYears(2));
    ui->dateFin->setDisplayFormat("dd.MM.yyyy");
    ui->dateFin->setDate(ui->dateDebut->date());
    ui->dateFin->setMinimumDate(ui->dateDebut->minimumDate());
    ui->dateFin->setMaximumDate(ui->dateDebut->maximumDate());

    ui->groupBoxPers->setLayout(ui->GroupePersLayout);
    ui->groupBoxPers->setTitle(tr("Personnes pour covoiturage"));

    _tableView = new TableViewPers(this);
    ui->GroupePersLayout->addWidget(_tableView);


}

FrameEntreeData::~FrameEntreeData()
{
    delete ui;
}


