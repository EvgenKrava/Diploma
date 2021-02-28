//
// Created by Yevhenii on 10/4/2020.
//
#include <chrono>
#include <random>
#include "sbox.h"


int *generate_descendant(int *sbox, int size, int count);

/**
 * Переводить s-box з бінарного представлення у десяткове
 * @param sbox - s-box у двійковому предсталенні
 * @param size - довжина кожної функції
 * @param count - кількість функцій у блоці
 * @return s-box довжиною size у десятковому представленні
 */
int *SBoxBinaryToDecimal(int *sbox, int size, int count) {
    int *result = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        *(result + i) = 0;
        for (int j = 0; j < count; ++j) {
            *(result + i) += *(sbox + j * size + i) * pow(2, j);
        }
    }
    return result;
}

/**
 * @param sbox decimal numbers
 * @param size function size
 * @param count
 * @return count functions in binary type
 */
int *SBoxDecimalToBinary(int *sbox, int size, int count) {
    int *result = (int *) malloc(size * count * sizeof(int));
    for (int i = 0; i < size; ++i) {
        int *bin = int_to_binary(sbox[i], count);
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

void reverse(int *f, int func_size) {
    int *tmp = array_copy(f, func_size);
    for (int i = 0; i < func_size; ++i) {
        f[func_size - i - 1] = tmp[i];
    }
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
    free(f);
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

int N(int *func, int n) {
    int *wh = WHT(func, pow(2, n));
    int max = abs_max(wh, pow(2, n));
    free(wh);
    return pow(2, n) - max / 2;
}

int NL(int *f, int size) {
    int *wh = WHT(f, size);
    int max = abs_max(wh, size);
    free(wh);
    return pow(2, log2int(size)) - max / 2;
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
    reverse(fs, size);
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
        int tmp = abs(arr[i]);
        if (tmp > max) max = tmp;
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

int isBalancedSBox(int *sbox, int size, int count) {
    int result = 0;
    for (int i = 0; i < count; ++i) {
        result += isBalanced(sbox + i * size, size);
    }
    return result == count;
}


int SBoxNL(int *sbox, int size, int count) {
    int min = NL(sbox, size);
    for (int i = 1; i < size; ++i) {
        int *binary_i = int_to_binary(i, count);
        int com[size];
        for (int j = 0; j < size; ++j) {
            com[j] = 0;
        }
        for (int j = 0; j < count; ++j) {
            if (binary_i[j]) {
                for (int k = 0; k < size; ++k) {
                    com[k] = (com[k] + *(sbox + j * size + k)) % 2;
                }
            }
        }
        int nl = NL(com, size);
        if (nl < min) {
            min = nl;
        }
        free(binary_i);
    }
    return min - pow(2, count - 1);
}

int SBoxAC(int *sbox, int size, int count) {
    int max = autocorrelation(sbox, size);
    for (int i = 1; i < size; ++i) {
        int *binary_i = int_to_binary(i, count);
        int com[size];
        for (int j = 0; j < size; ++j) {
            com[j] = 0;
        }
        for (int j = 0; j < count; ++j) {
            if (binary_i[j]) {
                for (int k = 0; k < size; ++k) {
                    com[k] = (com[k] + *(sbox + j * size + k)) % 2;
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

int *simulated_annealing(int *sbox, int size, int count, SAParams *params) {
    double delta;
    int costS, costY;
    int SNL = 0;
    int SAC = 0;
    int *bestSBox = array_copy(sbox, size * count);
    int bestNL = pow(2, count - 1) - LAT(bestSBox, size, count);
    int MUL = params->MUL;
    int MIL = params->MIL;//количество внутренних циклов
    double a = params->solidification_coefficient;//коэфициент застывания
    double T = params->initial_temperature;//начальная температура
    //std::mt19937 engine; // mt19937 - генератор случайных чисел
    int *S = (int *) malloc(size * count * sizeof(int));
    //записываем изначальный s-box в переменную S
    for (int i = 0; i < size * count; ++i) {
        S[i] = sbox[i];
    }
    //Пока не достигнуты требуемые значания нелинейности и автокареляции
    while ((pow(2, count - 1) - LAT(S, size, count) < params->requiredNL ||
            SBoxAC(S, size, count) > params->requiredAC) && MUL > 0) {
        if (params->ready) {
            return nullptr;
        }
        //Выполняем поиск лучшего потомка указанное количество раз
        for (int i = 0; i < MIL; ++i) {
            int *Y = generate_descendant(S, size, count);//генерация потомка
            costY = cost_4_16(Y, count, params->costParams);//стоимость потомка
            costS = cost_4_16(S, count, params->costParams);//текущая стоимость
            delta = costY - costS;//разница стоимости
            //если потомок лучше текущего претендента
            if (delta < 0) {
                for (int j = 0; j < size * count; ++j) {
                    S[j] = Y[j];
                }
            } else {
                //генерируем случайное число 0..1
                double u = (double) (rand() + pthread_self()) / RAND_MAX;
                double t = (double) delta / T;
                if (u < exp(-t)) {
                    //с некой вероятностью принимаем худшего потомка
                    for (int j = 0; j < size * count; ++j) {
                        S[j] = Y[j];
                    }
                }
            }
            free(Y);
        }//кінець внутрішнього циклу
        T = T * a;//уменьшаем температуру
        SNL = SBoxNL(S, size, count);
        SAC = SBoxAC(S, size, count);
        if (SNL > bestNL) {
            for (int j = 0; j < size * count; ++j) {
                bestSBox[j] = S[j];
            }
            bestNL = SNL;
        }
        params->pairs.push_back({SNL, SAC});
        printf("%5d %5d %9d %9.0f %9.0f\n",
               SNL,
               SAC,
               costS,
               T,
               delta);//вивід даних у консоль

        MUL--;
    }//кінець зовнішннього циклу
    params->ready = 1;
    return bestSBox;
}

int *generate_descendant(int *sbox, int size, int count) {
    int *Y = (int *) malloc(sizeof(int) * size * count);
    int a = (rand() + pthread_self()) % size;
    int b = (rand() + pthread_self()) % size;
    //printf("%d %d\n", a, b);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < count; ++j) {
            if (i == a) {
                *(Y + j * size + i) = *(sbox + j * size + b);
            } else {
                *(Y + j * size + i) = *(sbox + j * size + (i == b ? a : i));
            }
        }
    }
    return Y;
}

int *generate_decimal_sbox(int n, int m) {
    int size = pow(2, n);
    int k = pow(2, m);
    int *dec = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size;) {
        dec[i] = (rand() + pthread_self()) % k;
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
    /*printf("Generated s-box: ");
    for (int i = 0; i < size; ++i) {
        printf("%02X ", dec[i]);
    }
    printf("\n");*/
    return dec;
}


int *generate_binary_sbox(int n, int m) {
    int size = pow(2, n);
    int *dec = (int *) malloc(sizeof(int) * size);
    std::random_device rd;
    std::default_random_engine eng{rd()};
    std::uniform_int_distribution<> dist(0, size);
    for (int i = 0; i < size;) {
        dec[i] = dist(eng) % pow(2, m);
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
    /*printf("Generated s-box: ");
    for (int i = 0; i < size; ++i) {
        printf("%02X ", dec[i]);
    }
    printf("\n");*/
    int *sb = SBoxDecimalToBinary(dec, size, m);
    free(dec);
    return sb;
}

int *generate_function(int size) {
    int *f = (int *) malloc(sizeof(int) * size);
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

int cost_4_16(int *sbox, int n, CostParams costParams) {
    /*int X1 = pow(2, count / 2);
    int R1 = 2;
    int X2 = X1;//pow(2, log2int(size) / 2);
    int R2 = 2;*/
    int size = pow(2, n);
    int res = 0;
    for (int i = 0; i < n; ++i) {
        int *F = WHT(sbox + i * size, size);
        int *r = AC(sbox + i * size, size);
        for (int j = 0; j < size; ++j) {
            res += pow(abs(abs(F[j]) - costParams.X1), costParams.R1);
            res += pow(abs(abs(r[j]) - costParams.X2), costParams.R2);
        }
        free(r);
        free(F);
    }
    return res;
}


void *pthread_simulating_annealing(void *args) {
    auto *args1 = (pthread_simulating_annealing_args *) args;
    int *res = simulated_annealing(args1->sbox, args1->size, args1->count, args1->params);
    if (res) {
        for (int i = 0; i < args1->size * args1->count; ++i) {
            args1->result[i] = res[i];
        }
    }
    //pthread_exit(args1->result);
    return args1;
}

int LAT(int *sbox, int size, int count) {
    int n = log2int(size);
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
    /*for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - 1; ++j) {
            printf("%d  ", *(table + i * (size - 1) + j));
        }
        printf("\n");
    }*/
    free(gf);
    free(table);
    return max;
}


int *cycle_crossover(const int *d_sbox1, const int *d_sbox2, int n) {
    int size = pow(2, n);
    int *child = (int *) malloc(sizeof(int) * size);
    int spos = rand() % size;
    int pos = spos;
    child[spos] = d_sbox1[spos];
    for (int i = 0; i < size; ++i) {
        child[i] = -1;
    }
    do {
        for (int i = 0; i < size; ++i) {
            if (d_sbox1[i] == d_sbox2[pos]) {
                child[i] = d_sbox1[i];
                pos = i;
                break;
            }
        }
    } while (spos != pos);
    for (int i = 0; i < size; ++i) {
        if (child[i] == -1) {
            child[i] = d_sbox2[i];
        }
    }
    return child;
}

int *PMX_crossover(const int *d_sbox1, const int *d_sbox2, int n) {
    int size = pow(2, n);
    int *child = (int *) malloc(sizeof(int) * size);
    int cp1 = rand() % size / 2;
    int cp2 = rand() % size / 2 + size / 2;
    for (int i = 0; i < size; ++i) {
        child[i] = -1;
    }
    for (int i = cp1; i < cp2; ++i) {
        child[i] = d_sbox1[i];
    }
    for (int i = 0; i < cp1; ++i) {
        int contains = 0;
        for (int j = cp1; j < cp2; ++j) {
            if (d_sbox2[i] == child[j]) {
                contains = 1;
                break;
            }
        }
        if (!contains) {
            child[i] = d_sbox2[i];
        }
    }
    for (int i = cp2; i < size; ++i) {
        int contains = 0;
        for (int j = 0; j < cp2; ++j) {
            if (d_sbox2[i] == child[j]) {
                contains = 1;
                break;
            }
        }
        if (!contains) {
            child[i] = d_sbox2[i];
        }
    }
    for (int i = 0; i < size; i++) {
        if (child[i] == -1) {
            for (int j = 0; j < size; ++j) {
                int contains = 0;
                for (int k = 0; k < size; ++k) {
                    if (child[k] == d_sbox1[j]) {
                        contains = 1;
                        break;
                    }
                }
                if (contains) {
                    contains = 0;
                    for (int k = 0; k < size; ++k) {
                        if (child[k] == d_sbox2[j]) {
                            contains = 1;
                            break;
                        }
                    }
                    if (!contains) {
                        child[i] = d_sbox2[j];
                        break;
                    }
                } else {
                    child[i] = d_sbox1[j];
                    break;
                }
            }
        }
    }
    return child;
}

int *GeneticAlgorithm(int n, GAParams *params) {
    using namespace std;
    vector<int *> population;
    int generation = 0;
    for (int i = 0; i < params->popsize; ++i) {
        population.push_back(generate_decimal_sbox(n, n));
    }
    /*for (int i = 0; i < params->popsize; ++i) {
        for (int j = 0; j < pow(2, n); ++j) {
            printf("%02X ", population[i][j]);
        }
        cout << endl;
    }*/
    for (int k = 0; k < params->generationCount; ++k) {
        sort(population.begin(), population.end(), [params, n](int *x, int *y) {

            int *bx = SBoxDecimalToBinary(x, pow(2, n), n);
            int *by = SBoxDecimalToBinary(y, pow(2, n), n);

            int delta = cost_4_16(bx, n, params->costParams) -
                        cost_4_16(by, n, params->costParams);
            free(bx);
            free(by);
            return delta < 0;
        });

        cout << "Generation: " << generation << "\t";
        int bestNL = SBoxNL(SBoxDecimalToBinary(population[0], pow(2, n), n), pow(2, n), n);
        cout << "Best NL: " << bestNL << endl;
        population.shrink_to_fit();
        if (bestNL >= params->requiredNL) {
            break;
        }
        vector<int *> new_population;
        int s = (int) ((1 - params->x) * params->popsize);
        for (int i = 0; i < s; i++) {
            new_population.push_back(population[i]);
        }
        for (; new_population.size() < params->popsize;) {
            int r = rand() % s;
            int *parent1 = population[r];
            r = rand() % s;
            int *parent2 = population[r];
            int *offspring = cycle_crossover(parent1, parent2, n);
            new_population.push_back(offspring);
        }
        for (int i = 0; i < params->m * params->popsize; ++i) {
            int r = rand() % params->popsize;
            int *mutant = SBoxDecimalToBinary(new_population[r], pow(2, n), n);
            free(new_population[r]);
            new_population[r] = generate_descendant(mutant, pow(2, n), n);
            free(mutant);
        }
        for (int i = s; i < population.size(); ++i) {
            free(population[i]);
        }
        population = new_population;
        population.shrink_to_fit();
        generation++;
    }
    sort(population.begin(), population.end(), [n](int *x, int *y) {
        int *bx = SBoxDecimalToBinary(x, pow(2, n), n);
        int *by = SBoxDecimalToBinary(y, pow(2, n), n);
        int delta = SBoxNL(bx, pow(2, n), n) - SBoxNL(by, pow(2, n), n);
        free(bx);
        free(by);
        return delta > 0;
    });
    int *res = population[0];
    for (int i = 1; i < population.size(); ++i) {
        free(population[i]);
    }
    return res;
}

int *GATree(int *sbox_d, int n, GATParams *params) {
//    КРОК 0: Визначаємо параметри методу,
//    особливо ціле число N - розмірність S-коробки NxN,
//            де N = 8, ціле число M - кількість S-коробок у фермі,
//    ціле число C - кількість наступників для S-коробки кожної ферми,
//    ціле число I - максимальна кількість ітерацій у генетичній
//    частині алгоритму, ціле число NT - значення нелінійності
//    щоб перейти до деревної частини алгоритму, ціле число Z - максимум
//    кількість загально шуканих S-коробок, ціле число NEL
//    - бажане значення нелінійності. Визначаємо функцію витрат
//    CF. Якщо NEL менше або дорівнює NT, то деревна частина
//    алгоритм використовувати не буде.

//    КРОК 1: Встановіть нуль на кількість ітерацій. Встановіть для MxC кількість загально шуканих S-коробок. Випадково генерують MxC
//    S-коробки. Цей набір називається сукупністю. Для всіх S-коробок
//    серед населення, підрахуйте функцію витрат CF та нелінійність. Встановити в якості поточного найкращу нелінійність з
//    сукупності до цілого числа CN. Якщо будь-який S-box із сукупності має нелінійність більшу або рівну NEL, то GoTo
//    КРОК 9, або якщо будь-який S-box із сукупності має нелінійність, більшу або рівну NT, перейдіть до КРОКУ 4. Інакше
//    замовити S-коробки за функцією витрат CF та найкращим M
//    з них стають фермами. Потім перейдіть до КРОКУ 2.


    for (int i = 0; i < 0; ++i) {
        int *arr;
    }

    int width = 5;
    int height = 3;

    int *arr = (int *) malloc(sizeof(int) * width);
    for (int i = 0; i < width; ++i) {

    }
    *(arr + 0 * height + 0) = 1;
    for (int i = 0; i < height; ++i) {

    }

    return nullptr;
}