#include <locale>
#include <iostream>
#include <pthread.h>
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
    static parameters params;
    std::string filename = "settings.txt";
    fill_parameters_default_values(&params);//������������ ��������� �� �������������
    //���������� ��������� � �����
    if (read_settings_from_file(filename.c_str(), &params)) {
        std::cerr
                << "Can't open settings.txt file!" << std::endl
                << "Use default parameters!" << std::endl;
    } else {
        std::cout << "Parameters read successfully!" << std::endl;
    }
    std::cout << "�������i� SBox (�i���i��� �����i� " << params.thread_count << ")" << std::endl;
    int n = params.N;
    int size = pow(2, n);
    std::cout << "��i��i ���������: " << std::endl
              << "��������� ������������: " << params.requiredNL << std::endl
              << "��������� ��������������: " << params.requiredAC << std::endl
              << "��������� �����������: " << params.initial_temperature << std::endl
              << "���������� ���������� ������: " << params.MIL << std::endl
              << "���������� ������� ������: " << params.MUL << std::endl
              << "����������� ���������: " << params.solidification_coefficient << std::endl
              << "X1: " << params.X1 << " X2: " << params.X2 << " R1: " << params.R1 << " R2: " << params.R2
              << std::endl;
    int *sbox = generate_sbox(n, n);//��������� ����������� s-box
    int *result = (int *) malloc(sizeof(int) * size * n);
    printf("   NL    AC      COST      TEMP     DELTA\n");
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> end;
    pthread_t ptids[params.thread_count];
    start = std::chrono::high_resolution_clock::now();//������� ���������� ����
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
    end = std::chrono::high_resolution_clock::now();//ʳ���� ���������� ����
    result = pthreadSimulatingAnnealingArgs.result;

    int *dec = SBoxBinaryToDecimal(result, size, n);//������ ������������� � ���������
    for (int i = 0; i < size; ++i) {
        printf("%02X ", dec[i]);
    }
    save_result_to_file(result, size, n, params,
                        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(),
                        "result.txt");
    printf("��������� ��������� � ���� \"result.txt\" \n ��� ������ ���������: %d seconds\n",
           std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000);
    save_statistics_in_file(params.pairs, "statistics.csv");
    return 0;
    /*for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", *(sbox + i * size + j));
        }
        printf("\n");
    }*/

    /*int sbox[] = {0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1};
    using namespace std;
    cout << SBoxNL(sbox, 8, 3) << endl;
    cout << LAT(sbox, 8, 3) << endl;*/

}

void save_statistics_in_file(std::vector<pair> vector, const char *file_name) {
    FILE *f = fopen(file_name, "w");
    if (f != nullptr) {
        fprintf(f, "NL, AC, COUNT");
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
        fprintf(f, "\n���i�i��i���: %d", pow(2, count - 1) - LAT(sbox, size, count));
        fprintf(f, "\n������������i�: %d", SBoxAC(sbox, size, count));
        fprintf(f, "\n��� ���������: %dc", time_in_microseconds / 1000000);
        fprintf(f, "\nX1: %d X2: %d R1: %d R2: %d\n", params.X1, params.X2, params.R1, params.R2);
    }
    fclose(f);
}

int read_settings_from_file(const char *file_name, struct parameters *params) {
    std::string line;
    std::ifstream in(file_name); // �������� ���� ��� ������
    if (in.is_open()) {
        while (getline(in, line)) {
            int index = line.find(": ");
            if (line.find("������������") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());
                params->requiredNL = i > 0 ? i : 100;
            }
            if (line.find("��������������") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());
                params->requiredAC = i > 0 ? i : 70;
            }
            if (line.find("�����������") != std::string::npos) {
                double i = atof(line.substr(index + 1, line.size() - index).c_str());
                params->initial_temperature = i > 1000 ? i : 5000;
            }
            if (line.find("���������� ������") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());
                params->MIL = i > 0 ? i : 100;
            }
            if (line.find("������� ������") != std::string::npos) {
                int i = atoi(line.substr(index + 1, line.size() - index).c_str());
                params->MUL = i > 0 ? i : 100;
            }
            if (line.find("���������") != std::string::npos) {
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
