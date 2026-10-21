#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include "../include/algoritmos.hpp"
#include "../include/generadores.hpp"
using namespace std;

bool estaOrdenado(int a[], int n) {
    for (int i = 1; i < n; i++)
        if (a[i - 1] > a[i]) return false;
    return true;
}

int main(int argc, char* argv[]) {
    // Uso: benchmark [repeticiones] [n_maximo] [archivo_csv] [commit_hash]
    int repeticiones = (argc > 1) ? stoi(argv[1]) : 30;
    int nMax         = (argc > 2) ? stoi(argv[2]) : 100000;
    string salida    = (argc > 3) ? argv[3] : "results/benchmark.csv";
    string commit    = (argc > 4) ? argv[4] : "desconocido";

    unsigned SEMILLA_BASE = 12345;

    int tamanos[] = {1000, 10000, 100000};
    string patrones[] = {"aleatorio", "ascendente", "descendente",
                        "casi_ordenado", "duplicados"};
    string algoritmos[] = {"insercion", "quicksort"};

    ofstream csv(salida.c_str());
    if (!csv) {
        cout << "No se pudo crear " << salida << " (existe la carpeta results?)" << endl;
        return 1;
    }
    csv << "algoritmo,n,patron,repeticion,tiempo_ms,comparaciones,intercambios,"
        "profundidad_recursion,semilla,commit_hash\n";

    for (int t = 0; t < 3; t++) {
        int n = tamanos[t];
        if (n > nMax) continue;
        cout << "Procesando n = " << n << "..." << endl;

        for (int p = 0; p < 5; p++) {
            for (int rep = 1; rep <= repeticiones; rep++) {
                unsigned semilla = SEMILLA_BASE + rep;

                int* original = new int[n];
                generar(patrones[p], original, n, semilla);

                for (int k = 0; k < 2; k++) {
                    int* copia = new int[n];
                    for (int i = 0; i < n; i++) copia[i] = original[i];

                    Contadores c;

                    auto t0 = chrono::steady_clock::now();
                    if (k == 0) insercion(copia, n, c);
                    else        quicksort(copia, n, c);
                    auto t1 = chrono::steady_clock::now();

                    double ms = chrono::duration<double, milli>(t1 - t0).count();

                    if (!estaOrdenado(copia, n)) {
                        cout << "ERROR: " << algoritmos[k] << " no ordeno bien" << endl;
                        return 1;
                    }

                    csv << algoritmos[k] << "," << n << "," << patrones[p] << ","
                        << rep << "," << ms << "," << c.comparaciones << ","
                        << c.intercambios << "," << c.profundidadMax << ","
                        << semilla << "," << commit << "\n";

                    delete[] copia;
                }
                delete[] original;
            }
        }
    }
    cout << "Listo. Resultados en " << salida << endl;
    return 0;
}