#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class FrameListeEtCalendar;
class FrameEntreeData;
class DataBaseManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    FrameListeEtCalendar  *_FrameChoixData,
                          *_FrameSolution;
    FrameEntreeData*      _FrameEntreeData;
    DataBaseManager* _DBmanager ;

    void ActualiserAffichageCourant ();
    QWidget * getCurrentWidget(QTabWidget* atab);

private slots:
    void OnOngletChanged (int);
};

#endif // MAINWINDOW_H
