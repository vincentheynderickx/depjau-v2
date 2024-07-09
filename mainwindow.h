#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "mesure.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_vert_clicked();

    void on_prec_vert_clicked();

    void on_next_vert_clicked();

    void on_add_mes_clicked();

    void on_ouvrirButton_clicked();

    void on_enregistrerButton_clicked();

    void on_tableau_mesures_cellChanged(int row, int column);

    void on_input_dist_editingFinished();

    void on_input_prof_editingFinished();

    void on_input_coeff_fond_editingFinished();

    void on_input_coeff_bord_editingFinished();

    void on_input_dbgauche_editingFinished();

    void on_input_dbdroit_editingFinished();

    void on_input_pbgauche_editingFinished();

    void on_input_pbdroit_editingFinished();

    void setupPlot(QCustomPlot *customPlot);

    void setupFond(QCustomPlot *customPlot);



private:
    Ui::MainWindow *ui;
    mesure current_mesure;
    int current_vertical;
    void update_display();
};
#endif // MAINWINDOW_H
