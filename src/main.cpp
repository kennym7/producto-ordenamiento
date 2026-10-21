#include <iostream>
#include "../include/algoritmos.hpp"

using namespace std;

int main() {
    int n = 6;
    int d[] = {5, 2, 4, 9, 1, 7};
    Contadores c;
    quicksort(d, n, c);
    for (int i = 0; i < n; i++) cout << d[i] << " ";
    cout << "| comp " << c.comparaciones << " interc " << c.intercambios
        << " prof " << c.profundidadMax << endl;
    return 0;
}
