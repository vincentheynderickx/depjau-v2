#ifndef __CALCUL_H
#define __CALCUL_H

#include <cmath>
#include <vector>
#include <algorithm>

double debit_lineique (std::vector<double> vitesses , std::vector<double> hauteurs, double profondeur, double coeff_fond);

double debit_sous_section_general (double debit_lin_g , double debit_lin_d , double b_g , double b_d);

double debit_sous_section_bord (double debit_lin , double b_d , double b_g , double h_g , double h_d , double Kb);

double debit_total (std::vector<double> debits_ss);


#endif