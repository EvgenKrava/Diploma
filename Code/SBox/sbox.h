//
// Created by Yevhenii on 10/4/2020.
//

#ifndef SBOX_SBOX_H
#define SBOX_SBOX_H

#include "malloc.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "chrono"
#include "pthread.h"
#include "algorithm"
#include "vector"
#include "iostream"

struct pair {
    int NL;
    int AC;
    int count;
};

struct parameters {
    int requiredNL;
    int requiredAC;
    int MIL;
    int MUL;
    double solidification_coefficient;
    double initial_temperature;
    int X1;
    int X2;
    int R1;
    int R2;
    int N;
    int thread_count;
    int ready;
    std::vector<pair> pairs;
};

struct pthread_simulating_annealing_args {
    int *sbox;
    int size;
    int count;
    parameters *params;
    int *result;
};

int cost_4_16(int *sbox, int size, int count, int X1, int X2, int R1, int R2);

int *SBoxBinaryToDecimal(int *sbox, int size, int count);

int isBalancedSBox(int *sbox, int size, int count);

int SBoxAC(int *sbox, int size, int count);

int cost_4_11(int *sbox, int size, int count);

int *simulated_annealing(int *sbox, int size, int count, parameters *params);

int *read_from_file(const char *file_name, int size);

int *to_binary(char *ANF, int size);

int *int_to_binary(int decimal, int rank);

int SBoxNL(int *sbox, int size, int count);

int pow(int a, int b);

void balance(int *func, int size);

int contains(std::vector<int> container, const int *f, int func_size);

int gradient_down(int *func, int size);

void IS(int *func, int size);

int KP(const int *func, int size, int alpha);

int Hamming_Weight(int *f, int size);

int criterion_of_degree(int *func, int size, int k);

int *AC(int *func, int size);

char *to_ANF(int *f, int size);

int isBentFunction(int *func, int size);

int gradient_up(int *func, int func_size);

int *array_copy(int *array, int size);

int gradient_descent(int *func, int size);

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

int autocorrelation(int *func, int size);

int abs_max(int *arr, int size);

int *getFunctions(int *decimalFunc, int size, int count);

int isEqual(int *f1, int *f2, int size);

int *IS(int func, int size);

int L(int *w, int *x, int size);

int *to_arr(std::vector<int> vector);

int *generate_function(int size);

int *generate_sbox(int n, int m);

int N(int *func, int n);

void *pthread_simulating_annealing(void *args);

int LAT(int *sbox, int size, int count);

#endif //SBOX_SBOX_H
