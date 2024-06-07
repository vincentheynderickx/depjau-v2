#include "mesure.hpp"
#include "materiel.hpp"
#include "calcul_debit.hpp"

using namespace std;


double mesure::debit(){
    vector<double> debits_ss;
    vector<double> debits_lin;

    vector<double> vitesses;
    vector<double> hauteurs;

    une_mesure mes;
    vertical vert;

    for (int i=0; i < this->ensemble_des_mesures_des_verticales.size(); i++){
        vert = this->ensemble_des_mesures_des_verticales[i];

        vitesses.clear();
        hauteurs.clear();

        for (int j=0; j < vert.les_mesures_de_la_vertical.size(); j++){
            mes = vert.les_mesures_de_la_vertical[j];
            hauteurs.push_back(mes.profondeur_mesure);
            double v = this->debitmetre.calcul_vitesse(mes.nombre_tops / mes.temps);
            vitesses.push_back(v);
        }
        debits_lin.push_back(debit_lineique(vitesses, hauteurs, vert.profondeur, this->coeff_de_fond));
        // debit bord gauche
        if (i == 0){
            debits_ss.push_back(debit_sous_section_bord(debits_lin[i], vert.distance, this->distance_bord_gauche, this->hauteur_bord_gauche, vert.profondeur, this->coeff_de_bord));
        }
        // debits sections centrales
        if (i > 0){
            debits_ss.push_back(debit_sous_section_general(debits_lin[i-1], debits_lin[i], vert.distance, this->ensemble_des_mesures_des_verticales[i-1].distance));
        }
        // debit bord droit
        if (i == this->ensemble_des_mesures_des_verticales.size() - 1){
            debits_ss.push_back(debit_sous_section_bord(debits_lin[i], vert.distance, this->distance_bord_droit, this->hauteur_bord_droit, vert.profondeur, this->coeff_de_bord));
        }
    }
    return debit_total(debits_ss);
}
