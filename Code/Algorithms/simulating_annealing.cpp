//
// Created by Yevhenii on 2/28/2021.
//

#include "simulating_annealing.h"

void *pthread_simulating_annealing(void *pthreadParams);

/**
 * Генерація нащадка на основі батьківського sbox
 * @param sbox_b батьківський sbox
 * @param n - розмырність sbox
 * @return нащадка
 */
int *generate_descendant(const int *sbox_b, int n);

int *single_simulating_annealing(const int *sbox_b, int n, GeneratingParams generatingParams);

int *simulating_annealing(const int *sbox_b, int n, GeneratingParams generatingParams) {
    pthread_t ptids[generatingParams.count_of_threads];
    PthreadParams pthreadParams{
            pthreadParams.sbox_b = sbox_b,
            pthreadParams.n = n,
            pthreadParams.generatingParams = generatingParams,
    };
    for (int i = 0; i < generatingParams.count_of_threads; ++i) {
        pthread_create(ptids + i, nullptr, &pthread_simulating_annealing, &pthreadParams);
    }
    for (int i = 0; i < generatingParams.count_of_threads; ++i) {
        pthread_join(ptids[i], nullptr);
    }
    return pthreadParams.target_sbox_b;
}

void *pthread_simulating_annealing(void *pthreadParams) {
    auto *pParams = (PthreadParams *) pthreadParams;
    int *res = single_simulating_annealing(pParams->sbox_b, pParams->n, pParams->generatingParams);
    if (res) {
        for (int i = 0; i < (int) pow(2, pParams->n) * pParams->n; ++i) {
            pParams->target_sbox_b[i] = res[i];
        }
    }
    //pthread_exit(args1->result);
    return pParams;
}

int *single_simulating_annealing(const int *sbox_b, int n, GeneratingParams generatingParams) {
    int size = (int) pow(2, n);
    double delta;
    int costS, costY;
    int SNL = 0;
    int SAC = 0;
    int *bestSBox = array_copy(sbox_b, size * n);
    int bestNL = NL(sbox_b, n);
    int MUL = generatingParams.MUL;
    int MIL = generatingParams.MIL;//количество внутренних циклов
    double a = generatingParams.solidification_coefficient;//коэфициент застывания
    double T = generatingParams.initial_temperature;//начальная температура
    //std::mt19937 engine; // mt19937 - генератор случайных чисел
    int *S = (int *) malloc(size * n * sizeof(int));
    //записываем изначальный s-box в переменную S
    for (int i = 0; i < size * n; ++i) {
        S[i] = sbox_b[i];
    }
    //Пока не достигнуты требуемые значания нелинейности и автокареляции
    while (NL(S, n) < generatingParams.target_nonlinearity && MUL > 0) {
        if (generatingParams.target_sbox_generated) {
            return nullptr;
        }
        //Выполняем поиск лучшего потомка указанное количество раз
        for (int i = 0; i < MIL; ++i) {
            int *Y = generate_descendant(S, n);//генерация потомка
            costY = cost(Y, n, generatingParams.costParams);//стоимость потомка
            costS = cost(S, n, generatingParams.costParams);//текущая стоимость
            delta = costY - costS;//разница стоимости
            //если потомок лучше текущего претендента
            if (delta < 0) {
                for (int j = 0; j < size * n; ++j) {
                    S[j] = Y[j];
                }
            } else {
                //генерируем случайное число 0..1
                double u = (double) (rand() + pthread_self()) / RAND_MAX;
                double t = (double) delta / T;
                if (u < exp(-t)) {
                    //с некой вероятностью принимаем худшего потомка
                    for (int j = 0; j < size * n; ++j) {
                        S[j] = Y[j];
                    }
                }
            }
            free(Y);
        }//кінець внутрішнього циклу
        T = T * a;//уменьшаем температуру
        SNL = NL(S, n);
        if (SNL > bestNL) {
            for (int j = 0; j < size * n; ++j) {
                bestSBox[j] = S[j];
            }
            bestNL = SNL;
        }
        printf("%5d %5d %9d %9.0f %9.0f\n",
               SNL,
               SAC,
               costS,
               T,
               delta);//вивід даних у консоль

        MUL--;
    }//кінець зовнішннього циклу
    generatingParams.target_sbox_generated = 1;
    return bestSBox;
}


int *generate_descendant(const int *sbox_b, int n) {
    int size = (int) pow(2, n);
    int *Y = (int *) malloc(sizeof(int) * size * n);
    int a = (int) (rand() + pthread_self()) % size;
    int b = (int) (rand() + pthread_self()) % size;
    //printf("%d %d\n", a, b);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == a) {
                *(Y + j * size + i) = *(sbox_b + j * size + b);
            } else {
                *(Y + j * size + i) = *(sbox_b + j * size + (i == b ? a : i));
            }
        }
    }
    return Y;
}