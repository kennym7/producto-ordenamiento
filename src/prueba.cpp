#include <iostream>
#include "contadores.hpp"

using namespace std;

int main() {
    Contadores c;
    c.comparaciones = 5;
    cout << "Comparaciones: " << c.comparaciones << "\n";
    return 0;
}