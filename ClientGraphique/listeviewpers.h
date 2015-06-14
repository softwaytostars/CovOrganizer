#ifndef LISTEVIEWPERS_H
#define LISTEVIEWPERS_H

#include <QListView>
#include <QSqlQueryModel>

class ListeViewPers : public QListView
{
    Q_OBJECT
public:
    explicit ListeViewPers(QWidget *parent = 0);

signals:

public slots:
private:
    QSqlQueryModel *_SourceModel;

};

#endif // LISTEVIEWPERS_H
