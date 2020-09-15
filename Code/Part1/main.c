#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int log2int(int n);

int *GF(int n);

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

int part(char *str, int index, int n) {
    if (*str == '1') return 1;
    int *table = GF(n);
    char *ptr = strtok(str, "x");
    int res = 1;
    while (ptr != NULL) {
        printf("\n%d\n", ptr[0] - 48);
        res *= table[index * n + n - (int) ptr[0] - 48];
        ptr = strtok(NULL, "x");
    }
    free(table);
    return res;
}

int *to_Table(char *ANF, int size) {
    int *arr = malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        char *ptr = strtok(ANF, "+");
        while (ptr != NULL) {
            arr[i] += part(ptr, i, log2int(size)) % 2;
            ptr = strtok(NULL, "+");
        }
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
        fprintf(file, "%d ", function[i]);
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

int max(int *arr, int size) {
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
    for (int i = 0; i < size; ++i) {
        f[i] = myPow(-1, f[i]);
    }
    return f;
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
int W(int *f, int size) {
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
        if (f[size - i - 1]) {
            int tmp = 0;
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1u) tmp++;
            }
            if (tmp > max) max = tmp;
        }
    }
    return max;
}

int NLAdamara(int *f) {
//TODO
    return -1;
}

//Walsh Hadamard Transform
int WHT() {
    //TODO
    return -1;
}

int rf(int a, int *fs, int size) {
    int res = 0;
    if (a == 0) {
        return size;
    }
    for (int i = 0; i < size; ++i) {
        printf("%d ", fs[i]);
    }
    printf("\n");
    for (int i = 0; i < size; ++i) {
        res += (fs[size - 1 - i] * fs[size - 1 - (i ^ a)]);
        printf("  %d %d   %d %d", i, i ^ a, fs[i], fs[i ^ a]);
    }

    printf(" = %d\n", res);
    return res;
}

int *AC(int *f, int size) {
    int *fs = ptt(f, size);
    int *fsa = malloc(size * sizeof(int));
    int *a = malloc(size * sizeof(int));

    for (int alpha = 0; alpha < size; ++alpha) {
        int tmp = rf(alpha, fs, size);
        a[size - alpha - 1] = tmp;
    }
    return a;
}

int NL(int *f, int fSize) {
    int min;
    int cols = fSize;
    int rows = myPow(2, cols);
    int functions[rows][cols];
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            functions[i][j] = (i >> (cols - j - 1)) & 1u;
        }
    }
    min = d(f, functions[0], cols);
    for (int i = 0; i < rows; ++i) {
        if (deg(functions[i], fSize) <= 1 && !isEqual(f, functions[i], cols)) {
            int tmp = d(f, functions[i], cols);
            if (tmp < min) min = tmp;
        }
    }
    return min;
}

//GF(2^n)
int main(int args, char **argv) {
    /*for (int i = 0; i < args; ++i) {
        printf("%s\n", argv[i]);
    }*/
    int n = 3;
    int size = myPow(2, n);
    int *f = read_from_file("D:\\Diploma\\Code\\Part1\\test.txt", size);
    printf("%s\n", to_ANF(f, size));
    printf("func = ");
    for (int i = 0; i < size; ++i) {
        printf("%d ", *to_Table(to_ANF(f, size), size));
    }
    printf("\n");
    for (int i = 0; i < size; ++i) {
        printf("%d ", f[i]);
    }
    printf("\n");
    printf("NL(f) = %d\n", NL(f, size));
    printf("if W(f) = %d == %d then function balanced\n", W(f, size), myPow(2, n - 1));
    printf("deg(f) = %d\n", deg(f, myPow(2, n)));
    printTableUsingArgs(f, n);
    printf("%d\n", isEqual(f, f, size));
    //f = ptt(f, size);
    for (int i = 0; i < size; ++i) {
        printf("%d ", f[i]);
    }
    printf("\n");
    int *ac = AC(f, size);
    for (int i = 0; i < size; ++i) {
        printf("%d ", ac[i]);
    }
    printf("\n%d\n", 0 ^ 1);
    write_binary_to_file("D:\\Diploma\\Code\\Part1\\write_test.txt", f, size);
    return 0;
}
