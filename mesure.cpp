#include "mesure.hpp"
#include "materiel.hpp"
#include "calcul_debit.hpp"
#include "pugixml.hpp"
#include <iostream>
#include <string>


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


/*
void mesure::to_xml(Debimeter debi, const std::string& filename) {
    pugi::xml_document doc;

    // Créer l'élément racine
    pugi::xml_node study = doc.append_child("study");
    study.append_attribute("type") = "NOINPUT";

    // Créer l'élément processings
    pugi::xml_node processings = study.append_child("processings");

    // Créer l'élément processing
    pugi::xml_node processing = processings.append_child("processing");

    // Ajouter les paramètres
    auto add_parameter = [&](pugi::xml_node& parent, const char* name, const char* value) {
        pugi::xml_node parameter = parent.append_child("parameter");
        parameter.append_attribute("name") = name;
        parameter.append_attribute("value") = value;
    };

    add_parameter(processing, "bottomRatio", std::to_string(mesure.coeff_de_fond));
    add_parameter(processing, "bottomZero", "true");
    add_parameter(processing, "hardwareType", "Micromoulinet");
    add_parameter(processing, "lastVerticalDistance", "0.6");
    add_parameter(processing, "lastVerticalIndex", "9");
    add_parameter(processing, "leftRatio", std::to_string(mesure.coeff_de_bord));
    add_parameter(processing, "measureTime", "30");
    add_parameter(processing, "method", "Multipoints");
    add_parameter(processing, "moduleName", "DEPJAU");

    // Ajouter l'objet propellerEquations
    pugi::xml_node propellerEquations = processing.append_child("object");
    propellerEquations.append_attribute("name") = "propellerEquations";

    auto add_propeller_parameter = [&](const char* name, const char* value) {
        pugi::xml_node parameter = propellerEquations.append_child("parameter");
        parameter.append_attribute("name") = name;
        parameter.append_attribute("value") = value;
    };
    ak1=debi.intervalles[0];ak2=debi.intervalles[0]
    a1=debi.pentes[0];a2=debi.pentes[1];a3=debi.pentes[2];
    b1=debi.ordonnées[0];b2=debi.ordonnées[1];b2=debi.ordonnées[0];

    add_propeller_parameter("a1", std::to_string(a1));
    add_propeller_parameter("a2", std::to_string(a2));
    add_propeller_parameter("a3", std::to_string(a3));
    add_propeller_parameter("ak1", std::to_string(ak1));
    add_propeller_parameter("ak2", std::to_string(ak2));
    add_propeller_parameter("b1", std::to_string(b1));
    add_propeller_parameter("b2", std::to_string(b2));
    add_propeller_parameter("b3", std::to_string(b3));

    add_parameter(processing, "rightRatio", std::to_string(mesure.coeff_de_bord));
    add_parameter(processing, "version", "1.00");

    // Ajouter la liste des verticales
    pugi::xml_node verticals = processing.append_child("liste");
    verticals.append_attribute("name") = "verticals";

    // Ajouter chaque verticale
    for (const auto& vert : ensemble_des_mesures_des_verticales) {
        pugi::xml_node vertical = verticals.append_child("object");
        vertical.append_attribute("name") = "verticals";

        // Ajouter les paramètres de chaque verticale
        add_parameter(vertical, "distance", std::to_string(vert.distance).c_str());

        pugi::xml_node points = vertical.append_child("liste");
        points.append_attribute("name") = "points";

        for (const auto& measure : vert.les_mesures_de_la_vertical) {
            pugi::xml_node point = points.append_child("object");
            point.append_attribute("name") = "points";

            add_parameter(point, "height", std::to_string(measure.profondeur_mesure).c_str());
            add_parameter(point, "speed", std::to_string(measure.cote).c_str());
            add_parameter(point, "topsNb", std::to_string(measure.nombre_tops).c_str());
        }

        add_parameter(vertical, "reducedPointsNb", "1"); // Placeholder
        add_parameter(vertical, "totalDepth", std::to_string(vert.profondeur).c_str());
    }

    // Sauvegarder le fichier XML
    doc.save_file(filename.c_str());
}
*/


