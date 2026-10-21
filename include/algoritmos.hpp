#pragma once
#include <iostream>
#include "contadores.hpp"
using namespace std;

//in
inline void insercion(int a[], int n, Contadores& c) {
    for (int i = 1; i < n; i++) {
        int clave = a[i];
        int j = i - 1;
        while (j >= 0) {
            c.comparaciones++;
            if (a[j] > clave) {
                a[j + 1] = a[j];
                c.intercambios++;
                j--;
            } else {
                break;
            }
        }
        a[j + 1] = clave;
    }
}

inline int medianaDeTres(int x, int y, int z, Contadores& c) {
    c.comparaciones += 3;
    if ((x <= y && y <= z) || (z <= y && y <= x)) return y;
    if ((y <= x && x <= z) || (z <= x && x <= y)) return x;
    return z;
}

inline void quicksortRec(int a[], int lo, int hi, Contadores& c, int prof) {
    if (prof > c.profundidadMax) c.profundidadMax = prof;
    if (lo >= hi) return;

    int mid = lo + (hi - lo) / 2;
    int pivote = medianaDeTres(a[lo], a[mid], a[hi], c);

    int i = lo - 1;
    int j = hi + 1;
    while (true) {
        do { i++; c.comparaciones++; } while (a[i] < pivote);
        do { j--; c.comparaciones++; } while (a[j] > pivote);
        if (i >= j) break;
        swap(a[i], a[j]);
        c.intercambios++;
    }
    quicksortRec(a, lo, j, c, prof + 1);
    quicksortRec(a, j + 1, hi, c, prof + 1);
}

inline void quicksort(int a[], int n, Contadores& c) {
    if (n > 0) quicksortRec(a, 0, n - 1, c, 1);
}
