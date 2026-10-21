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

