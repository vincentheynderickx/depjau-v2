#include <cmath>
#include <vector>
#include <algorithm>
#include "calcul_debit.hpp"
#include <iostream>


double debit_lineique (std::vector<double> vitesses , std::vector<double> hauteurs, double profondeur, double coeff_fond) {
    double debit_lin = 0;
    int n = vitesses.size();
    if (n == 0){
        return 0;
    }
    for (int i = 0; i < n - 1; i++) {
        debit_lin += (vitesses[i+1] + vitesses[i]) * fabs(hauteurs[i+1] - hauteurs[i])/2;
    }
    debit_lin += coeff_fond * vitesses[n-1] * (profondeur - hauteurs[n-1]);
    std::cout << "debit_lineique : " << debit_lin << std::endl;
    return debit_lin;
}

double debit_sous_section_general (double debit_lin_g , double debit_lin_d , double b_g , double b_d) {
    double debit_ss = (debit_lin_g + debit_lin_d) * fabs(b_d - b_g) / 2;
    std::cout << "debit_ss : " << debit_ss << std::endl;
    return debit_ss;
}

double debit_sous_section_bord (double debit_lin , double b_d , double b_g , double h_g , double h_d , double Kb) {
    double h_max = std::max(h_g, h_d);
    if (h_max == 0){
        return 0;
    }
    double debit_ss = debit_lin * Kb * fabs(b_d - b_g) * (h_g + h_d) / (2 * h_max);
    std::cout << "debit_ss_bord : " << debit_ss << std::endl;
    return debit_ss;

}

//éventuellement séparer en deux fonctions : une pour le bord gauche et une pour le bord droit

double debit_total (std::vector<double> debits_ss) {
    double debit_tot = 0;
    for (int i = 0; i < debits_ss.size(); i++){
        debit_tot += debits_ss[i];
    }
    return debit_tot;
}
