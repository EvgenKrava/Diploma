//
// Created by Yevhenii on 10/4/2020.
//
#include <chrono>
#include <random>
#include "sbox.h"

int *generate_descendant(int *sbox, int size, int count);

/**
 * @param decimalFunc decimal numbers
 * @param size function size
 * @param count
 * @return count functions in binary type
 */
int *getFunctions(int *decimalFunc, int size, int count) {
    int *result = (int *) malloc(size * count * sizeof(int));
    for (int i = 0; i < size; ++i) {
        int *bin = int_to_binary(decimalFunc[i], count);
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
    for (int i = 0; i < pow(2, size); ++i) {
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
    return tmp;
}

char *to_ANF(int *func, int size) {
    int *f = reverse(func, size);
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

int *read_from_file(const char *file_name, int size) {
    FILE *fp = fopen(file_name, "r");
    if (fp == nullptr) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    int *res = (int *) malloc(sizeof(int) * size);
    int ch;
    int i = 0;
    while ((ch = fgetc(fp)) != EOF && i < size) {
        *(res + i++) = ch - 48;
    }
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
        res[i] = pow(-1, f[i]);
    }
    return res;
}

int log2int(int n) {
    for (int i = 0; i < n; ++i) {
        if (pow(2, i) == n) return i;
    }
    return -1;
}

int *getTable(int n) {
    int *arr = (int *) malloc(n * pow(2, n) * sizeof(int));
    for (int i = 0; i < pow(2, n); ++i) {
        for (int j = n - 1; j >= 0; --j) {
            *(arr + i * n + j) = (i >> (n - j - 1)) & 1u;
        }
    }
    return arr;
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

void printTableUsingArgs(int *args, int n) {
    int size = pow(2, n);
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
    int *wht = WHT(f, size);
    int res = (size - abs_max(wht, size)) / 2;
    free(wht);
    return res;
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
            res += pow(-1, mod(func[j] + r, 2));
        }
        result[i] = res;
    }
    free(t);
    free(w);
    return result;
}

int mod(int num, int mod) {
    return num % 2 < 0 ? (num % 2) + mod : num % 2;
}

void balance(int *func, int size) {
    int h = Hamming_Weight(func, size);
    int *wht = WHT(func, size);
    for (int i = 0; i < size; ++i) {
        if ((!wht[i] || abs(wht[i]) == 2) && h > size / 2 && func[i] == 1) {
            func[i] = 0;
        } else if ((!wht[i] || abs(wht[i]) == 2) && h < size / 2 && func[i] == 0) {
            func[i] = 1;
        }
        h = Hamming_Weight(func, size);
        if (h == size / 2) break;
    }
    free(wht);
}

int *AC(int *func, int size) {
    int n = log2int(size);
    int *fs = ptt(func, size);
    fs = reverse(fs, size);
    int *ac = (int *) malloc(size * sizeof(int));
    ac[0] = pow(2, n);
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
    return Hamming_Weight(func, size) == pow(2, (log2int(size) - 1));
}

int pow(int a, int b) {
    int res = 1;
    for (int i = 0; i < b; ++i) {
        res *= a;
    }
    return res;
}

int isBentFunction(int *func, int size) {
    int n = log2int(size);
    if (n % 2 != 0) {
        return -1;
    }
    char *anf = to_ANF(func, size);
    //printf("%s\n", anf);
    if (deg(anf) > (size / 2)) {
        free(anf);
        return 0;
    }
    free(anf);
    int hw = Hamming_Weight(func, size);
    int hw1 = pow(2, n - 1) + pow(2, n / 2 - 1);
    int hw2 = pow(2, n - 1) - pow(2, n / 2 - 1);
    if (hw != hw1 && hw != hw2) {
        return 0;
    }
    if (NL(func, size) != hw2) {
        return 0;
    }
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

int autocorrelation(int *func, int size) {
    int max = 0;
    int *ac = AC(func, size);
    for (int i = 1; i < size; ++i) {
        if (ac[i] > max) {
            max = ac[i];
        }
    }
    return max;
}

int gradient_up(int *func, int size) {
    std::vector<int> w_plus;
    std::vector<int> w_minus;
    std::vector<int> is;
    w_minus.clear();
    w_plus.clear();
    is.clear();
    int *wht = WHT(func, size);
    int wht_max = abs_max(wht, size);
    for (int i = 0; i < size; ++i) {
        if (wht[i] > 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            w_plus.push_back(i);
        }
        if (wht[i] < 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            w_minus.push_back(i);
        }
    }
    for (int i = 0; i < size; ++i) {
        if ((std::find(w_plus.begin(), w_plus.end(), i) == w_plus.end())
            &&
            (std::find(w_minus.begin(), w_minus.end(), i) != w_minus.end())) {
            is.push_back(i);
        }
    }
    for (int i : is) {
        func[i] = func[i] ? 0 : 1;
    }
    return 0;
}

int gradient_descent(int *func, int size) {
    std::vector<int> w_plus;
    std::vector<int> w_minus;
    std::vector<int> is;
    w_minus.clear();
    w_plus.clear();
    is.clear();
    int *wht = WHT(func, size);
    int wht_max = abs_max(wht, size);
    for (int i = 0; i < size; ++i) {
        if (wht[i] > 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            w_plus.push_back(i);
        }
        if (wht[i] < 0 && (abs(wht[i]) == wht_max || abs(wht[i]) == wht_max - 2)) {
            w_minus.push_back(i);
        }
    }
    for (int i = 0; i < size; ++i) {
        if ((std::find(w_plus.begin(), w_plus.end(), i) != w_plus.end())
            ||
            (std::find(w_minus.begin(), w_minus.end(), i) != w_minus.end())) {
            is.push_back(i);
        }
    }
    for (int i : is) {
        func[i] = func[i] ? 0 : 1;
    }
    free(wht);
    return 0;
}

int N(int *func, int n) {
    int *wh = WHT(func, pow(2, n));
    int max = abs_max(wh, pow(2, n));
    free(wh);
    return (pow(2, n) - max) / 2;
}

int isBalancedSBox(int *sbox, int size, int count) {
    int result = 0;
    for (int i = 0; i < count; ++i) {
        result += isBalanced(sbox + i * size, size);
    }
    return result == count;
}


int SboxNL(int *sbox, int size, int count) {
    int min = NL(sbox, size);
    for (int i = 0; i < count; ++i) {
        int nl = NL(sbox + size * i, size);
        if (nl < min) {
            min = nl;
        }
    }
    return min;
}

int SBoxAC(int *sbox, int size, int count) {
    int max = autocorrelation(sbox, size);
    for (int i = 1; i < count; ++i) {
        int ac = autocorrelation(sbox + size * i, size);
        if (ac > max) {
            max = ac;
        }
    }
    return max;
}

int *simulated_annealing(int *sbox, int size, int count, int requiredNL, int requiredAC) {
    double d;
    int MIL = 100;
    double a = 0.7;
    double T = 100000;
    std::mt19937 engine; // mt19937 как один из вариантов
    int *S = (int *) malloc(size * count * sizeof(int));
    for (int i = 0; i < size * count; ++i) {
        S[i] = sbox[i];
    }
    while ((SboxNL(S, size, count) < requiredNL || SBoxAC(S, size, count) > requiredAC)) {
        for (int i = 0; i < MIL; ++i) {
            int *Y = generate_descendant(S, size, count);
            int costY = cost_4_11(Y, size, count);
            int costS = cost_4_11(sbox, size, count);
            d = costY - costS;
            //printf("%d  %d\n", costS, costY);
            if (d < 0) {
                //printf("change\n");
                for (int j = 0; j < size * count; ++j) {
                    S[j] = Y[j];
                }
            } else {
                double u = (double) engine() / engine.max();
                //printf("%f\n", u);
                double t = (double) d / T;
                if (u < exp(-t)) {
                    for (int j = 0; j < size * count; ++j) {
                        S[j] = Y[j];
                    }
                }
            }
            T = T * a;
            free(Y);
        }
        printf("NL = %d\n", SboxNL(S, size, count));
        printf("AC = %d\n", SBoxAC(S, size, count));
    }
    return S;
}

int *generate_descendant(int *sbox, int size, int count) {
    int *Y = (int *) malloc(sizeof(int) * size * count);
    std::mt19937 engine;
    static int a = 0;
    int range = engine() % size;
    range = range >= 0 ? range : range + size;
    a = (a + 1) % size;
    int b;
    do {
        b = (a + range) % size;
    } while (b == a);
    //printf("%d %d\n", a, b);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < count; ++j) {
            if (i == a) {
                *(Y + j * size + i) = *(sbox + j * size + b);
            } else *(Y + j * size + i) = *(sbox + j * size + (i == b ? a : i));
        }
    }
    return Y;
}

int *generate_sbox(int n, int m) {
    int size = pow(2, n);
    int *dec = (int *) malloc(sizeof(int) * size);
    srand(time(NULL));
    for (int i = 0; i < size;) {
        dec[i] = rand() % pow(2, m);
        int contains = 0;
        for (int j = 0; j < i; ++j) {
            if (dec[i] == dec[j]) {
                contains = 1;
                break;
            }
        }
        if (!contains) {
            i++;
        }
    }
    int *sb = getFunctions(dec, size, m);
    free(dec);
    return sb;
}

int *generate_function(int size) {
    int *f = (int *) malloc(sizeof(int) * size);
    srand(time(nullptr));
    for (int i = 0; i < size; ++i) {
        f[i] = rand() % 2;
    }
    return f;
}

int cost_4_11(int *sbox, int size, int count) {
    int res = 0;
    int R = 1;
    for (int i = 0; i < count; ++i) {
        int re = 0;
        int *wh = WHT((sbox + size * count), size);
        for (int j = 0; j < size; ++j) {
            int n = log2int(size) / 2;
            int k = abs(abs(wh[j]) - pow(2, n));
            re += pow(k, R);
        }
        free(wh);
        res += re;
    }
    return res;
}
