#include <locale>
#include <iostream>
#include <map>
#include <windows.h>
#include "fstream"
#include "sbox.h"

void printFunctionParams(int *func, int size);

int read_settings_from_file(const char *file_name, parameters *params);

void save_result_to_file(int *sbox, int size, int count, struct parameters params, int time_in_microseconds,
                         const char *file_name);

void fill_parameters_default_values(parameters *pParameters);

void save_statistics_in_file(std::vector<pair> element, const char *file_name);

//GF(2^n)
int main(int argc, char **argv) {
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
    static parameters params;
    std::string filename = "settings.txt";
    fill_parameters_default_values(&params);//встановлюємо параметри за замовчуванням
    //Зчитування параметрів з файлу
    if (read_settings_from_file(filename.c_str(), &params)) {
        std::cerr
                << "Can't open settings.txt file!" << std::endl
                << "Use default parameters!" << std::endl;
    } else {
        std::cout << "Parameters read successfully!" << std::endl;
    }
    std::cout << "Генерацiя SBox (кiлькiсть потокiв " << params.thread_count << ")" << std::endl;
    int n = params.N;
    int size = pow(2, n);
    std::cout << "Вхiднi параметри: " << std::endl
              << "Ожидаемая нелинейность: " << params.requiredNL << std::endl
              << "Ожидаемая автокорреляция: " << params.requiredAC << std::endl
              << "Начальная температура: " << params.initial_temperature << std::endl
              << "Количество внутренних циклов: " << params.MIL << std::endl
              << "Количество внешних циклов: " << params.MUL << std::endl
              << "Коэффициент затухания: " << params.solidification_coefficient << std::endl
              << "X1: " << params.X1 << " X2: " << params.X2 << " R1: " << params.R1 << " R2: " << params.R2
              << std::endl;
    int *sbox = generate_sbox(n, n);//генерація випадкового s-box
    int *result = (int *) malloc(sizeof(int) * size * n);
    printf("   NL    AC      COST      TEMP     DELTA\n");
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> end;
    pthread_t ptids[params.thread_count];
    start = std::chrono::high_resolution_clock::now();//Початок вимірювання часу
    pthread_simulating_annealing_args pthreadSimulatingAnnealingArgs{
            sbox,
            size,
            n,
            &params,
            result,
    };
    for (int i = 0; i < params.thread_count; ++i) {
        pthread_create(ptids + i, nullptr, &pthread_simulating_annealing, &pthreadSimulatingAnnealingArgs);
    }
    for (int i = 0; i < params.thread_count; ++i) {
        pthread_join(ptids[i], nullptr);
    }
    end = std::chrono::high_resolution_clock::now();//Кінець вимірювання часу
    result = pthreadSimulatingAnnealingArgs.result;

    int *dec = SBoxBinaryToDecimal(result, size, n);//бінарне представлення у десяткове
    for (int i = 0; i < size; ++i) {
        printf("%02X ", dec[i]);
    }
    save_result_to_file(result, size, n, params,
                        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(),
                        "result.txt");
    printf("Результат збережено у файл \"result.txt\" \n Час роботи алгоритму: %d seconds\n",
           std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000);
    save_statistics_in_file(params.pairs, "statistics.csv");
    return 0;
    /*for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", *(sbox + i * size + j));
        }
        printf("\n");
    }*/

    /* int sbox[] = {0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1};
     using namespace std;
     cout << SBoxNL(sbox, 8, 3) << endl;
     cout << LAT(sbox, 8, 3) << endl;*/

    /*int hex[] = {0xC3, 0xFF, 0x04, 0x83, 0x12, 0xF3, 0xB8, 0x1F, 0x57, 0x0B, 0xA6, 0x64, 0xE1, 0x49, 0x0C, 0x18, 0xC9,
                 0x68, 0x3B, 0x90, 0xDF, 0x44, 0x3C, 0x45, 0x0E, 0x79, 0x72, 0xFA, 0xF2, 0xDE, 0x81, 0x65, 0xF0, 0xF7,
                 0xB3, 0x74, 0xBB, 0x80, 0x1B, 0x4D, 0xD4, 0xDB, 0x58, 0x26, 0x1C, 0xF1, 0x05, 0x02, 0x2E, 0xA1, 0x5B,
                 0x71, 0x39, 0x0D, 0x52, 0x87, 0x76, 0x4B, 0x29, 0xE7, 0x6F, 0x17, 0x2D, 0x62, 0x1D, 0xAE, 0x8D, 0x19,
                 0xA2, 0xBE, 0x14, 0x1E, 0x5A, 0xF6, 0xE9, 0x9F, 0xAB, 0xA5, 0x6A, 0xD6, 0x9A, 0xDD, 0x08, 0x3E, 0x5C,
                 0x63, 0xE4, 0xA0, 0xAD, 0x36, 0x8E, 0x4A, 0x51, 0x07, 0x56, 0x31, 0xB4, 0x5D, 0x6B, 0x32, 0x43, 0xFE,
                 0x34, 0xEB, 0xD7, 0x41, 0x5F, 0x10, 0x61, 0x42, 0x98, 0xD3, 0x3F, 0x9D, 0xBC, 0x70, 0xD8, 0xF8, 0x54,
                 0xBD, 0x2B, 0x82, 0x37, 0x20, 0x35, 0x8A, 0xCE, 0x25, 0xBF, 0xB7, 0xEA, 0xE5, 0xEC, 0x93, 0x28, 0x8C,
                 0xE6, 0x22, 0xF5, 0xC5, 0xE3, 0xD1, 0x23, 0xA3, 0x9E, 0x2C, 0xEE, 0x53, 0x06, 0xCB, 0x21, 0x4E, 0x9B,
                 0x8F, 0xFB, 0xCF, 0xA9, 0xC4, 0xDC, 0x40, 0xAF, 0x13, 0x48, 0xAC, 0x16, 0xD9, 0x50, 0x38, 0x66, 0xED,
                 0xB6, 0x01, 0xCD, 0xB2, 0x4C, 0xCC, 0xFD, 0xB0, 0x67, 0xA8, 0xCA, 0x84, 0x09, 0xAA, 0x96, 0xD2, 0x46,
                 0x95, 0xFC, 0x85, 0x5E, 0xB1, 0x33, 0xC1, 0x86, 0x8B, 0xE0, 0xBA, 0x2F, 0x0F, 0x6D, 0x3A, 0x91, 0x11,
                 0x7B, 0x27, 0xC8, 0xF9, 0x00, 0x99, 0x9C, 0x7C, 0x2A, 0x60, 0x7E, 0xEF, 0x6C, 0x3D, 0x77, 0xB5, 0x4F,
                 0xC0, 0x88, 0x73, 0x7F, 0x55, 0xF4, 0x75, 0xC6, 0xE2, 0x69, 0xB9, 0x7D, 0x03, 0x47, 0x92, 0xA7, 0xA4,
                 0x78, 0x6E, 0x59, 0x7A, 0x97, 0x0A, 0xD5, 0x94, 0x24, 0xC7, 0xC2, 0x30, 0x1A, 0x15, 0x89, 0xE8, 0xD0,
                 0xDA};
    int *sbox = SBoxDecimalToBinary(hex, 256, 8);
    std::cout << pow(2, 7) - LAT(sbox, 256, 8) << std::endl;
    std::cout << SBoxNL(sbox, 256, 8);*/
}

void save_statistics_in_file(std::vector<pair> vector, const char *file_name) {
    FILE *f = fopen(file_name, "w");
    if (f != nullptr) {
        fprintf(f, "NL, AC, COUNT\n");
        for (auto it = vector.begin(); it != vector.end(); it++) {
            int ac = it->AC;
            int nl = it->NL;
            it->count = std::count_if(vector.begin(), vector.end(),
                                      [nl, ac](pair element) {
                                          return element.AC == ac &&
                                                 element.NL == nl;

                                      });
        }
        for (int i = 0; i < vector.size(); ++i) {
            for (int j = i + 1; j < vector.size(); ++j) {
                if (vector[i].NL == vector[j].NL && vector[i].AC == vector[j].AC) {
                    vector.erase(vector.begin() + j);
                }
            }
        }
        for (auto it = vector.begin(); it != vector.end(); it++) {
            fprintf(f, "%d, %d, %d\n", it->NL, it->AC, it->count);
        }
    }
    fclose(f);
}


void fill_parameters_default_values(parameters *pParameters) {
    pParameters->N = 8;
    pParameters->MUL = 100;
    pParameters->R2 = 2;
    pParameters->R1 = 2;
    pParameters->X1 = 16;
    pParameters->X2 = 16;
    pParameters->solidification_coefficient = 0.999;
    pParameters->initial_temperature = 1000000;
    pParameters->requiredAC = 64;
    pParameters->requiredNL = 104;
    pParameters->MIL = 100;
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    int numCPU = sysinfo.dwNumberOfProcessors;
    pParameters->thread_count = numCPU - 1;
    pParameters->ready = 0;
}

void save_result_to_file(int *sbox, int size, int count, struct parameters params, int time_in_microseconds,
                         const char *file_name) {
    int *dec = SBoxBinaryToDecimal(sbox, size, count);
    FILE *f = fopen(file_name, "a");
    if (f != nullptr) {
        for (int i = 0; i < size; ++i) {
            fprintf(f, "%02X ", dec[i]);
        }
        fprintf(f, "\nНелiнiйнiсть: %d", pow(2, count - 1) - LAT(sbox, size, count));
        fprintf(f, "\nАвтокорреляцiя: %d", SBoxAC(sbox, size, count));
        fprintf(f, "\nЧас виконання: %dc", time_in_microseconds / 1000000);
        fprintf(f, "\nX1: %d X2: %d R1: %d R2: %d\n", params.X1, params.X2, params.R1, params.R2);
    }
    fclose(f);
}

int read_settings_from_file(const char *file_name, struct parameters *params) {
    std::string line;
    std::ifstream in(file_name); // окрываем файл для чтения
    if (in.is_open()) {
        while (getline(in, line)) {
            int index = line.find(": ");
            if (line.find("нелинейность") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());
                params->requiredNL = i > 0 ? i : 100;
            }
            if (line.find("автокорреляция") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());
                params->requiredAC = i > 0 ? i : 70;
            }
            if (line.find("температура") != std::string::npos) {
                double i = atof(line.substr(index + 1, line.size() - index).c_str());
                params->initial_temperature = i > 1000 ? i : 5000;
            }
            if (line.find("внутренних циклов") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());
                params->MIL = i > 0 ? i : 100;
            }
            if (line.find("внешних циклов") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());
                params->MUL = i > 0 ? i : 100;
            }
            if (line.find("затухания") != std::string::npos) {
                double f = atof(line.substr(index + 1, line.size() - index).c_str());;
                params->solidification_coefficient = f > 0 && f < 1 ? f : 0.5;
            }
            if (line.find("X1") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->X1 = f > 0 ? f : 2;
            }
            if (line.find("X2") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->X2 = f > 0 ? f : 2;
            }
            if (line.find("R1") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->R1 = f > 0 ? f : 2;
            }
            if (line.find("R2") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->R2 = f > 0 ? f : 2;
            }
            if (line.find("N:") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->N = f > 0 ? f : 8;
            }
            if (line.find("thread count") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->thread_count = i > 0 ? i : 1;
            }
        }
        in.close();
        return 0;
    } else {
        return -1;
    }
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
