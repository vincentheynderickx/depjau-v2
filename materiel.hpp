#ifndef __MATERIEL_H
#define __MATERIEL_H

#include <tuple>
#include <string>
#include <vector>
#include "pugixml.hpp"

using namespace std;

class Debitmetre{
    public:
        int n_intervalles; // nombre de sections de droite qui définissent l'étalonnage
        vector<double> intervalles; // les n-1 valeurs délimitant les intervalles
        vector<double> pentes; // pente de chaque section de droite
        vector<double> ordonnees; // ordonnée à l'origine de chaque section de droite
        string nom;
        Debitmetre();
        Debitmetre(string nom_fichier);
        Debitmetre(string nom, int n_intervalles, vector<double> intervalles, vector<double> pentes, vector<double> ordonnees);
        Debitmetre(std::string& name);
        double calcul_vitesse(double n) const ;
        void enregistrer_xml(string nom_fichier);
};

void ajout_xml_debitmetre(std::string filename, const std::vector<double>& intervalles, const std::vector<double>& pentes, const std::vector<double>& ordonnees);
void creer_nouveau_debitmetre(std::string name);

#endif
