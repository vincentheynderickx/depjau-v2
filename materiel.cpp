#include "materiel.hpp"
#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include "pugixml.hpp"

using namespace std;


Debitmetre::Debitmetre(){
    this->nom = "Helice par défaut";
    this->n_intervalles = 3;
    this->intervalles = {2.94, 10.56};
    this->pentes = {0.0614, 0.0546, 0.0528};
    this->ordonnees = {0.015, 0.035, 0.054};
}


Debitmetre::Debitmetre(string nom, int n_intervalles, vector<double> intervalles, vector<double> pentes, vector<double> ordonnees){
    this->nom = nom;
    this->n_intervalles = n_intervalles;
    this->intervalles = intervalles;
    this->pentes = pentes;
    this->ordonnees = ordonnees;
}


double Debitmetre::calcul_vitesse(double n){
    int i = 0;
    while (i < this->n_intervalles - 1 && n > this->intervalles[i]){
        i++;
    }
    return n * this->pentes[i] + this->ordonnees[i];
}

Debitmetre::Debitmetre(string& name) //lis le fichier xml de l'hélice name et crée l'instance de classe
{
    vector<double> intervalles;
    vector<double> pentes;
    vector<double> ordonnees;
    string filename = name + ".xml";

    pugi::xml_document doc;
    doc.load_file(filename.c_str());

    pugi::xml_node root = doc.child("Debimetre");

    for (pugi::xml_node node = root.child("intervalles").child("intervalle"); node; node = node.next_sibling("intervalle"))
    {
        intervalles.push_back(node.text().as_double());
    }

    for (pugi::xml_node node = root.child("pentes").child("pente"); node; node = node.next_sibling("pente"))
    {
        pentes.push_back(node.text().as_double());
    }

    for (pugi::xml_node node = root.child("ordonnees").child("ordonnee"); node; node = node.next_sibling("ordonnee"))
    {
        ordonnees.push_back(node.text().as_double());
    }

    this->nom = name;
    this->intervalles = intervalles;
    this->pentes = pentes;
    this->ordonnees = ordonnees;
    this->n_intervalles = intervalles.size() + 1;
}

void ajout_xml_debitmetre(string filename, const vector<double>& intervalles, const vector<double>& pentes, const vector<double>& ordonnees)
{ // créer le fichier xml de l'hélice
    pugi::xml_document doc;

    pugi::xml_node root = doc.append_child("Debimetre");

    pugi::xml_node intervallesNode = root.append_child("intervalles");
    for (const auto& intervalle : intervalles)
    {
        intervallesNode.append_child("intervalle").text() = intervalle;
    }

    pugi::xml_node pentesNode = root.append_child("pentes");
    for (const auto& pente : pentes)
    {
        pentesNode.append_child("pente").text() = pente;
    }

    pugi::xml_node ordonneesNode = root.append_child("ordonnees");
    for (const auto& ordonnee : ordonnees)
    {
        ordonneesNode.append_child("ordonnee").text() = ordonnee;
    }

    doc.save_file(filename.c_str());
}


void creer_nouveau_debitmetre(string name)
{ // demande les coefficients des équations de l'hélice et crée les listes associées puis appelle ajout_xml_debitmetre
    vector<double> intervalles;
    vector<double> pentes;
    vector<double> ordonnees;
    string filename = name + ".xml";

    cout << "Entrez 1 pour ajouter une zone de fonctionnement, 2 pour terminer : ";
    int choix;
    cin >> choix;
    double intervalle;
    double pente;
    double ordonnee;
    while (true){
        if (choix == 2) break;

        cout << "Entrez la valeur de la limite de la zone de fonctionnement : ";
        cin >> intervalle;
        intervalles.push_back(intervalle);

        cout << "Entrez la pente de la zone de fonctionnement : ";
        cin >> pente;
        pentes.push_back(pente);

        cout << "Entrez l'ordonnée à l'origine de la zone de fonctionnement : ";
        cin >> ordonnee;
        ordonnees.push_back(ordonnee);

        cout << "Entrez 1 pour ajouter une zone de fonctionnement, 2 pour terminer : ";
        cin >> choix;
    }
    ajout_xml_debitmetre(filename, intervalles, pentes, ordonnees);
}
