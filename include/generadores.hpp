#pragma once
#include <string>
#include <random>
#include <algorithm>
using namespace std;

inline void generar(string patron, int a[], int n, unsigned semilla) {
    mt19937 gen(semilla);

    if (patron == "aleatorio") {
        uniform_int_distribution<int> d(0, 1000000);
        for (int i = 0; i < n; i++) a[i] = d(gen);
    }
    else if (patron == "ascendente") {
        for (int i = 0; i < n; i++) a[i] = i;
    }
    else if (patron == "descendente") {
        for (int i = 0; i < n; i++) a[i] = n - i;
    }
    else if (patron == "casi_ordenado") {
        for (int i = 0; i < n; i++) a[i] = i;
        uniform_int_distribution<int> d(0, n - 1);
        int k = n / 20;
        for (int t = 0; t < k; t++) swap(a[d(gen)], a[d(gen)]);
    }
    else if (patron == "duplicados") {
        uniform_int_distribution<int> d(0, 14);
        for (int i = 0; i < n; i++) a[i] = d(gen);
    }
}