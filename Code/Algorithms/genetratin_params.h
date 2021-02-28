//
// Created by Yevhenii on 2/28/2021.
//

#ifndef INFO_GENETRATIN_PARAMS_H
#define INFO_GENETRATIN_PARAMS_H
#include "cost.h"

struct GeneratingParams {
    // cost params
    CostParams costParams;
    // multithreading
    int count_of_threads;
    int target_sbox_generated;
    // target properties
    int target_nonlinearity;
    int target_algebraic_immunity;
    int target_autocorrelation;
    // simulating_annealing
    int initial_temperature;
    int MUL; // оличество внешних циклов
    int MIL; // количество внутренних циклов
    double solidification_coefficient; // коэфициент застывания
};
#endif //INFO_GENETRATIN_PARAMS_H
