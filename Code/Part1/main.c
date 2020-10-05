#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // заголовочный файл, содержащий функции API

int AC(int *func, int size);

char *to_ANF(int *func, int size);

int myPow(int num, int pow);

int NL(int *f, int func_size);

int isBentFunction(int *func, int size);

int *gradient_descent(int *func, int func_size);

int *int_to_binary(int i, int rank);

int *array_copy(int *a, int size);

int log2int(int n);

int KP(int *func, int size, int k);

int *GF(int n);

int *reverse(int *f, int func_size);

int deg(int *f, int size);

int *WHT(int *func, int func_size);

int mod(int num, int mod);

int Hadamard_NL(int *f, int size);

int isBalanced(int *func, int size);

int abs_max(int *arr, int arr_size);

int *getFunctions(int *func, int size, int count);

//GF(2^n)
int main(int args, char **argv) {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    //printf("%d\n", MessageBox(NULL, "Привет, мир!!!", "Оконная процедура", MB_YESNO));

    int n = 3;
    int size = myPow(2, n);
    int df[] = {4, 6, 1, 0, 5, 2, 7, 3};
    printf("Задание 1.\n"
           "Задан произвольный S-бокс 3x3:\n");
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
    int *f3 = malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        f3[i] = ar[2 * size + i];
    }
    int answer = isBalanced(f3, size);
    printf("-является ли функция сбалансированной?  %s\n", answer == 1 ? "Yes" : "No");
    printf("-расчитать нелинейность функции NL через коэффициенты Уолша-Адамара: %d\n", Hadamard_NL(f3, size));
    printf("-рассчитать автокорреляцию AC функции: %d\n", AC(f3, size));
    printf("-представить функцию в АНФ:  %s.\n"
           "Определить ее алгебраическую степень Deg = %d.\n", to_ANF(f3, size), deg(f3, size));
    answer = KP(f3, size, 1);
    printf("-удовлетворяет ли функция КР степени 1, КР(1)? %s", answer == 1 ? "Yes" : "No");
    printf("Задание 2.\n"
           "Задана произвольная булева функция от 4 переменных:\nf = ");
    int f[] = {0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1};
    for (int i = 0; i < 16; ++i) {
        printf("%d ", f[i]);
    }
    printf("\nДаны ее коэффициенты Уолша-Адамара:\n");
    for (int i = 0; i < 16; ++i) {
        printf("W(%d) = %d\n", i, WHT(f, 16)[i]);
    }
    printf("1) Рассчитать NL функции.\n");
    printf("NL(f) = %d\n", NL(f, 16));
    printf("2) Методом градиентного спуска, если это возможно, понизить NL функции на 1.\n");

    printf("3) Рассчитать новые коэффициенты Уолша-Адамара, рассчитать новую NL.\n");
    printf("Задание 3.\n"
           "Построить S-бокс алгебраическим методом.\n");
    printf("/////////////////////\n");
    int function[] = {0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1};
    int *wht = WHT(function, 16);
    for (int i = 0; i < 16; ++i) {
        printf("%d\n", wht[i]);
    }
    printf("NL(f) = %d\n", Hadamard_NL(function, 16));
    printf("Is bent function: %s", isBentFunction(function, 16) ? "YES" : "NO");
    return 0;
}

int *getFunctions(int *func, int size, int count) {
    int *result = malloc(size * count * sizeof(int));
    for (int i = 0; i < size; ++i) {
        int *bin = int_to_binary(func[i], count);
        for (int j = 0; j < count; ++j) {
            result[j * size + i] = bin[j];
        }
    }
    return result;
}

int *int_to_binary(int i, int rank) {
    int *res = malloc(sizeof(int) * rank);
    for (int j = 0; j < rank; ++j) {
        res[rank - 1 - j] = i >> j & 1u;
    }
    return res;
}

int *array_copy(int *a, int size) {
    int *copy = malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        copy[i] = a[i];
    }
    return copy;
}

int KP(int *func, int size, int k) {
    int result = 0;
    for (int i = 0; i < size; ++i) {
        result += func[i] ^ func[i ^ k];
    }
    return result;
}

int *reverse(int *f, int func_size) {
    int *tmp = malloc(func_size * sizeof(int));
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
    int *matrix = malloc(sizeof(int) * size * size);
    for (int i = 0; i < size; ++i) {
        *(matrix + i) = *(func + size - 1 - i);
    }
    for (int i = 1; i < size; ++i) {
        for (int j = 0; j < size - i; ++j) {
            *(matrix + i * size + j) = (*(matrix + size * (i - 1) + j) + *(matrix + size * (i - 1) + (j + 1))) % 2;
        }
    }
    /*for (int i = 0; i < size; ++i) {
        if (*(matrix + i * size)) {
            for (int j = 0; j < n; ++j) {
                if ((table + i * n + j)) {
                    length += 2;
                }
            }
            length++;
        }
    }*/
    int *coefs = malloc(size * sizeof(int));
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

    char *result = malloc(sizeof(char) * length - 1);
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
    int *arr = malloc(size * sizeof(int));
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
    int *res = malloc(sizeof(*fp));
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
    int *res = malloc(sizeof(int) * size);
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
    int *arr = malloc(n * myPow(2, n) * sizeof(int));
    for (int i = 0; i < myPow(2, n); ++i) {
        for (int j = n - 1; j >= 0; --j) {
            *(arr + i * n + j) = (i >> (n - j - 1)) & 1u;
        }
    }
    return arr;
}

int *GF(int n) {
    int *arr = malloc(n * myPow(2, n) * sizeof(int));
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
int deg(int *f, int size) {
    int n = log2int(size);
    int max = 0;
    for (int i = 0; i < size; ++i) {
        if (f[i]) {
            int tmp = 0;
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1u) tmp++;
            }
            if (tmp > max) max = tmp;
        }
    }
    return max;
}

int deg2(int *f, int size) {
    char *anf = to_ANF(f, size);
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
    int *result = malloc(func_size * sizeof(int));
    int *w = malloc(n * sizeof(int));
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

int *AC_Vector(int *f, int size) {
    int *fs = ptt(f, size);
    fs = reverse(fs, size);
    int *ac = malloc(size * sizeof(int));
    for (int a = 0; a < size; ++a) {
        int result = 0;
        for (int i = 0; i < size; ++i) {
            result += fs[i] * (fs[i ^ a]);
        }
        ac[a] = result;
    }
    free(fs);
    return reverse(ac, size);
}

int AC(int *func, int size) {
    int max = func[0];
    for (int i = 0; i < size - 1; ++i) {
        if (max < func[i]) {
            max = func[i];
        }
    }
    return max;
}

int NL(int *f, int func_size) {
    int min;
    int cols = func_size;
    int rows = myPow(2, cols);
    int *functions = malloc(rows * cols * sizeof(int));
    for (int i = 1; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            *(functions + i * cols + j) = (i >> cols - j - 1) & 1u;
        }
    }
    min = d(f, functions, func_size);
    for (int i = 0; i < rows; ++i) {
        if (deg(functions + cols * i, func_size) <= 1 && !isEqual(f, functions + cols * i, func_size)) {
            int tmp = d(f, functions + cols * i, cols);
            if (tmp < min) min = tmp;
        }
    }
    return min;
}

int isAffine(int *func, int size) {
    return 0;
}

int abs_max(int *arr, int arr_size) {
    int max = 0;
    for (int i = 0; i < arr_size; ++i) {
        if (abs(arr[i] > max)) max = arr[i];
    }
    return max;
}

int isBalanced(int *func, int size) {
    return Hamming_Weight(func, size) == myPow(2, (log2int(size) - 1));
}

int isBentFunction(int *func, int size) {
    if (deg(func, size) > size / 2) return 0;
    int hw = Hamming_Weight(func, size);
    int n = myPow(2, size - 1) - myPow(2, size / 2 - 1);
    if (hw != (myPow(2, size - 1) + myPow(2, size / 2 - 1))
        &&
        hw != n)
        return 0;
    if (Hadamard_NL(func, size) != n) return 0;
    return 1;
}

int *gradient_descent(int *func, int func_size) {
    return NULL;
}