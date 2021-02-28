#include <iostream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

// реалізація фільтру Блума

typedef unsigned int(*HashFunction)(string);

class BloomFilter {
    unsigned int numberOfCells;
    unsigned int numberOfFunctions;
    bool *cell;
    vector<HashFunction> hashFunctions;

public:

    BloomFilter(unsigned int numbCells, vector<HashFunction> funcs) : numberOfCells(numbCells), hashFunctions(funcs) {
        cell = (bool *) calloc(numbCells, sizeof(bool));
    }

    void addElement(string str) {
        for (vector<HashFunction>::iterator iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
            cell[(*iter)(str) % numberOfCells] = true;
        }
    }

    bool searchElement(string str) {
        bool strInSet = true;

        for (vector<HashFunction>::iterator iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
            if (cell[(*iter)(str) % numberOfCells] == false) {
                strInSet = false;
                break;
            }
        }

        return strInSet;
    }

    ~BloomFilter() {
        free(cell);
        cell = NULL;
    }
};

// реалізація пари хеш-функцій для тестування

unsigned int djb2(string str) {
    unsigned int hash = 5381;

    for (string::iterator iter = str.begin(); iter != str.end(); iter++) {
        hash = ((hash << 5) + hash) + *iter;
    }

    return hash;
}

unsigned int sdbm(string str) {
    unsigned int hash = 0;

    for (string::iterator iter = str.begin(); iter != str.end(); iter++) {
        hash = ((hash << 6) + (hash << 16) - hash) + *iter;
    }

    return hash;
}


int main() {
    // створення фільтру Блума
    vector<HashFunction> hashFunctions;
    hashFunctions.push_back(djb2);
    hashFunctions.push_back(sdbm);

    BloomFilter bf(1024, hashFunctions);

    // вставка слів до фільтру
    vector<string> setOfStrings({
                                        "Mad World!",
                                        "Hi Oleh Ivanovich",
                                        "C",
                                        "alpha",
                                        "beta",
                                        "gamma"
                                });

    cout << "Bloom filter created." << endl;
    cout << "Bloom filter tests for the following set of strings:" << endl;

    for (vector<string>::iterator iter = setOfStrings.begin(); iter != setOfStrings.end(); iter++) {
        bf.addElement(*iter);
        cout << "\t" + *iter << endl;
    }

    // рядки для тестування фільтру Блума
    vector<string> testSetOfStrings({
                                            "Hello World!",
                                            "Hi Oleh Ivanovich",
                                            "C",
                                            "alpha",
                                            "beta",
                                            "gamma",
                                            "delta",
                                            "where am i?",
                                            "foo",
                                            "bar"
                                    });

    cout << "Testing set inclusion for the following strings:" << endl;
    cout << boolalpha;

    for (vector<string>::iterator iter = testSetOfStrings.begin(); iter != testSetOfStrings.end(); iter++) {
        cout << "\t" + *iter + ": " << bf.searchElement(*iter) << endl;
    }

    getchar();
}


