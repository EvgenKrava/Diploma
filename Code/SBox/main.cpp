#include <clocale>
#include <iostream>
#include "sbox.h"


void printFunctionParams(int *func, int size);

//GF(2^n)
int main(int args, char **argv) {
    setlocale(LC_ALL, "Rus");
    /*int func_num = 1;
    int n = 3;
    int size = pow(2, n);
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

    printf("3) Рассчитать новые коэффициенты Уолша-Адамара, рассчитать новую NL.\n");*/
    /*int size = 16;
    int *f = read_from_file("D:\\Diploma\\Code\\SBox\\test.txt", size);
    for (int i = 0; i < size; ++i) {
        printf("%d ", f[i]);
    }
    int *fu = generate_function(size);
    printf("\nf(bend) = ");
    for (int k = 0; k < size; ++k) {
        printf("%d ", fu[k]);
    }
    printf("\nNL = %d", NL(fu, size));
    gradient_down(fu, size);
    printf("\nf = ");
    for (int k = 0; k < size; ++k) {
        printf("%d ", fu[k]);
    }
    printf("\nNL = %d", NL(fu, size));*/
    int n = 8;
    int size = pow(2, n);
    int *sbox = generate_sbox(n, 8);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", *(sbox + i * size + j));
        }
        printf("\n");
    }
    printf("cost = %d\n", cost_4_11(sbox, size, 8));
    printf("NL = %d\n", SboxNL(sbox, size, 8));
    printf("AC = %d\n", SBoxAC(sbox, size, 8));
    printf("Balanced: %s\n", isBalancedSBox(sbox, size, 8) ? "Yes" : "No");
    printf("\n");
    auto start = std::chrono::high_resolution_clock::now();
    sbox = simulated_annealing(sbox, size, 8, 104, 60);
    auto end = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", *(sbox + i * size + j));
        }
        printf("\n");
    }
    printf("cost = %d\n", cost_4_11(sbox, size, 8));
    printf("NL = %d\n", SboxNL(sbox, size, 8));
    printf("AC = %d\n", SBoxAC(sbox, size, 8));
    printf("Balanced: %s\n", isBalancedSBox(sbox, size, 8) ? "Yes" : "No");
    printf("%d microseconds\n", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    return 0;
}

void printFunctionParams(int *func, int size) {
    printf("Функция: ");
    for (int i = 0; i < size; ++i) {
        printf("%d", func[i]);
    }
    printf("\n");
    char *anf = to_ANF(func, size);
    printf("АНФ сгенерированной функции: %s\n", anf);
    printf("Сбалансированность: %s\n", isBalanced(func, size) ? "Да" : "Нет");
    printf("Нелинейность: %d\n", NL(func, size));
    printf("Автокорреляция: %d\n", autocorrelation(func, size));
    printf("Критерий распространения: %d\n", criterion_of_degree(func, size, 1));
    printf("Алгебраическая степень: %d\n", deg(anf));
    printf("Бент: %s\n", isBentFunction(func, size) ? "Да" : "Нет");
    free(anf);
}
