//
// Created by Yevhenii on 2/28/2021.
//

#include <cmath>
#include <cstring>
#include <dshow.h>
#include <cstdio>
#include <cstring>
#include <string>
#include "props.h"

int *ac(const int *func, int size);

char *to_ANF(int *func, int size);

//Algebraic degree
int deg(char *anf) {
    int max = 0;
    int tmp = 0;
    for (int i = 0; i < strlen(anf); ++i) {
        if (anf[i] == 'x') {
            tmp++;
        }
        if (anf[i] == '+' || i == strlen(anf) - 1) {
            if (tmp > max) max = tmp;
            tmp = 0;
        }
    }
    return max;
}

//Polar truth table
int *ptt(const int *f, int size);

void reverse(int *f, int func_size);

int log2int(int n);

int autocorrelation(const int *func, int size);

int *GF(int n);

int nonlinearity(const int *f, int size);

int *WHT(const int *func, int func_size);

int mod(int num, int mod);

int abs_max(int *arr, int size);

int *SBoxBinaryToDecimal(const int *sbox_b, int n) {
    int size = (int) pow(2, n);
    int *sbox = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        *(sbox + i) = 0;
        for (int j = 0; j < n; ++j) {
            *(sbox + i) += *(sbox_b + j * size + i) * (int) pow(2, j);
        }
    }
    return sbox;
}

int *SBoxDecimalToBinary(const int *sbox, int n) {
    int size = (int) pow(2, n);
    int *sbox_b = (int *) malloc(size * n * sizeof(int));
    for (int i = 0; i < size; ++i) {
        int *bin = int_to_binary(sbox[i], n);
        for (int j = 0; j < n; ++j) {
            sbox_b[j * size + i] = bin[n - j - 1];
        }
        free(bin);
    }
    return sbox_b;
}

int NL(const int *sbox, int n) {
    int size = (int) pow(2, n);
    int min = nonlinearity(sbox, size);
    for (int i = 1; i < size; ++i) {
        int *binary_i = int_to_binary(i, n);
        int com[size];
        for (int j = 0; j < size; ++j) {
            com[j] = 0;
        }
        for (int j = 0; j < n; ++j) {
            if (binary_i[j]) {
                for (int k = 0; k < size; ++k) {
                    com[k] = (com[k] + *(sbox + j * size + k)) % 2;
                }
            }
        }
        int nl = nonlinearity(com, size);
        if (nl < min) {
            min = nl;
        }
        free(binary_i);
    }
    return min - (int) pow(2, n - 1);
}

int nonlinearity(const int *f, int size) {
    int *wh = WHT(f, size);
    int max = abs_max(wh, size);
    free(wh);
    return (int) pow(2, log2int(size)) - max / 2;
}

int *WHT(const int *func, int func_size) {
    int n = log2int(func_size);
    int *t = GF(n);
    int *result = (int *) malloc(func_size * sizeof(int));
    int *w = (int *) malloc(n * sizeof(int));
    for (int i = 0; i < func_size; ++i) {
        for (int j = 0; j < n; ++j) {
            w[j] = t[i * n + j];
        }
        int res = 0;
        for (int j = 0; j < func_size; ++j) {
            int r = 0;
            for (int k = 0; k < n; ++k) {
                r += mod(w[k] * t[j * n + k], 2);
            }
            res += (int) pow(-1, mod(func[j] + r, 2));
        }
        result[i] = res;
    }
    free(t);
    free(w);
    return result;
}

int mod(int num, int mod) {
    return num % mod < 0 ? (num % mod) + mod : num % mod;
}

int abs_max(int *arr, int size) {
    int max = 0;
    for (int i = 0; i < size; ++i) {
        int tmp = abs(arr[i]);
        if (tmp > max) max = tmp;
    }
    return max;
}

int *GF(int n) {
    int *arr = (int *) malloc(n * pow(2, n) * sizeof(int));
    for (int i = 0; i < pow(2, n); ++i) {
        for (int j = n - 1; j >= 0; --j) {
            *(arr + i * n + j) = (i >> (n - j - 1)) & 1;
        }
    }
    return arr;
}

int LAT(const int *sbox_b, int n) {
    int size = (int) pow(2, n);
    int *gf = GF(n);
    int *table = (int *) malloc(sizeof(int) * (size - 1) * (size - 1));
    int norm = (int) pow(2, n - 1);
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
                    Y += *(sbox_b + (size * l + k)) * (*(gf + ((i + 1) * n + l)));
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

int log2int(int n) {
    for (int i = 0; i < n; ++i) {
        if (pow(2, i) == n) return i;
    }
    return -1;
}

/**
 *
 * @param decimal
 * @param rank count of symbols
 * @return decimal number in binary
 */
int *int_to_binary(int decimal, int rank) {
    int *res = (int *) malloc(sizeof(int) * rank);
    for (int j = 0; j < rank; ++j) {
        res[rank - 1 - j] = decimal >> j & 1;
    }
    return res;
}

int autocorrelation(const int *func, int size) {
    int max = 0;
    int *acp = ac(func, size);
    for (int i = 1; i < size; ++i) {
        if (acp[i] > max) {
            max = acp[i];
        }
    }
    return max;
}

int *ac(const int *func, int size) {
    int n = log2int(size);
    int *fs = ptt(func, size);
    reverse(fs, size);
    int *ac = (int *) malloc(size * sizeof(int));
    ac[0] = (int) pow(2, n);
    for (int a = 1; a < size; ++a) {
        int result = 0;
        for (int i = 0; i < size; ++i) {
            result += fs[i] * (fs[i ^ a]);
        }
        ac[a] = result;
    }
    free(fs);
    return ac;
}

int AC(const int *sbox_b, int n) {
    int size = (int) pow(2, n);
    int max = autocorrelation(sbox_b, size);
    for (int i = 1; i < size; ++i) {
        int *binary_i = int_to_binary(i, n);
        int com[size];
        for (int j = 0; j < size; ++j) {
            com[j] = 0;
        }
        for (int j = 0; j < n; ++j) {
            if (binary_i[j]) {
                for (int k = 0; k < size; ++k) {
                    com[k] = (com[k] + *(sbox_b + j * size + k)) % 2;
                }
            }
        }
        int ac = autocorrelation(com, size);
        if (ac > max) {
            max = ac;
        }
        free(binary_i);
    }
    return max;
}

//Polar truth table
int *ptt(const int *f, int size) {
    int *res = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        res[i] = (int) pow(-1, f[i]);
    }
    return res;
}

void reverse(int *f, int func_size) {
    int *tmp = array_copy(f, func_size);
    for (int i = 0; i < func_size; ++i) {
        f[func_size - i - 1] = tmp[i];
    }
    free(tmp);
}

int *array_copy(const int *array, int size) {
    int *copy = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        copy[i] = array[i];
    }
    return copy;
}

int delta_uniformity(const int *sbox, int n) {
    int size = (int) pow(2, n);
    int max_res = 0;
    int res = 0;
    for (int a = 1; a < size; a++) {
        for (int b = 0; b < size; b++) {
            res = 0;
            for (int x = 0; x < size; x++)
                if ((sbox[x] ^ sbox[x ^ a]) == b) {
                    res++;
                }
            if (res > max_res)max_res = res;
        }

    }
    return max_res;
}


char *to_ANF(int *func, int size) {
    int *f = array_copy(func, size);
    reverse(f, size);
    int n = log2int(size);
    int length = 0;
    int *table = GF(n);
    int *matrix = (int *) malloc(sizeof(int) * size * size);
    for (int i = 0; i < size; ++i) {
        *(matrix + i) = *(f + size - 1 - i);
    }
    for (int i = 1; i < size; ++i) {
        for (int j = 0; j < size - i; ++j) {
            *(matrix + i * size + j) = (*(matrix + size * (i - 1) + j) + *(matrix + size * (i - 1) + (j + 1))) % 2;
        }
    }
    int *coefs = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        coefs[i] = *(matrix + i * size);
        if (coefs[i]) {
            for (int j = 0; j < n; ++j) {
                if (*(table + i * n + j)) {
                    length += 2;
                }
            }
            length++;
        }
    }
    char *result = (char *) malloc(sizeof(char) * length);
    sprintf(result, "");
    for (int i = 0; i < size; ++i) {
        coefs[i] = *(matrix + i * size);
        if (i == 0 && coefs[i] == 1) {
            sprintf(result, "1");
        }
        if (coefs[i]) {
            for (int j = 0; j < n; ++j) {
                if (*(table + i * n + j)) {
                    sprintf(result, "%sx%d", result, n - j);
                }
            }
            if (i != 0 && i != length - 1) {
                sprintf(result, "%s+", result);
            }
        }
    }
    result[length - 1] = '\0';
    free(coefs);
    free(table);
    free(matrix);
    free(f);
    return result;
}

int min_degree(int *sbox, int n) {
    int size = (int) pow(2, n);
    char *anf = to_ANF(sbox, size);
    int min = deg(anf);
    int *com = (int *) malloc(size * sizeof(int));
    for (int i = 1; i < size; ++i) {
        int *binary_i = int_to_binary(i, n);
        for (int j = 0; j < size; ++j) {
            com[j] = 0;
        }
        for (int j = 0; j < n; ++j) {
            if (binary_i[j]) {
                for (int k = 0; k < size; ++k) {
                    com[k] = (com[k] + *(sbox + j * size + k)) % 2;
                }
            }
        }
        anf = to_ANF(com, size);
        //printf("ANF: %s\n********************\n", anf);
        int degree = deg(anf);
        if (degree < min) {
            min = degree;
        }
        free(binary_i);
    }
    free(com);
    return min;
}

