#ifndef FRAMECALENDAR_H
#define FRAMECALENDAR_H

#include <QFrame>
#include <QMap>

#include "ClientGraphique/Types/TypeAffichageCalendar.hpp"

namespace Ui {
class FrameCalendar;
}

class CovCalendarView;

class FrameCalendar : public QFrame
{
    Q_OBJECT

public:
    explicit FrameCalendar(eTypeAffichageCalendar aTypeAffichage, QWidget *parent = 0);
    ~FrameCalendar();
    void setPers (int aIdPers);

    void Actualiser (int aIdPers, const QDate &aDateDebut, const QDate &aDateFin);

private:
    Ui::FrameCalendar *ui;
    CovCalendarView * _CalendarView;
    QMap<QString, int> _mapMois;
private slots:
    void OnMonthChanged(const QString&aStrMonth);
    void OnYearChanged(const QString&aStrYear);
    void on_ButtonNext_clicked();
    void on_ButtonPrevious_clicked();
};

#endif // FRAMECALENDAR_H
