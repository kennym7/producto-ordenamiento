#include <iostream>
#include "algoritmos.hpp"
using namespace std;

int main() {
    int n = 6;
    int datos[] = {5, 2, 4, 9, 1, 7};
    Contadores c;
    insercion(datos, n, c);
    for (int i = 0; i < n; i++) cout << datos[i] << " ";
    cout << "| comp " << c.comparaciones << " interc " << c.intercambios << endl;
    return 0;
}
