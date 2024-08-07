#ifndef __MESURE_H
#define __MESURE_H

#include <iostream>
#include <vector>
#include <string>

#include "materiel.hpp"



class une_mesure
{
public:
    double heure;
    double cote;
    double profondeur_mesure;
    int nombre_tops;
    double temps;

    une_mesure(){this->profondeur_mesure = 0;
        this->nombre_tops = 0;
        this->temps = 30;};

    une_mesure(double heure_en_minute, double cote_en_cm,
               double profondeur_mesure_en_cm , int nombre_de_tops, double temps_dizieme_de_secondes) :   heure(heure_en_minute), cote(cote_en_cm),
        profondeur_mesure(profondeur_mesure_en_cm),
        nombre_tops(nombre_de_tops), temps(temps_dizieme_de_secondes) {}
};

class vertical
{
public:
    double distance;
    double profondeur;
    std::vector<une_mesure> les_mesures_de_la_vertical;
    vertical(){this->profondeur = 0;
        this->distance = 0;};
    vertical(double distance_en_cm, double profondeur_en_cm) : distance(distance_en_cm), profondeur(profondeur_en_cm) {}
    void ajout_mesure(une_mesure la_mesure) { les_mesures_de_la_vertical.push_back(la_mesure); };
};

class mesure
{
public:
    double temperature_eau;
    double temperature_air;
    double conductivite;
    double temp_mesure_conductivite;
    double coeff_de_bord_droit;
    double coeff_de_bord_gauche;
    double coeff_de_fond;
    double distance_bord_gauche;
    double distance_bord_droit;
    double hauteur_bord_gauche;
    double hauteur_bord_droit;
    std::string commentaires;

    Debitmetre debitmetre;

    double temps_acquisition;

    mesure(double temp_eau, double temp_air, double conductivite_mesure, double temp_conductivite, double coeff_de_bord_doit, double coeff_de_bord_gauche, double coeff_de_fond) : temperature_eau(temp_eau), temperature_air(temp_air),
                                                                                                                                conductivite(conductivite_mesure), temp_mesure_conductivite(temp_conductivite), coeff_de_bord_droit(coeff_de_bord_droit),coeff_de_bord_gauche(coeff_de_bord_gauche), coeff_de_fond(coeff_de_fond) {}
    std::vector<vertical> ensemble_des_mesures_des_verticales;
    void ajout_mesure(vertical la_vertical) { ensemble_des_mesures_des_verticales.push_back(la_vertical); };
    

    void to_xml(const Debitmetre& debi, const std::string& filename) const;

    void from_xml(const std::string& filename);

    mesure(){this->coeff_de_bord_droit = 0;
        this->coeff_de_bord_gauche = 0;
        this->coeff_de_fond = 0;
        this->distance_bord_droit = 0;
        this->distance_bord_gauche = 0;
        this->hauteur_bord_droit = 0;
        this->hauteur_bord_gauche = 0;
        this->temps_acquisition = 30;};

    double debit();
};


#endif
