#include <iostream>
#include <cmath>
#include <bitset>
#include <conio.h>
#include <ctime>
#include "fstream"
#include <cstdlib>
#include "props.h"
#include "simulating_annealing.h"

int read(const char *file_name, int *sbox, int n);

unsigned long mix(unsigned long clock, unsigned long time, unsigned long pid);

int main(int argc, char **argv) {
    setlocale(LC_ALL, "Rus");
    unsigned long seed = mix(clock(), time(nullptr), getpid());
    srand(seed);
    using namespace std;
    int n = 8;
    int size = (int) pow(2, n);
    int *sbox = (int *) malloc(sizeof(int) * size);
    read("file.txt", sbox, 8);
    for (int i = 0; i < size; ++i) {
        printf("%02X ", sbox[i]);
    }
    cout << endl << "LAT: " << LAT(SBoxDecimalToBinary(sbox, n), n);
    cout << endl << "Нелинейность: " << NL(SBoxDecimalToBinary(sbox, n), n);
    cout << endl << "Д Равномерность: " << delta_uniformity(sbox, n);
    cout << endl << "min_degree: " << min_degree(sbox, n);
    getch();

    return 0;
}

int SBoxNL(int *sbox, int n) {
    return (int) pow(2, n - 1) - LAT(sbox, n);
}


int read(const char *file_name, int *sbox, int n) {
    using namespace std;
    int size = (int) pow(2, n);
    string line;
    ifstream in(file_name);
    if (in.is_open()) {
        while (getline(in, line)) {
            //cout << line;
            for (int i = 0; i < size; i++) {
                sscanf(line.substr(3 * i, 2).c_str(), "%x", sbox + i);
            }
        }
        in.close();
    }
    return 0;
}

unsigned long mix(unsigned long clock, unsigned long time, unsigned long pid) {
    clock = clock - time;
    clock = clock - pid;
    clock = clock ^ (pid >> 13);
    time = time - pid;
    time = time - clock;
    time = time ^ (clock << 8);
    pid = pid - clock;
    pid = pid - time;
    pid = pid ^ (time >> 13);
    clock = clock - time;
    clock = clock - pid;
    clock = clock ^ (pid >> 12);
    time = time - pid;
    time = time - clock;
    time = time ^ (clock << 16);
    pid = pid - clock;
    pid = pid - time;
    pid = pid ^ (time >> 5);
    clock = clock - time;
    clock = clock - pid;
    clock = clock ^ (pid >> 3);
    time = time - pid;
    time = time - clock;
    time = time ^ (clock << 10);
    pid = pid - clock;
    pid = pid - time;
    pid = pid ^ (time >> 15);
    return pid;
}
