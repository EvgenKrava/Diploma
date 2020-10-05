//
// Created by Yevhenii on 10/4/2020.
//

#include <algorithm>
#include "sbox.h"

/**
 * @param func decimal numbers
 * @param size function size
 * @param count
 * @return count functions in binary type
 */
int *getFunctions(int *func, int size, int count) {
    int *result = (int *) malloc(size * count * sizeof(int));
    for (int i = 0; i < size; ++i) {
        int *bin = int_to_binary(func[i], count);
        for (int j = 0; j < count; ++j) {
            result[j * size + i] = bin[count - j - 1];
        }
        free(bin);
    }
    return result;
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
        res[rank - 1 - j] = decimal >> j & 1u;
    }
    return res;
}

/**
 *
 * @param array
 * @param size of array
 * @return copy of array
 */
int *array_copy(int *array, int size) {
    int *copy = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        copy[i] = array[i];
    }
    return copy;
}

/**
 * Vector alpha propagation criterion
 * @param func function
 * @param size size of function
 * @param alpha
 * @return criterion 1 if true else 0
 */
int KP(const int *func, int size, int alpha) {
    int *arr = (int *) malloc(sizeof(int) * size);

    int res = 0;
    for (int i = 0; i < size; ++i) {
        //res += (func[i] + func[i ^ alpha]) % 2;
        arr[i] = (func[size - i - 1] + func[size - 1 - (i ^ alpha)]) % 2;
    }
    //printf("KP(%d) = %d\n", alpha, res);
    return isBalanced(arr, size);
}

int criterion_of_degree(int *func, int size, int k) {
    int *gf = GF(size);
    for (int i = 0; i < myPow(2, size); ++i) {
        int hw = Hamming_Weight(gf + i * size, size);
        if (hw >= 1 && hw <= k) {
            int k = KP(func, size, i);
            printf("KP(%d) = %d\n", i, k);
            if (!k) {
                return 0;
            }
        }
    }
    free(gf);
    return 1;
}

int *reverse(int *f, int func_size) {
    int *tmp = (int *) malloc(func_size * sizeof(int));
    for (int i = 0; i < func_size; ++i) {
        tmp[func_size - i - 1] = f[i];
    }
    free(f);
    return tmp;
}

char *to_ANF(const int *func, int size) {
    int n = log2int(size);
    int length = 0;
    int *table = GF(n);
    int *matrix = (int *) malloc(sizeof(int) * size * size);
    for (int i = 0; i < size; ++i) {
        *(matrix + i) = *(func + size - 1 - i);
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

    char *result = (char *) malloc(sizeof(char) * length - 1);
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
            sprintf(result, "%s+", result);
        }
    }
    result[strlen(result) - 1] = '\0';
    free(coefs);
    free(table);
    free(matrix);
    return result;
}

int *to_binary(char *ANF, int size) {
    int *arr = (int *) malloc(size * sizeof(int));
    int n = log2int(size);
    int *table = GF(n);
    int res = 1;
    for (int i = 0; i < size; ++i) {
        int result = 0;
        for (int j = 0;; ++j) {
            if (j == strlen(ANF) - 1) {
                result += res;
                res = 1;
                break;
            }
            if (ANF[j] == 'x') {
                res *= *(table + (n * i + n - ANF[j + 1] + 48));
            }
            if (ANF[j] == '+' && j != 0) {
                result += res;
                res = 1;
            }
        }
        arr[size - i - 1] = (arr[0] == '1' ? (result + 1) : result) % 2;
    }
    return arr;
}

int *read_from_file(char *file_name, int size) {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    int *res = (int *) malloc(sizeof(*fp));
    int ch;
    int i = 0;
    while ((ch = fgetc(fp)) != EOF)
        *(res + i++) = ch - 48;
    fclose(fp);
    return res;
}

int write_binary_to_file(char *file_name, int *function, int size) {
    FILE *file = fopen(file_name, "w");
    for (int i = 0; i < size; ++i) {
        fprintf(file, "%d", function[i]);
    }
    fprintf(file, "\n");
    fclose(file);
    return 0;
}

int write_ANF_to_file(char *file_name, char *function, int size) {
    FILE *file = fopen(file_name, "w");
    for (int i = 0; i < size; ++i) {
        fprintf(file, "%d ", function[i]);
    }
    fprintf(file, "\n");
    fclose(file);
    return 0;
}

int maximum(int *arr, int size) {
    int m = arr[0];
    for (int i = 0; i < size; ++i) {
        if (arr[i] > m) m = arr[i];
    }
    return m;
}

int myPow(int num, int pow) {
    int res = 1;
    for (int i = 0; i < pow; ++i) {
        res *= num;
    }
    return res;
}

int isEqual(int *f1, int *f2, int size) {
    int counter = 0;
    for (int i = 0; i < size; ++i) {
        if (f1[i] == f2[i]) {
            counter++;
        }
    }
    return counter == size;
}

//Polar truth table
int *ptt(int *f, int size) {
    int *res = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        res[i] = myPow(-1, f[i]);
    }
    return res;
}

int log2int(int n) {
    for (int i = 0; i < n; ++i) {
        if (myPow(2, i) == n) return i;
    }
    return -1;
}

int *getTable(int n) {
    int *arr = (int *) malloc(n * myPow(2, n) * sizeof(int));
    for (int i = 0; i < myPow(2, n); ++i) {
        for (int j = n - 1; j >= 0; --j) {
            *(arr + i * n + j) = (i >> (n - j - 1)) & 1u;
        }
    }
    return arr;
}

int *GF(int n) {
    int *arr = (int *) malloc(n * myPow(2, n) * sizeof(int));
    for (int i = 0; i < myPow(2, n); ++i) {
        for (int j = n - 1; j >= 0; --j) {
            *(arr + i * n + j) = (i >> (n - j - 1)) & 1u;
        }
    }
    return arr;
}

void printTableUsingArgs(int *args, int n) {
    int size = myPow(2, n);
    int *arr = GF(n);
    for (int i = n; i > 0; --i) {
        printf("x%d ", i);
    }
    printf(" f(x)\n");
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d  ", *(arr + n * i + j));
        }
        printf(" %d", args[size - i - 1]);
        printf("\n");
    }

}

//Hamming Weight
int Hamming_Weight(int *f, int size) {
    int res = 0;
    for (int i = 0; i < size; ++i) {
        if (f[i]) res++;
    }
    return res;
}

//Hamming distance
int d(int *f, int *g, int size) {
    int res = 0;
    for (int i = 0; i < size; ++i) {
        if (f[i] != g[i]) res++;
    }
    return res;
}

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

int NL(int *f, int size) {
    return (size - abs_max(WHT(f, size), size)) / 2;
}

//Walsh Hadamard Transform
int *WHT(int *func, int func_size) {
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
            res += myPow(-1, mod(func[j] + r, 2));
        }
        result[i] = res;
    }
    free(t);
    free(w);
    return result;
}

int mod(int num, int mod) {
    return num % 2 < 0 ? num + mod : num;
}

int *AC(int *func, int size) {
    int n = log2int(size);
    int *fs = ptt(func, size);
    fs = reverse(fs, size);
    int *ac = (int *) malloc(size * sizeof(int));
    ac[0] = myPow(2, n);
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

int isAffine(int *func, int size) {
    return 0;
}

int abs_max(int *arr, int size) {
    int max = 0;
    for (int i = 0; i < size; ++i) {
        if (abs(arr[i]) > max) max = abs(arr[i]);
    }
    return max;
}

int isBalanced(int *func, int size) {
    return Hamming_Weight(func, size) == myPow(2, (log2int(size) - 1));
}

int isBentFunction(int *func, int size) {
    if (deg(to_ANF(func, size)) > size / 2) return 0;
    int hw = Hamming_Weight(func, size);
    int n = myPow(2, size - 1) - myPow(2, size / 2 - 1);
    if (hw != (myPow(2, size - 1) + myPow(2, size / 2 - 1))
        &&
        hw != n)
        return 0;
    if (NL(func, size) != n) return 0;
    return 1;
}

int L(int *w, int *x, int size) {
    int res = 0;
    for (int i = 0; i < size; ++i) {
        if (w[i]) res += x[i];
    }
    return res % 2;
}

int contains(std::vector<int> container, const int *f, int func_size) {
    for (int i = 0; i < container.size() / func_size; i += func_size) {
        int tmp = 0;
        for (int j = i; j < i + func_size; ++j) {
            if (container[j] == f[j]) {
                tmp++;
            }
        }
        if (tmp == func_size) return 1;
    }
    return 0;
}

void IS(int *func, int size) {
    int n = log2int(size);
    int *ws = GF(n);
    int *xs = GF(n);
    std::vector<int> w_plus;
    std::vector<int> w_minus;
    std::vector<int> is;
    int *wht = WHT(func, size);
    int wht_max = abs_max(wht, size);
    for (int i = 0; i < size; ++i) {
        if (wht[i] > 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            for (int j = i * n; j < i * n + n; ++j) {
                w_plus.push_back(ws[j]);
            }
        } else if (wht[i] < 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            for (int j = i * n; j < i * n + n; ++j) {
                w_minus.push_back(ws[j]);
            }
        }
    }
    printf("W+ = {");
    for (int i = 0; i < w_plus.size(); ++i) {
        printf("%d", w_plus[i]);
        if ((i + 1) % n == 0) {
            printf(", ");
        }
    }
    printf("}\n");
    printf("W- = {");
    for (int i = 0; i < w_minus.size(); ++i) {
        printf("%d", w_minus[i]);
        if ((i + 1) % n == 0) {
            printf(", ");
        }
    }
    printf("}\n");
    free(ws);
    free(wht);
}

int *to_arr(std::vector<int> vector) {
    int *arr = (int *) malloc(sizeof(int) * vector.size());
    for (int i = 0; i < vector.size(); ++i) {
        arr[i] = vector[i];
    }
    return arr;
}

int *up(int *func, int size) {
    std::vector<int> w_plus;
    std::vector<int> w_minus;
    int *wht = WHT(func, size);
    int wht_max = abs_max(wht, size);
    for (int i = 0; i < size; ++i) {
        if (wht[i] > 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            w_plus.push_back(i);
        } else if (wht[i] < 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            w_minus.push_back(i);
        }
    }
    if (!w_minus.empty()) {
        for (int i = 0; i < size; ++i) {
            if ((std::find(w_plus.begin(), w_plus.end(), i) == w_plus.end()) &&
                (std::find(w_minus.begin(), w_minus.end(), i) != w_minus.end())) {
                func[i] = (func[i] + 1) % 2;
            }
        }
    }
    return func;
}

int *down(int *func, int size) {
    if (!isBalanced(func, size)){

    }
    std::vector<int> w_plus;
    std::vector<int> w_minus;
    std::vector<int> is;
    int *wht = WHT(func, size);
    int wht_max = abs_max(wht, size);
    for (int i = 0; i < size; ++i) {
        if (wht[i] > 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            w_plus.push_back(i);
        } else if (wht[i] < 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            w_minus.push_back(i);
        }
    }
    if (!w_plus.empty()) {
        for (int i = 0; i < size; ++i) {
            if ((std::find(w_plus.begin(), w_plus.end(), i) != w_plus.end()) &&
                (std::find(w_minus.begin(), w_minus.end(), i) == w_minus.end())) {
                func[i] = (func[i] + 1) % 2;
            }
        }
    }
    return func;
}