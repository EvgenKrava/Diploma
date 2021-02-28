//
// Created by Yevhenii on 2/28/2021.
//

#ifndef INFO_SIMULATING_ANNEALING_H
#define INFO_SIMULATING_ANNEALING_H

#include <cmath>
#include <cstdio>
#include "genetratin_params.h"
#include "props.h"
#include "pthread.h"

struct PthreadParams {
    const int *sbox_b;
    int n;
    GeneratingParams generatingParams;
    int *target_sbox_b;
};

int *simulating_annealing(const int *sbox_b, int n, GeneratingParams generatingParams);

#endif //INFO_SIMULATING_ANNEALING_H
