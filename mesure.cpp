#include "mesure.hpp"
#include "materiel.hpp"
#include "calcul_debit.hpp"
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

double mesure::debit() {
    vector<double> debits_ss;
    vector<double> debits_lin;
    vector<double> vitesses;
    vector<double> hauteurs;

    for (size_t i = 0; i < ensemble_des_mesures_des_verticales.size(); ++i) {
        const vertical& vert = ensemble_des_mesures_des_verticales[i];
        vitesses.clear();
        hauteurs.clear();

        for (const auto& mes : vert.les_mesures_de_la_vertical) {
            hauteurs.push_back(mes.profondeur_mesure);
            double v = this->debitmetre.calcul_vitesse(mes.nombre_tops / this->temps_acquisition);
            vitesses.push_back(v);
        }
        debits_lin.push_back(debit_lineique(vitesses, hauteurs, vert.profondeur, coeff_de_fond));

        if (i == 0) {
            debits_ss.push_back(debit_sous_section_bord(debits_lin[i], vert.distance, distance_bord_gauche, hauteur_bord_gauche, vert.profondeur, coeff_de_bord));
        } else if (i == ensemble_des_mesures_des_verticales.size() - 1) {
            debits_ss.push_back(debit_sous_section_bord(debits_lin[i], vert.distance, distance_bord_droit, hauteur_bord_droit, vert.profondeur, coeff_de_bord));
        } else {
            debits_ss.push_back(debit_sous_section_general(debits_lin[i - 1], debits_lin[i], vert.distance, ensemble_des_mesures_des_verticales[i - 1].distance));
        }
    }
    return debit_total(debits_ss);
}

void mesure::to_xml(const Debitmetre& debi, const std::string& filename) const {
    pugi::xml_document doc;

    pugi::xml_node study = doc.append_child("study");
    study.append_attribute("type") = "NOINPUT";

    pugi::xml_node processings = study.append_child("processings");
    pugi::xml_node processing = processings.append_child("processing");

    auto add_parameter = [](pugi::xml_node& parent, const char* name, const char* value) {
        pugi::xml_node parameter = parent.append_child("parameter");
        parameter.append_attribute("name") = name;
        parameter.append_attribute("value") = value;
    };

    add_parameter(processing, "bottomRatio", to_string(coeff_de_fond).c_str());
    add_parameter(processing, "bottomZero", "true");
    add_parameter(processing, "hardwareType", "Micromoulinet");
    add_parameter(processing, "lastVerticalDistance", "0.6");
    add_parameter(processing, "lastVerticalIndex", "9");
    add_parameter(processing, "leftRatio", to_string(coeff_de_bord).c_str());
    add_parameter(processing, "measureTime", "30");
    add_parameter(processing, "method", "Multipoints");
    add_parameter(processing, "moduleName", "DEPJAU");

    pugi::xml_node propellerEquations = processing.append_child("object");
    propellerEquations.append_attribute("name") = "propellerEquations";

    auto add_propeller_parameter = [&](const char* name, const char* value) {
        pugi::xml_node parameter = propellerEquations.append_child("parameter");
        parameter.append_attribute("name") = name;
        parameter.append_attribute("value") = value;
    };

    add_propeller_parameter("a1", to_string(debi.pentes[0]).c_str());
    add_propeller_parameter("a2", to_string(debi.pentes[1]).c_str());
    add_propeller_parameter("a3", to_string(debi.pentes[2]).c_str());
    add_propeller_parameter("ak1", to_string(debi.intervalles[0]).c_str());
    add_propeller_parameter("ak2", to_string(debi.intervalles[0]).c_str());
    add_propeller_parameter("b1", to_string(debi.ordonnees[0]).c_str());
    add_propeller_parameter("b2", to_string(debi.ordonnees[1]).c_str());
    add_propeller_parameter("b3", to_string(debi.ordonnees[2]).c_str());

    add_parameter(processing, "rightRatio", to_string(coeff_de_bord).c_str());
    add_parameter(processing, "version", "1.00");

    pugi::xml_node verticals = processing.append_child("liste");
    verticals.append_attribute("name") = "verticals";

    for (const auto& vert : ensemble_des_mesures_des_verticales) {
        pugi::xml_node vertical = verticals.append_child("object");
        vertical.append_attribute("name") = "verticals";

        add_parameter(vertical, "distance", to_string(vert.distance).c_str());

        pugi::xml_node points = vertical.append_child("liste");
        points.append_attribute("name") = "points";

        for (const auto& measure : vert.les_mesures_de_la_vertical) {
            pugi::xml_node point = points.append_child("object");
            point.append_attribute("name") = "points";

            double vitesse = debi.calcul_vitesse(measure.nombre_tops);
            add_parameter(point, "height", to_string(measure.profondeur_mesure).c_str());
            add_parameter(point, "speed", to_string(vitesse).c_str());
            add_parameter(point, "topsNb", to_string(measure.nombre_tops).c_str());
        }

        add_parameter(vertical, "reducedPointsNb", "1");
        add_parameter(vertical, "totalDepth", to_string(vert.profondeur).c_str());
    }

    doc.save_file(filename.c_str());
}

void mesure::from_xml(const std::string& filename) {

    this->ensemble_des_mesures_des_verticales.clear();

    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str())) {
        cerr << "Erreur de chargement du fichier XML" << endl;
        return;
    }

    pugi::xml_node propellerEquations = doc.child("study").child("processings").child("processing").child("object");

    vector<double> pentes, ordonnees, intervalles;

    for (pugi::xml_node param = propellerEquations.child("parameter"); param; param = param.next_sibling("parameter")) {
        string name = param.attribute("name").value();
        double value = param.attribute("value").as_double();

        if (name == "a1" || name == "a2" || name == "a3") {
            pentes.push_back(value);
        } else if (name == "b1" || name == "b2" || name == "b3") {
            ordonnees.push_back(value);
        } else if (name == "ak1" || name == "ak2") {
            intervalles.push_back(value);
        }
    }

    Debitmetre debi("Helice_from_xml", intervalles.size(), intervalles, pentes, ordonnees);
    debitmetre = debi;

    pugi::xml_node processing = doc.child("study").child("processings").child("processing");

    for (pugi::xml_node param = processing.child("parameter"); param; param = param.next_sibling("parameter")) {
        string name = param.attribute("name").value();
        double value = param.attribute("value").as_double();

        if (name == "bottomRatio") {
            this->coeff_de_fond = value;
        } else if (name == "leftRatio" || name == "rightRatio") {
            this->coeff_de_bord = value;
        }
    }

    pugi::xml_node verticals_node = processing.child("liste");

    for (pugi::xml_node vertical_node = verticals_node.child("object"); vertical_node; vertical_node = vertical_node.next_sibling("object")) {
        double distance = 0;
        double profondeur = 0;

        for (pugi::xml_node param = vertical_node.child("parameter"); param; param = param.next_sibling("parameter")) {
            string name = param.attribute("name").value();
            double value = param.attribute("value").as_double();

            if (name == "distance") {
                distance = value;
            } else if (name == "totalDepth") {
                profondeur = value;
            }
        }

        vertical vert(distance, profondeur);

        pugi::xml_node points_node = vertical_node.child("liste");

        for (pugi::xml_node point_node = points_node.child("object"); point_node; point_node = point_node.next_sibling("object")) {
            double hauteur = 0;
            double vitesse = 0;
            int topsNb = 0;

            for (pugi::xml_node param = point_node.child("parameter"); param; param = param.next_sibling("parameter")) {
                string name = param.attribute("name").value();
                double value = param.attribute("value").as_double();

                if (name == "height") {
                    hauteur = value;
                } else if (name == "speed") {
                    vitesse = value;
                } else if (name == "topsNb") {
                    topsNb = static_cast<int>(value);
                }
            }

            une_mesure la_mesure;
            la_mesure.profondeur_mesure = hauteur;
            la_mesure.nombre_tops = topsNb;
            la_mesure.temps = 30;  // Placeholder value

            vert.ajout_mesure(la_mesure);
        }

        ajout_mesure(vert);
    }
}
