#include <iostream>
#include <cmath>
#include <bitset>
#include <conio.h>
#include "fstream"
#include "stdlib.h"

int *int_to_binary(int decimal, int rank);

int *SBoxDecimalToBinary(int *sbox, int n);

int LAT(int *sbox, int n);

int SBoxNL(int *sbox, int n);

int read(const char *file_name, int *sbox, int n);

int *GF(int n);

int main(int argc, char **argv) {
    setlocale(LC_ALL, "Rus");
    using namespace std;
    int n = 8;
    int size = (int) pow(2, n);
    int *sbox = (int *) malloc(sizeof(int) * size);
    read("file.txt", sbox, 8);
    for (int i = 0; i < size; ++i) {
        printf("%02X ", sbox[i]);
    }
    cout << endl << "Нелинейность: " << SBoxNL(SBoxDecimalToBinary(sbox, n), n);
    return 0;
}

int SBoxNL(int *sbox, int n) {
    return pow(2, n - 1) - LAT(sbox, n);
}

int LAT(int *sbox, int n) {
    int size = pow(2, n);
    int *gf = GF(n);
    int *table = (int *) malloc(sizeof(int) * (size - 1) * (size - 1));
    int norm = pow(2, n - 1);
    int max = 0;
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - 1; ++j) {
            int result = 0;
            for (int k = 0; k < size; ++k) {
                int X = 0, Y = 0;
                for (int l = 0; l < n; ++l) {
                    X += *(gf + (k * n + l)) * (*(gf + ((j + 1) * n + l)));
                }
                for (int l = 0; l < n; ++l) {
                    Y += *(sbox + (size * l + k)) * (*(gf + ((i + 1) * n + l)));
                }
                if (X % 2 == Y % 2) {
                    result++;
                }
            }
            *(table + i * (size - 1) + j) = result;
            if (abs(result - norm) > max) {
                max = abs(result - norm);
            }
        }
    }
    free(gf);
    free(table);
    return max;
}

int *GF(int n) {
    int *arr = (int *) malloc(n * pow(2, n) * sizeof(int));
    for (int i = 0; i < pow(2, n); ++i) {
        for (int j = n - 1; j >= 0; --j) {
            *(arr + i * n + j) = (i >> (n - j - 1)) & 1u;
        }
    }
    return arr;
}

int read(const char *file_name, int *sbox, int n) {
    using namespace std;
    int size = pow(2, n);
    string line;
    ifstream in(file_name);
    if (in.is_open()) {
        while (getline(in, line)) {
            //cout << line;
            for (int i = 0; i < size; i++) {
                sscanf(line.substr(3 * i, 2).c_str(), "%x", sbox + i);
            }
        }
        in.close();
    }
    return 0;
}

int *SBoxDecimalToBinary(int *sbox, int n) {
    int size = (int) pow(2, n);
    int *result = (int *) malloc(size * n * sizeof(int));
    for (int i = 0; i < size; ++i) {
        int *bin = int_to_binary(sbox[i], n);
        for (int j = 0; j < n; ++j) {
            result[j * size + i] = bin[n - j - 1];
        }
        free(bin);
    }
    return result;
}

int *int_to_binary(int decimal, int rank) {
    int *res = (int *) malloc(sizeof(int) * rank);
    for (int j = 0; j < rank; ++j) {
        res[rank - 1 - j] = decimal >> j & 1u;
    }
    return res;
}