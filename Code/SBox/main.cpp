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
    printf("������� 1.\n"
           "����� ������������ S-���� 3x3:\n");
    for (int i = 0; i < size; ++i) {
        printf("%d ", df[i]);
    }
    printf("\n1) ����������� ��� � ���� ������� �������.\n");
    int *ar = getFunctions(df, size, n);
    for (int i = 0; i < n; ++i) {
        printf("f%d = ", i);
        for (int j = 0; j < size; ++j) {
            printf("%d ", ar[i * size + j]);
        }
        printf("\n");
    }
    printf("2) ��� ������� 3 ���������� �� ��������� ����������������� ����������:\n");
    int *f1 = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        f1[i] = ar[(func_num - 1) * size + i];
    }
    int answer = isBalanced(f1, size);
    printf("-�������� �� ������� ����������������?  %s\n", answer == 1 ? "Yes" : "No");
    int *wht = WHT(f1, size);
    for (int i = 0; i < size; ++i) {
        printf("W(%d) = %d\n", i, wht[i]);
    }
    printf("-��������� ������������ ������� NL ����� ������������ �����-�������: %d\n", NL(f1, size));
    int *ac = AC(f1, size);
    for (int i = 0; i < size; ++i) {
        printf("AC(%d) = %d\n", i, ac[i]);
    }
    printf("-���������� �������������� AC �������: %d\n", abs_max(ac + 1, size - 1));
    printf("-����������� ������� � ���:  %s.\n"
           "���������� �� �������������� ������� Deg = %d.\n", to_ANF(f1, size), deg(to_ANF(f1, size)));
    answer = criterion_of_degree(f1, size, 1);
    printf("-������������� �� ������� �� ������� 1, ��(1)? %s\n", answer == 1 ? "Yes" : "No");
    printf("������� 2.\n"
           "������ ������������ ������ ������� �� 4 ����������:\nf = ");
    int f[] = {1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1};
    for (int i : f) {
        printf("%d ", i);
    }
    printf("\n���� �� ������������ �����-�������:\n");
    int *wht2 = WHT(f, 16);
    for (int i = 0; i < 16; ++i) {
        printf("W(%d) = %d\n", i, wht2[i]);
    }
    printf("1) ���������� NL �������.\n");
    printf("NL(f) = %d\n", NL(f, 16));
    printf("2) ������� ������������ ������, ���� ��� ��������, �������� NL ������� �� 1.\n");

    printf("3) ���������� ����� ������������ �����-�������, ���������� ����� NL.\n");*/
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
    printf("�������: ");
    for (int i = 0; i < size; ++i) {
        printf("%d", func[i]);
    }
    printf("\n");
    char *anf = to_ANF(func, size);
    printf("��� ��������������� �������: %s\n", anf);
    printf("������������������: %s\n", isBalanced(func, size) ? "��" : "���");
    printf("������������: %d\n", NL(func, size));
    printf("��������������: %d\n", autocorrelation(func, size));
    printf("�������� ���������������: %d\n", criterion_of_degree(func, size, 1));
    printf("�������������� �������: %d\n", deg(anf));
    printf("����: %s\n", isBentFunction(func, size) ? "��" : "���");
    free(anf);
}
