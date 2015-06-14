#ifndef FRAMEENTREEDATA_H
#define FRAMEENTREEDATA_H

#include <QFrame>
#include <QItemDelegate>
#include <QTableView>

namespace Ui {
class FrameEntreeData;
}

class QStandardItemModel;
class TableViewPers;

class FrameEntreeData : public QFrame
{
    Q_OBJECT

public:
    explicit FrameEntreeData(QWidget *parent = 0);
    ~FrameEntreeData();


private:
    Ui::FrameEntreeData *ui;

    TableViewPers *_tableView;


};

#endif // FRAMEENTREEDATA_H
