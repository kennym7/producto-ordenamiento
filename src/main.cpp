#include <iostream>
#include "../include/generadores.hpp"
using namespace std;

int main() {
    int n = 40;
    int a[40];
    string pats[] = {"aleatorio","ascendente","descendente","casi_ordenado","duplicados"};
    for (int p = 0; p < 5; p++) {
        generar(pats[p], a, n, 12345);
        cout << pats[p] << ": ";
        for (int i = 0; i < n; i++) cout << a[i] << " ";
        cout << endl << endl;
    }
    return 0;
}