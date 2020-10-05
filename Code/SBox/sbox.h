//
// Created by Yevhenii on 10/4/2020.
//

#ifndef SBOX_SBOX_H
#define SBOX_SBOX_H

#include <vector>
#include "malloc.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

int contains(std::vector<int> container, const int *f, int func_size);

int *down(int *func, int size);

void IS(int *func, int size);

int KP(const int *func, int size, int alpha);

int Hamming_Weight(int *f, int size);

int criterion_of_degree(int *func, int size, int k);

int *AC(int *func, int size);

char *to_ANF(const int *func, int size);

int myPow(int num, int pow);

int isBentFunction(int *func, int size);

int *gradient_descent(int *func, int func_size);

int *int_to_binary(int decimal, int rank);

int *array_copy(int *array, int size);

int *up(int *func, int size);

int log2int(int n);

int d(int *f, int *g, int size);

int vector_criterion(int *func, int size, int k);

int *GF(int n);

int *reverse(int *f, int func_size);

int deg(char *anf);

int *WHT(int *func, int func_size);

int mod(int num, int mod);

int NL(int *f, int size);

int isBalanced(int *func, int size);

int abs_max(int *arr, int size);

int *getFunctions(int *func, int size, int count);

int isEqual(int *f1, int *f2, int size);

int *IS(int func, int size);

int L(int *w, int *x, int size);

int *to_arr(std::vector<int> vector);

#endif //SBOX_SBOX_H
