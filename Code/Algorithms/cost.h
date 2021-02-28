//
// Created by Yevhenii on 2/28/2021.
//

#ifndef INFO_COST_H
#define INFO_COST_H
struct CostParams {
    int R1;
    int R2;
};

int cost(const int *sbox, int n, CostParams costParams);

#endif //INFO_COST_H
