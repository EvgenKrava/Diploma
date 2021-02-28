//
// Created by Yevhenii on 2/28/2021.
//

#ifndef INFO_PROPS_H
#define INFO_PROPS_H

#include <cstdio>

int *SBoxDecimalToBinary(const int *sbox, int n);

int *SBoxBinaryToDecimal(const int *sbox_b, int n);

int *int_to_binary(int decimal, int rank);

int NL(const int *sbox_b, int n);

int LAT(const int *sbox_b, int n);

int AC(const int *sbox_b, int n);

int delta_uniformity(const int *sbox_b, int n);

int *array_copy(const int *array, int size);

int min_degree(int *sbox, int n);

extern int input_values[256][8];
#endif //INFO_PROPS_H
