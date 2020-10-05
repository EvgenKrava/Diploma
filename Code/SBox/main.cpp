#include <cstdlib>
#include <clocale>
#include "sbox.h"


//GF(2^n)
int main(int args, char **argv) {
    setlocale(LC_ALL, "Rus");
    int func_num = 1;
    int n = 3;
    int size = myPow(2, n);
    int df[] = {7, 2, 1, 3, 6, 5, 0, 4};
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
    int *f1 = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        f1[i] = ar[(func_num - 1) * size + i];
    }
    int answer = isBalanced(f1, size);
    printf("-является ли функция сбалансированной?  %s\n", answer == 1 ? "Yes" : "No");
    int *wht = WHT(f1, size);
    for (int i = 0; i < size; ++i) {
        printf("W(%d) = %d\n", i, wht[i]);
    }
    printf("-расчитать нелинейность функции NL через коэффициенты Уолша-Адамара: %d\n", NL(f1, size));
    int *ac = AC(f1, size);
    for (int i = 0; i < size; ++i) {
        printf("AC(%d) = %d\n", i, ac[i]);
    }
    printf("-рассчитать автокорреляцию AC функции: %d\n", abs_max(ac + 1, size - 1));
    printf("-представить функцию в АНФ:  %s.\n"
           "Определить ее алгебраическую степень Deg = %d.\n", to_ANF(f1, size), deg(to_ANF(f1, size)));
    answer = criterion_of_degree(f1, size, 1);
    printf("-удовлетворяет ли функция КР степени 1, КР(1)? %s\n", answer == 1 ? "Yes" : "No");
    printf("Задание 2.\n"
           "Задана произвольная булева функция от 4 переменных:\nf = ");
    int f[] = {1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1};
    for (int i : f) {
        printf("%d ", i);
    }
    printf("\nДаны ее коэффициенты Уолша-Адамара:\n");
    int *wht2 = WHT(f, 16);
    for (int i = 0; i < 16; ++i) {
        printf("W(%d) = %d\n", i, wht2[i]);
    }
    printf("1) Рассчитать NL функции.\n");
    printf("NL(f) = %d\n", NL(f, 16));
    printf("2) Методом градиентного спуска, если это возможно, понизить NL функции на 1.\n");

    printf("3) Рассчитать новые коэффициенты Уолша-Адамара, рассчитать новую NL.\n");
    printf("Задание 3.\n"
           "Построить S-бокс алгебраическим методом.\n");
    IS(f, 16);
    up(f, 16);
    printf("\nf = ");
    for (int i : f) {
        printf("%d ", i);
    }
    printf("\nNL(f) = %d\n", NL(f, 16));
    IS(f, 16);
    down(f, 16);
    printf("\nf = ");
    for (int i : f) {
        printf("%d ", i);
    }
    printf("\nNL(f) = %d\n", NL(f, 16));
    wht2 = WHT(f, 16);
    for (int i = 0; i < 16; ++i) {
        printf("W(%d) = %d\n", i, wht2[i]);
    }
    return 0;
}





