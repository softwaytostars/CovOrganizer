#include "framecalendar.h"
#include "ui_framecalendar.h"

#include <QDate>
#include "ClientGraphique/covcalendarview.h"
#include <QComboBox>

FrameCalendar::FrameCalendar(eTypeAffichageCalendar aTypeAffichage, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameCalendar)
{
    ui->setupUi(this);

    this->setLayout(ui->MainVLayout);
    ui->MainVLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    ui->VLayout1->addWidget(ui->groupBox);
    ui->VLayout1->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    ui->ButtonNext->setText(">>");
    ui->ButtonPrevious->setText("<<");

    ui->HLayoutChoixDate->addWidget(ui->comboMois);
    ui->HLayoutChoixDate->addWidget(ui->comboYear);
    ui->HLayoutChoixDate->addWidget(ui->ButtonPrevious);
    ui->HLayoutChoixDate->addWidget(ui->ButtonNext);
    ui->groupBox->setLayout(ui->HLayoutChoixDate);

    ui->groupBox->setTitle(tr("Date"));

    _CalendarView = new CovCalendarView(aTypeAffichage,
                                        this);

    ui->VLayout2->addWidget(_CalendarView);
    ui->VLayout2->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    connect (ui->comboMois, SIGNAL (currentIndexChanged (const QString&)), this, SLOT (OnMonthChanged(const QString&)));
    connect (ui->comboYear, SIGNAL (currentIndexChanged (const QString&)), this, SLOT (OnYearChanged(const QString&)));

}

FrameCalendar::~FrameCalendar()
{
    delete ui;
}

void FrameCalendar::Actualiser (int aIdPers, const QDate &aDateDebut, const QDate &aDateFin) {

    ui->comboMois->blockSignals(true);
    ui->comboYear->blockSignals(true);

    QDate lCurrentDate = QDate::currentDate();
    bool lInsideDates = ((lCurrentDate >= aDateDebut) && (lCurrentDate <= aDateFin) );
    if (!lInsideDates)
        lCurrentDate = aDateDebut;

    int lIndexMonth = -1;
    int lIndexYear  = -1;

    int lYearDebut = aDateDebut.year();
    int lYearFin   = aDateFin.year();
    int i =0;

    ui->comboYear->clear();
    for (int lYear = lYearDebut; lYear <= lYearFin; lYear++) {
        ui->comboYear->addItem(QString("%1").arg(lYear));
        if (lYear == lCurrentDate.year())
            lIndexYear = i;
        i++;
    }


    int lMoisDebut = 1;
    int lMoisFin   = 12;
    i = 0;
    ui->comboMois->clear();
    _mapMois.clear();
    for (int lMonth = lMoisDebut; lMonth <= lMoisFin; lMonth++) {

        QString lNameMonth = QDate::longMonthName(lMonth);
        _mapMois [lNameMonth] = lMonth;
        ui->comboMois->addItem(lNameMonth);
        if (lMonth ==  lCurrentDate.month())
           lIndexMonth = i;
        i++;

    }


    ui->comboMois->setCurrentIndex(lIndexMonth);
    ui->comboYear->setCurrentIndex(lIndexYear);

    ui->comboMois->blockSignals(false);
    ui->comboYear->blockSignals(false);

    //Actualisation du calendrier
    _CalendarView->Actualiser(aIdPers, lCurrentDate.month(), lCurrentDate.year());


}

void FrameCalendar::setPers (int aIdPers) {

    _CalendarView->setPers (aIdPers);
}

void FrameCalendar::OnMonthChanged(const QString& aStrMonth) {

    if (_mapMois.contains(aStrMonth))
        _CalendarView->setMonth (_mapMois[aStrMonth]);
}

void FrameCalendar::OnYearChanged(const QString& aStrYear) {

    bool lOk;
    int lYear = aStrYear.toInt(&lOk);
    if (lOk)
        _CalendarView->setYear(lYear);
}

void FrameCalendar::on_ButtonNext_clicked()
{
    int lIndexMois = ui->comboMois->currentIndex();
    if (lIndexMois < (ui->comboMois->count()-1))
        ui->comboMois->setCurrentIndex(++lIndexMois);
    else {
        int lIndexYear = ui->comboYear->currentIndex();
        if (lIndexYear < ((ui->comboYear->count()-1))) {
            ui->comboMois->setCurrentIndex(0);
            ui->comboYear->setCurrentIndex(++lIndexYear);
        }
    }
}

void FrameCalendar::on_ButtonPrevious_clicked()
{
    int lIndexMois = ui->comboMois->currentIndex();
    if (lIndexMois > 0 ) {
        ui->comboMois->setCurrentIndex(--lIndexMois);
    } else  {
        int lIndexYear = ui->comboYear->currentIndex();
        if (lIndexYear > 0) {
            ui->comboMois->setCurrentIndex((ui->comboMois->count()-1));
            ui->comboYear->setCurrentIndex(--lIndexYear);
        }
    }
}
