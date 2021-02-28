#include <locale>
#include <iostream>
#include <map>
#include <windows.h>
#include "fstream"
#include "sbox.h"

void save_GAResult_to_file(int *sbox, int n, GAParams params, int time_in_microseconds,
                           const char *file_name);

int read_SAParams_from_file(const char *file_name, SAParams *params);

int read_GAParams_from_file(const char *file_name, GAParams *params);


void save_SAResult_to_file(int *sbox, int size, int count, struct SAParams params, int time_in_microseconds,
                           const char *file_name);

void fill_SAParams_default_values(SAParams *pParameters);

void fill_GAParams_default_values(GAParams *pParameters);

void save_statistics_in_file(std::vector<pair> vector, const char *file_name);

unsigned long mix(unsigned long a, unsigned long b, unsigned long c);

void SA();

void GA();

//GF(2^n)
int main(int argc, char **argv) {
    setlocale(LC_ALL, "Rus");
    unsigned long seed = mix(clock(), time(nullptr), getpid());
    srand(seed);
    SA();
    //GA();
    return 0;
}

void GA() {
    using namespace std;
    std::cout << "Genetic Algorithm" << std::endl;
    static GAParams params;
    std::string fileName = "GASettings.txt";
    fill_GAParams_default_values(&params);
    if (read_GAParams_from_file(fileName.c_str(), &params)) {
        std::cerr << "Can't open settings file!" << std::endl
                  << "Use default parameters!" << std::endl;
    } else {
        std::cout << "Parameters read successfully!" << std::endl;
    }
    int n = 8;
    int size = pow(2, n);
    cout << "Init params:" << endl
         << "Population size " << params.popsize << endl
         << "Fraction of the population to be replaced by crossover in each iteration " << params.x << endl
         << "Mutation rate " << params.m << endl;
    int *result = GeneticAlgorithm(n, &params);
    for (int i = 0; i < size; ++i) {
        printf("%02X ", result[i]);
    }
    save_GAResult_to_file(result, n, params, 0, "GAResult.txt");
    cout << endl;
}

void SA() {
    std::cout << "Simulating Annealing" << std::endl;
    static SAParams params;
    std::string filename = "SASettings.txt";
    fill_SAParams_default_values(&params);//встановлюємо параметри за замовчуванням
    //Зчитування параметрів з файлу
    if (read_SAParams_from_file(filename.c_str(), &params)) {
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
              << "X1: " << params.costParams.X1
              << " X2: " << params.costParams.X2
              << " R1: " << params.costParams.R1
              << " R2: " << params.costParams.R2
              << std::endl;
    int *sbox = generate_binary_sbox(n, n);//генерація випадкового s-box
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
    std::string fileName = "SAResult.txt";
    save_SAResult_to_file(result, size, n, params,
                          std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(),
                          fileName.c_str()
    );
    printf("\nРезультат збережено у файл \"%s\" \n Час роботи алгоритму: %d seconds\n",
           fileName.c_str(),
           std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000);
    save_statistics_in_file(params.pairs, "statistics.csv");
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


void fill_SAParams_default_values(SAParams *pParameters) {
    pParameters->N = 8;
    pParameters->MUL = 100;
    pParameters->costParams.R2 = 2;
    pParameters->costParams.R1 = 2;
    pParameters->costParams.X1 = 16;
    pParameters->costParams.X2 = 16;
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

void fill_GAParams_default_values(GAParams *pParameters) {
    pParameters->costParams.R2 = 2;
    pParameters->costParams.R1 = 2;
    pParameters->costParams.X1 = 16;
    pParameters->costParams.X2 = 16;
    pParameters->x = 0.7;
    pParameters->popsize = 10;
    pParameters->requiredAC = 64;
    pParameters->requiredNL = 104;
    pParameters->m = 0.3;
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    int numCPU = sysinfo.dwNumberOfProcessors;
    pParameters->thread_count = numCPU - 1;
}


void save_SAResult_to_file(int *sbox, int size, int count, struct SAParams params, int time_in_microseconds,
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
        fprintf(f, "\nX1: %d X2: %d R1: %d R2: %d\n",
                params.costParams.X1,
                params.costParams.X2,
                params.costParams.R1,
                params.costParams.R2);
    }
    fclose(f);
}

void save_GAResult_to_file(int *sbox, int n, GAParams params, int time_in_microseconds,
                           const char *file_name) {
    int size = pow(2, n);
    FILE *f = fopen(file_name, "a");
    int *bin = SBoxDecimalToBinary(sbox, pow(2, n), n);
    if (f != nullptr) {
        for (int i = 0; i < size; ++i) {
            fprintf(f, "%02X ", sbox[i]);
        }
        fprintf(f, "\nНелiнiйнiсть: %d", pow(2, n - 1) - LAT(bin, size, n));
        fprintf(f, "\nАвтокорреляцiя: %d", SBoxAC(bin, size, n));
        fprintf(f, "\nЧас виконання: %dc", time_in_microseconds / 1000000);
        fprintf(f, "\nX1: %d X2: %d R1: %d R2: %d\n", params.costParams.X1, params.costParams.X2, params.costParams.R1,
                params.costParams.R2);
    }
    fclose(f);
}

int read_SAParams_from_file(const char *file_name, SAParams *params) {
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
                params->costParams.X1 = f > 0 ? f : 2;
            }
            if (line.find("X2") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->costParams.X2 = f > 0 ? f : 2;
            }
            if (line.find("R1") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->costParams.R1 = f > 0 ? f : 2;
            }
            if (line.find("R2") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->costParams.R2 = f > 0 ? f : 2;
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

int read_GAParams_from_file(const char *file_name, GAParams *params) {
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
            if (line.find("Population size") != std::string::npos) {
                double i = atof(line.substr(index + 1, line.size() - index).c_str());
                params->popsize = i > 1 ? i : 1;
            }
            if (line.find("Generation count") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());
                params->generationCount = i > 0 ? i : 100;
            }

            if (line.find("x:") != std::string::npos) {
                double f = atof(line.substr(index + 1, line.size() - index).c_str());;
                params->x = f > 0 && f < 1 ? f : 0.5;
            }
            if (line.find("x:") != std::string::npos) {
                double f = atof(line.substr(index + 1, line.size() - index).c_str());;
                params->m = f > 0 && f < 1 ? f : 0.5;
            }
            if (line.find("X1") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->costParams.X1 = f > 0 ? f : 2;
            }
            if (line.find("X2") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->costParams.X2 = f > 0 ? f : 2;
            }
            if (line.find("R1") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->costParams.R1 = f > 0 ? f : 2;
            }
            if (line.find("R2") != std::string::npos) {
                int f = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->costParams.R2 = f > 0 ? f : 2;
            }

            if (line.find("thread count") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());;
                params->thread_count = i > 1 ? i : 1;
            }
        }
        in.close();
        return 0;
    } else {
        return -1;
    }
}

unsigned long mix(unsigned long a, unsigned long b, unsigned long c) {
    a = a - b;
    a = a - c;
    a = a ^ (c >> 13);
    b = b - c;
    b = b - a;
    b = b ^ (a << 8);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 13);
    a = a - b;
    a = a - c;
    a = a ^ (c >> 12);
    b = b - c;
    b = b - a;
    b = b ^ (a << 16);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 5);
    a = a - b;
    a = a - c;
    a = a ^ (c >> 3);
    b = b - c;
    b = b - a;
    b = b ^ (a << 10);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 15);
    return c;
}

