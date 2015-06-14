#ifndef FRAMELISTEETCALENDAR_H
#define FRAMELISTEETCALENDAR_H

#include <QFrame>
#include "ClientGraphique/listeviewpers.h"
#include "ClientGraphique/Types/TypeAffichageCalendar.hpp"
#include "ClientGraphique/iactualisable.h"

class IGestionFrameCalendar;

namespace Ui {
class FrameListeEtCalendar;
}

class QSplitter;
class QListWidgetItem;
class FrameCalendar;

class FrameListeEtCalendar : public QFrame, public IActualisable
{
    Q_OBJECT

public:
    explicit FrameListeEtCalendar(eTypeAffichageCalendar aTypeAffichage, QWidget *parent = 0);
    ~FrameListeEtCalendar();

    virtual void Actualiser();

private:
    Ui::FrameListeEtCalendar *ui;
    QSplitter* _splitter;
    ListeViewPers *_listeDatas;
    FrameCalendar *_FrameCalendar;
private slots:
    void OnClickListeData(const QModelIndex &aIndex);
};

#endif // FRAMELISTEETCALENDAR_H
