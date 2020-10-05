#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>

int KP(int *func, int size, int alpha);

int Hamming_Weight(int *f, int size);

int criterion_of_degree(int *func, int size, int k);

int *AC(int *func, int size);

char *to_ANF(int *func, int size);

int myPow(int num, int pow);

int NL(int *f, int func_size);

int isBentFunction(int *func, int size);

int *gradient_descent(int *func, int func_size);

int *int_to_binary(int i, int rank);

int *array_copy(int *a, int size);

int log2int(int n);

int d(int *f, int *g, int size);

int vector_criterion(int *func, int size, int k);

int *GF(int n);

int *reverse(int *f, int func_size);

int deg(char *anf);

int *WHT(int *func, int func_size);

int mod(int num, int mod);

int Hadamard_NL(int *f, int size);

int isBalanced(int *func, int size);

int abs_max(int *arr, int size);

int *getFunctions(int *func, int size, int count);

int isEqual(int *f1, int *f2, int size);

int *IS(int func, int size);

int L(int *w, int *x, int size);

int *to_arr(std::vector<int> vector);

int L(int *w, int *x, int size) {
    int res = 0;
    for (int i = 0; i < size; ++i) {
        if (w[i]) res += x[i];
    }
    return res % 2;
}

int *IS(int *func, int size) {
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
    for (int i = 0; i < size; ++i) {
        int c_p = 0;
        int c_m = 0;
        for (int j = 0; j < w_plus.size(); j++) {
            int *a = to_arr(w_plus);
            if (func[i] == L(a, xs + (i * n), n)) {
                c_p++;
            }
            free(a);
        }
        for (int j = 0; j < w_minus.size(); j++) {
            int *a = to_arr(w_minus);
            if (func[i] != L(a, xs + (i * n), n))
                c_m++;
            free(a);
        }
        printf("%d %d\n", c_p, c_m);
        if (!(isBalanced(func, size) && c_m == w_minus.size() && c_p == w_plus.size())) {
            for (int j = i * n; j < i * n + n; ++j) {
                is.push_back(xs[j]);
            }
        }
    }

    free(ws);
    free(wht);
    return to_arr(is);
}

int *to_arr(std::vector<int> vector) {
    int *arr = (int *) malloc(sizeof(int) * vector.size());
    for (int i = 0; i < vector.size(); ++i) {
        arr[i] = vector[i];
    }
    return arr;
}


//GF(2^n)
int main(int args, char **argv) {
    int n = 3;
    int size = myPow(2, n);
    int df[] = {7, 1, 6, 2, 0, 3, 4, 5};
    printf("Задание 1.\nЗадан произвольный S-бокс 3x3:\n");
    for (int i = 0; i < size; ++i) {
        printf("%d ", df[i]);
    }
    printf("\n1) Представить его в виде булевых функций.\n");
    int *ar = getFunctions(df, size, n);
    for (int i = 0; i < n; ++i) {
        printf("f%d = ", i);
        for (int j = 0; j < size; ++j) {
            printf("%d ", ar[i * size + j]);
        }
        printf("\n");
    }
    printf("2) Для функции 3 определить ее следующие криптографические показатели:\n");
    int *f3 = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        f3[i] = ar[2 * size + i];
    }
    int answer = isBalanced(f3, size);
    printf("-является ли функция сбалансированной?  %s\n", answer == 1 ? "Yes" : "No");
    int *h = WHT(f3, size);
    for (int i = 0; i < size; ++i) {
        printf("W(%d) = %d\n", i, h[i]);
    }
    printf("-расчитать нелинейность функции NL через коэффициенты Уолша-Адамара: %d\n", NL(f3, size));
    int *ac = AC(f3, size);
    for (int i = 0; i < size; ++i) {
        printf("AC(%d) = %d\n", i, ac[i]);
    }
    printf("-рассчитать автокорреляцию AC функции: %d\n", abs_max(ac + 1, size - 1));
    printf("-представить функцию в АНФ:  %s.\n"
           "Определить ее алгебраическую степень Deg = %d.\n", to_ANF(f3, size), deg(to_ANF(f3, size)));
    answer = criterion_of_degree(f3, size, 1);
    printf("-удовлетворяет ли функция КР степени 1, КР(1)? %s\n", answer == 1 ? "Yes" : "No");
    printf("Задание 2.\n"
           "Задана произвольная булева функция от 4 переменных:\nf = ");
    int f[] = {1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0};
    for (int i : f) {
        printf("%d ", i);
    }
    printf("\nДаны ее коэффициенты Уолша-Адамара:\n");
    int *wht = WHT(f, 16);
    for (int i = 0; i < 16; ++i) {
        printf("W(%d) = %d\n", i, wht[i]);
    }
    printf("1) Рассчитать NL функции.\n");
    printf("NL(f) = %d\n", NL(f, 16));
    printf("2) Методом градиентного спуска, если это возможно, понизить NL функции на 1.\n");

    printf("3) Рассчитать новые коэффициенты Уолша-Адамара, рассчитать новую NL.\n");
    printf("Задание 3.\n"
           "Построить S-бокс алгебраическим методом.\n");
    int *is = IS(f, 16);
    for (int i = 0; i < 16; ++i) {
        printf("%d", is[i]);
        if ((i + 1) % 4 == 0) printf(" ");
    }
    return 0;
}

int *getFunctions(int *func, int size, int count) {
    int *result = (int *) malloc(size * count * sizeof(int));
    for (int i = 0; i < size; ++i) {
        int *bin = int_to_binary(func[i], count);
        for (int j = 0; j < count; ++j) {
            result[j * size + i] = bin[count - j - 1];
        }
    }
    return result;
}

int *int_to_binary(int i, int rank) {
    int *res = (int *) malloc(sizeof(int) * rank);
    for (int j = 0; j < rank; ++j) {
        res[rank - 1 - j] = i >> j & 1u;
    }
    return res;
}

int *array_copy(int *a, int size) {
    int *copy = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        copy[i] = a[i];
    }
    return copy;
}

int KP(int *func, int size, int alpha) {
    int res = 0;
    for (int i = 0; i < size; ++i) {
        res += (func[i] + func[i ^ alpha]) % 2;
    }
    //printf("KP(%d) = %s\n", alpha, res ? "Yes" : "No");
    return res;
}

int criterion_of_degree(int *func, int size, int k) {
    int *gf = GF(size);
    for (int i = 0; i < myPow(2, size); ++i) {
        int hw = Hamming_Weight(gf + i * size, size);
        if (hw >= 1 && hw <= k) {
            if (!KP(func, size, i)) {
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

char *to_ANF(int *func, int size) {
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

int Hadamard_NL(int *f, int size) {
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

int NL(int *f, int func_size) {
    int min;
    int cols = func_size;
    int rows = myPow(2, cols);
    int *functions = (int *) malloc(rows * cols * sizeof(int));
    for (int i = 1; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            *(functions + i * cols + j) = (i >> cols - j - 1) & 1u;
        }
    }
    min = d(f, functions, func_size);
    for (int i = 0; i < rows; ++i) {
        if (deg(to_ANF(functions + cols * i, func_size)) <= 1 && !isEqual(f, functions + cols * i, func_size)) {
            int tmp = d(f, functions + cols * i, cols);
            if (tmp < min) min = tmp;
        }
    }
    return min;
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
    if (Hadamard_NL(func, size) != n) return 0;
    return 1;
}


