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

    void on_tableau_mesures_cellChanged(int row, int column);

    void on_input_dist_editingFinished();

    void on_input_prof_editingFinished();

    void on_input_coeff_fond_editingFinished();

    void on_input_coeff_bord_editingFinished();

    void on_input_dbgauche_editingFinished();

    void on_input_dbdroit_editingFinished();

    void on_input_pbgauche_editingFinished();

    void on_input_pbdroit_editingFinished();

    void makePlot(QCustomPlot *customPlot);



    void on_configHelice_clicked();

    void on_input_temps_acq_editingFinished();

    void on_supp_vert_clicked();

    void on_supp_mes_clicked();

    void on_input_coef_bg_editingFinished();

private:
    Ui::MainWindow *ui;
    mesure current_mesure;
    int current_vertical;
    void update_display();
};


// Classe de la fenêtre de configuration de l'hélice
class HelixEquation : public QWidget {
    Q_OBJECT
public:
    Debitmetre* helice;
    QDoubleSpinBox *aSpinBoxes[3];
    QDoubleSpinBox *bSpinBoxes[3];
    QDoubleSpinBox *nSpinBoxes[2];
    HelixEquation(Debitmetre* helice, QWidget *parent = nullptr) : QWidget(parent) {
        this->helice = helice;


        QGridLayout *layout = new QGridLayout(this);

        QLabel *equationLabels[3];
        for (int i = 0; i < 3; ++i) {
            equationLabels[i] = new QLabel("V =", this);
        }

        for (int i = 0; i < 3; ++i) {
            this->aSpinBoxes[i] = new QDoubleSpinBox(this);
            this->bSpinBoxes[i] = new QDoubleSpinBox(this);

            this->aSpinBoxes[i]->setRange(-1000000, 1000000);
            this->aSpinBoxes[i]->setDecimals(4);
            this->aSpinBoxes[i]->setValue(this->helice->pentes[i]);

            this->bSpinBoxes[i]->setRange(-1000000, 1000000);
            this->bSpinBoxes[i]->setDecimals(4);
            this->bSpinBoxes[i]->setValue(this->helice->ordonnees[i]);
        }


        for (int i = 0; i < 2; ++i) {
            this->nSpinBoxes[i] = new QDoubleSpinBox(this);
            this->nSpinBoxes[i]->setRange(-1000000, 1000000);
            this->nSpinBoxes[i]->setDecimals(4);
            this->nSpinBoxes[i]->setValue(this->helice->intervalles[i]);
        }

        layout->addWidget(equationLabels[0], 0, 0);
        layout->addWidget(this->aSpinBoxes[0], 0, 1);
        layout->addWidget(new QLabel("x N +", this), 0, 2);
        layout->addWidget(this->bSpinBoxes[0], 0, 3);

        for (int i = 1; i < 3; ++i) {
            layout->addWidget(equationLabels[i], i, 0);
            layout->addWidget(this->aSpinBoxes[i], i, 1);
            layout->addWidget(new QLabel("x N +", this), i, 2);
            layout->addWidget(this->bSpinBoxes[i], i, 3);
            layout->addWidget(new QLabel("pour N >", this), i, 4);
            layout->addWidget(this->nSpinBoxes[i - 1], i, 5);
        }

        QPushButton* save = new QPushButton("Valider");
        layout->addWidget(save, 3, 3);
        QObject::connect(save,&QPushButton::clicked, this, &HelixEquation::saveConfig);


        setLayout(layout);
    }

private slots:
    void saveConfig(){
        for (int i = 0; i < 3; ++i) {
            this->helice->pentes[i] = this->aSpinBoxes[i]->value();
            this->helice->ordonnees[i] = this->bSpinBoxes[i]->value();
            if(i < 2){
                this->helice->intervalles[i] = this->nSpinBoxes[i]->value();
            }
        }
        this->close();
    }
};


#endif // MAINWINDOW_H
