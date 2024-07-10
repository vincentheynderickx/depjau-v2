#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>


// Inclusion des fichiers nécessaires
#include "mesure.hpp"
#include "materiel.hpp"
#include <stdio.h>
#include <string>





// Constructeur de la classe MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this); // Initialisation de l'interface utilisateur
    this->setWindowTitle("DEPJAU V2");
    this->current_vertical = 0; // Initialisation de la verticale courante à zéro
    vertical verticale_vide = vertical(); // Création d'une verticale vide
    this->current_mesure.ajout_mesure(verticale_vide); // Ajout de la verticale vide à la mesure courante
    this->makePlot(ui->customPlot); // Configuration du fond du graphique
}

// Destructeur de la classe MainWindow
MainWindow::~MainWindow() {
    delete ui; // Libération de l'interface utilisateur
}

// Slot pour le bouton "Ajouter verticale"
void MainWindow::on_add_vert_clicked() {
    vertical verticale_vide = vertical(); // Création d'une verticale vide
    this->current_mesure.ajout_mesure(verticale_vide); // Ajout de la verticale vide à la mesure courante
    this->current_vertical = this->current_mesure.ensemble_des_mesures_des_verticales.size() - 1; // Mise à jour de la verticale courante
    this->update_display(); // Mise à jour de l'affichage
}

// Slot pour le bouton "Verticale précédente"
void MainWindow::on_prec_vert_clicked() {
    if (this->current_vertical > 0) {
        this->current_vertical--; // Décrémentation de l'indice de la verticale courante si possible
    }
    this->update_display(); // Mise à jour de l'affichage
}

// Slot pour le bouton "Verticale suivante"
void MainWindow::on_next_vert_clicked() {
    if (this->current_vertical < this->current_mesure.ensemble_des_mesures_des_verticales.size() - 1) {
        this->current_vertical++; // Incrémentation de l'indice de la verticale courante si possible
    }
    this->update_display(); // Mise à jour de l'affichage
}

// Slot pour le bouton "Ajouter mesure"
void MainWindow::on_add_mes_clicked() {
    une_mesure mesure_vide; // Création d'une mesure vide
    this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].ajout_mesure(mesure_vide); // Ajout de la mesure vide à la verticale courante
    ui->tableau_mesures->insertRow(ui->tableau_mesures->rowCount()); // Insertion d'une ligne dans le tableau des mesures
    this->update_display(); // Mise à jour de l'affichage
}

// Fonction de mise à jour de l'affichage
void MainWindow::update_display() {
    this->makePlot(ui->customPlot); // Configuration du fond du graphique

    // Construction de la chaîne de texte pour afficher le numéro de verticale courante
    std::string s = "Verticale ";
    s += std::to_string(this->current_vertical + 1);
    s += "/";
    s += std::to_string(this->current_mesure.ensemble_des_mesures_des_verticales.size());
    ui->label_vert_count->setText(s.c_str()); // Mise à jour du texte du label

    // Nombre de lignes dans le tableau de mesures
    int n_rows = this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].les_mesures_de_la_vertical.size();
    ui->tableau_mesures->setRowCount(n_rows); // Mise à jour du nombre de lignes du tableau

    // Boucle pour remplir le tableau avec les mesures de la verticale courante
    for (int i = 0; i < n_rows; i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(std::to_string(this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].les_mesures_de_la_vertical[i].profondeur_mesure).c_str());
        ui->tableau_mesures->setItem(i, 0, item);
        item = new QTableWidgetItem();
        item->setText(std::to_string(this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].les_mesures_de_la_vertical[i].nombre_tops).c_str());
        ui->tableau_mesures->setItem(i, 1, item);
    }

    // Affichage des distances et profondeurs de la verticale courante dans les champs de texte
    ui->input_dist->setText(std::to_string(this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].distance).c_str());
    ui->input_prof->setText(std::to_string(this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].profondeur).c_str());

    // Calcul du débit total et affichage
    std::string s2 = "Débit total : ";
    double x = this->current_mesure.debit();
    //fprintf(stderr, "debit : %d\n", x);
    std::cout << x << std::endl;
    s2 += std::to_string(x);
    ui->debit->setText(s2.c_str());
}

// Slot appelé lorsque le contenu d'une cellule du tableau de mesures change
void MainWindow::on_tableau_mesures_cellChanged(int row, int column) {
    QString item = ui->tableau_mesures->item(row, column)->text();
    QByteArray ba = item.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        if (column == 0) {
            this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].les_mesures_de_la_vertical[row].profondeur_mesure = stod(c_str);
        } else if (column == 1) {
            this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].les_mesures_de_la_vertical[row].nombre_tops = stol(c_str);
        }
    } catch (...) {
        ui->tableau_mesures->item(row, column)->setText("ERR");
    }
    this->makePlot(ui->customPlot);
}

// Slot appelé lorsque l'édition du champ de distance est terminée
void MainWindow::on_input_dist_editingFinished() {
    QString inpt = ui->input_dist->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].distance = stol(c_str);
    } catch (...) {
        ui->input_dist->setText("ERR");
    }
    this->update_display();
}

// Slot appelé lorsque l'édition du champ de profondeur est terminée
void MainWindow::on_input_prof_editingFinished() {
    QString inpt = ui->input_prof->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.ensemble_des_mesures_des_verticales[this->current_vertical].profondeur = stod(c_str);
    } catch (...) {
        ui->input_prof->setText("ERR");
    }
    this->update_display();
}

// Slot appelé lorsque l'édition du champ de coefficient de fond est terminée
void MainWindow::on_input_coeff_fond_editingFinished() {
    QString inpt = ui->input_coeff_fond->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.coeff_de_fond = stod(c_str);
    } catch (...) {
        ui->input_coeff_fond->setText("ERR");
    }
    this->update_display();
}

// Slot appelé lorsque l'édition du champ de coefficient de bord est terminée
void MainWindow::on_input_coeff_bord_editingFinished() {
    QString inpt = ui->input_coeff_bord->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.coeff_de_bord_droit = stod(c_str);
    } catch (...) {
        ui->input_coeff_fond->setText("ERR");
    }
    this->update_display();
}

// Slot appelé lorsque l'édition du champ de mesure du bord gauche est terminée
void MainWindow::on_input_dbgauche_editingFinished() {
    QString inpt = ui->input_dbgauche->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.distance_bord_gauche = stod(c_str);
    } catch (...) {
        ui->input_dbgauche->setText("ERR");
    }
    this->update_display();
}

// Slot appelé lorsque l'édition du champ de distance du bord droit est terminée
void MainWindow::on_input_dbdroit_editingFinished() {
    QString inpt = ui->input_dbdroit->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.distance_bord_droit = stod(c_str);
    } catch (...) {
        // En cas d'erreur, aucune action spécifique n'est prise ici
    }
    this->update_display();
}

// Slot appelé lorsque l'édition du champ de hauteur du bord gauche est terminée
void MainWindow::on_input_pbgauche_editingFinished() {
    QString inpt = ui->input_pbgauche->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.hauteur_bord_gauche = stod(c_str);
    } catch (...) {
        ui->input_pbgauche->setText("ERR");
    }
    this->update_display();
}

// Slot appelé lorsque l'édition du champ de hauteur du bord droit est terminée
void MainWindow::on_input_pbdroit_editingFinished() {
    QString inpt = ui->input_pbdroit->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.hauteur_bord_droit = stod(c_str);
    } catch (...) {
        ui->input_pbdroit->setText("ERR");
    }
    this->update_display();
}

// Fonction de configuration du fond du graphique
void MainWindow::makePlot(QCustomPlot *customPlot) {
    // Effacement des graphiques existants
    customPlot->clearGraphs();

    // Récupération du nombre de mesures verticales
    int numMesures = this->current_mesure.ensemble_des_mesures_des_verticales.size();

    // Vérification s'il y a des mesures
    if (numMesures == 0) {
        qDebug() << "Aucune mesure verticale disponible.";
        return;
    }

    // Initialisation des vecteurs pour stocker les distances et les profondeurs du fond et des mesures
    QVector<double> x_fond;
    QVector<double> y_fond;
    QVector<double> x_mesures;
    QVector<double> y_mesures;
    QVector<double> x_selected_mesures;
    QVector<double> y_selected_mesures;

    // Ajout du premier point de fond qui correspond au bord gauche
    x_fond.append(this->current_mesure.distance_bord_gauche);
    y_fond.append(0);


    // Indice de la verticale sélectionnée
    int selected_vertical_index = this->current_vertical;  // Assurez-vous que cet attribut est défini dans votre classe

    // Remplissage des vecteurs avec les distances et les profondeurs des mesures
    for (int i = 0; i < numMesures; ++i) {
        // Récupération de la mesure verticale courante
        vertical& vert = this->current_mesure.ensemble_des_mesures_des_verticales[i];

        // Ajout des points du fond
        x_fond.append(vert.distance);
        y_fond.append(-vert.profondeur); // Négatif pour inverser l'axe y

        // Ajout des points de mesure
        for (size_t j = 0; j < vert.les_mesures_de_la_vertical.size(); ++j) {
            if (i == selected_vertical_index) {
                x_selected_mesures.append(vert.distance);
                y_selected_mesures.append(-vert.les_mesures_de_la_vertical[j].profondeur_mesure); // Négatif pour inverser l'axe y
            } else {
                x_mesures.append(vert.distance);
                y_mesures.append(-vert.les_mesures_de_la_vertical[j].profondeur_mesure); // Négatif pour inverser l'axe y
            }
        }
    }

    // Ajout du dernier point de fond qui correspond au bord droit
    x_fond.append(this->current_mesure.distance_bord_droit);
    y_fond.append(0);

    // Ajout d'un graphique pour les points de fond
    customPlot->addGraph();
    customPlot->graph(0)->setData(x_fond, y_fond);
    customPlot->graph(0)->setLineStyle(QCPGraph::lsLine); // Connexion des points par des lignes

    // Définition du style de ligne pour le fond (graphique 0)
    QPen penFond;
    penFond.setColor(Qt::black); // Couleur noire
    penFond.setWidth(2); // Largeur de ligne
    customPlot->graph(0)->setPen(penFond); // Application du style

    // Ajout d'un graphique pour les points de mesure non sélectionnés
    customPlot->addGraph();
    customPlot->graph(1)->setData(x_mesures, y_mesures);
    customPlot->graph(1)->setLineStyle(QCPGraph::lsNone); // Pas de ligne, uniquement des points
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 8)); // Style de point : cercle de taille 8

    // Ajout d'un graphique pour les points de mesure sélectionnés
    customPlot->addGraph();
    customPlot->graph(2)->setData(x_selected_mesures, y_selected_mesures);
    customPlot->graph(2)->setLineStyle(QCPGraph::lsNone); // Pas de ligne, uniquement des points
    customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QColor(255, 165, 0), QColor(255, 165, 0), 8)); // Orange (RGB)

    // Définition des étiquettes pour les axes
    customPlot->xAxis->setLabel("Distance (m)");
    customPlot->yAxis->setLabel("Profondeur (m)");

    // Recalibrage des axes pour ajuster les données
    customPlot->rescaleAxes();

    // Récupération des plages actuelles des axes
    QCPRange xRange = customPlot->xAxis->range();
    QCPRange yRange = customPlot->yAxis->range();

    // Définition d'un facteur de marge
    double paddingFactor = 0.03;

    // Calcul de la quantité de marge pour chaque axe
    double xPadding = xRange.size() * paddingFactor;
    double yPadding = yRange.size() * paddingFactor;

    // Application de la marge aux plages des axes
    customPlot->xAxis->setRange(xRange.lower - xPadding, xRange.upper + xPadding);
    customPlot->yAxis->setRange(yRange.lower - yPadding, yRange.upper + yPadding);

    // Représentation pour appliquer les changements
    customPlot->replot();

    qDebug() << "makePlot a été appelé et le graphique a été mis à jour.";
}




void MainWindow::on_configHelice_clicked()
{
    HelixEquation* window = new HelixEquation(&(this->current_mesure.debitmetre));
    window->setWindowTitle("Equation de l'hélice");
    window->show();
}



void MainWindow::on_input_temps_acq_editingFinished()
{
    QString inpt = ui->input_temps_acq->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.temps_acquisition = stod(c_str);
    } catch (...) {
        ui->input_temps_acq->setText("ERR");
    }
    this->update_display(); // Mise à jour de l'affichage
}


void MainWindow::on_supp_vert_clicked()
{

}


void MainWindow::on_supp_mes_clicked()
{

}


void MainWindow::on_input_coef_bg_editingFinished()
{
    QString inpt = ui->input_coef_bg->text();
    QByteArray ba = inpt.toLocal8Bit();
    const char *c_str = ba.data();
    try {
        this->current_mesure.coeff_de_bord_gauche = stod(c_str);
    } catch (...) {
        ui->input_coef_bg->setText("ERR");
    }
    this->update_display(); // Mise à jour de l'affichage
}


void MainWindow::on_ouvrirButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Open XML File", "", "XML Files (*.xml)");
    if (!filename.isEmpty()) {
        current_mesure.from_xml(filename.toStdString());
        update_display();
        QMessageBox::information(this, "Success", "XML file loaded successfully.");
    }
}

void MainWindow::on_enregistrerButton_clicked() {
    QString filename = QFileDialog::getSaveFileName(this, "Save XML File", "", "XML Files (*.xml)");
    if (!filename.isEmpty()) {
        current_mesure.to_xml(this->current_mesure.debitmetre, filename.toStdString());
        QMessageBox::information(this, "Success", "XML file saved successfully.");
    }
}


void MainWindow::on_input_dbgauche_textChanged(const QString &arg1)
{

}


void MainWindow::on_tableau_mesures_itemChanged(QTableWidgetItem *item)
{
    std::cout << "test" << std::endl;
}


void MainWindow::on_pushButton_clicked()
{
    this->update_display(); // Mise à jour de l'affichage
}

