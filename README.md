# Comparación experimental de algoritmos de ordenamiento: Inserción vs. Quicksort

Proyecto de la asignatura **Programación Avanzada** (Unidad I).
Universidad Nacional Jorge Basadre Grohmann, Facultad de Ingeniería, Escuela Profesional de Ingeniería en Informática y Sistemas.
Ciclo II, sección B, semestre 2026-II.

- **Autor:** Rodrigo Kenny Calcina Uribe (rcalcinau@unjbg.edu.pe)
- **Docente:** Hugo Manuel Barraza Vizcarra

## Objetivo

Comparar empíricamente el ordenamiento por **Inserción** (método directo, O(n²)) y **Quicksort** (método logarítmico, O(n log n) en promedio). Se miden el tiempo de ejecución, las comparaciones, los intercambios y la profundidad de recursión, y los resultados se relacionan con la complejidad teórica y la estabilidad de cada algoritmo.

## Resultados clave

Tiempo medio (ms) con n = 100 000 y 30 repeticiones por condición (tabla completa en `results/analisis/tabla1_tiempos.md`):

| Patrón | Inserción | Quicksort | Observación |
|---|---|---|---|
| Aleatorio | 3105.065 | 15.347 | Quicksort unas 202 veces más rápido |
| Ascendente | 0.318 | 2.697 | Inserción unas 8.5 veces más rápida |
| Descendente | 6271.632 | 3.004 | Quicksort unas 2 090 veces más rápido |
| Casi ordenado | 396.827 | 4.916 | Quicksort unas 81 veces más rápido |
| Duplicados | 2923.731 | 6.004 | Quicksort unas 490 veces más rápido |

- Con entrada descendente, Inserción hizo n(n − 1)/2 comparaciones (4 999 950 000); con entrada ascendente, n − 1 (99 999).
- Al multiplicar n por 10, las comparaciones de Inserción se multiplicaron por cerca de 100 y las de Quicksort por entre 11.8 y 12.6, como predicen O(n²) y O(n log n).
- La profundidad máxima de recursión de Quicksort fue de 38 niveles con n = 100 000 (entrada aleatoria) y 18 con entradas ordenadas.
- El análisis completo está en el artículo (`docs/manuscrito.pdf`).

## Algoritmos

| Algoritmo | Tipo | Mejor / promedio / peor | Estable |
|---|---|---|---|
| Inserción | Directo | O(n) / O(n²) / O(n²) | Sí |
| Quicksort | Logarítmico | O(n log n) / O(n log n) / O(n²) | No |

- **Variante de Quicksort:** pivote por mediana de tres y partición de Hoare. Evita el peor caso con entradas ordenadas y maneja bien los valores repetidos.
- **Métricas:** en Inserción, cada desplazamiento se cuenta como un intercambio. En Quicksort, la mediana de tres suma 3 comparaciones por llamada. La profundidad cuenta la primera llamada como nivel 1 (en Inserción es 0).
- Ambos algoritmos son implementaciones propias; no se usa `std::sort`.

## Diseño experimental

- **Patrones de entrada:** aleatorio, ascendente, descendente, casi ordenado (n/20 intercambios aleatorios sobre un arreglo ordenado) y con duplicados (15 valores únicos).
- **Tamaños:** n = 1 000, 10 000 y 100 000.
- **Repeticiones:** 30 por condición (algoritmo × patrón × tamaño), es decir, 900 ejecuciones.
- **Semillas:** semilla base 12345; en la repetición `r` se usa `12345 + r`. Ambos algoritmos reciben una copia del mismo arreglo en cada repetición. Los patrones ascendente y descendente no dependen de la semilla.
- **Medición:** `std::chrono::steady_clock`, solo el ordenamiento (se excluyen la copia del arreglo, la verificación y la escritura del CSV). Tiempo en milisegundos.
- **Verificación:** tras cada ordenamiento se comprueba que el arreglo quedó ordenado.

## Estructura del repositorio

```
analisis/    analisis.py (estadísticas, tablas y figuras)
docs/        manuscrito.pdf
include/     contadores.hpp, algoritmos.hpp, generadores.hpp
src/         main.cpp (benchmark)
results/     benchmark.csv (corrida completa) y results/analisis/ (tablas y figuras)
```

Los archivos `results/piloto_*.csv`, si existen, son pruebas piloto con pocas repeticiones y **no** se usan en el análisis.

## Requisitos

- Compilador `g++` con soporte de C++17.
- Python 3 con `pandas` y `matplotlib` (solo para el análisis): `pip install pandas matplotlib`.

## Cómo compilar

Desde la carpeta raíz del repositorio:

```bash
g++ -O2 -std=c++17 src/main.cpp -o benchmark.exe
```

En Linux o macOS puedes usar `-o benchmark` en lugar de `-o benchmark.exe`.

## Cómo ejecutar el benchmark

```bash
./benchmark.exe [repeticiones] [n_maximo] [archivo_csv] [commit_hash]
```

En PowerShell o CMD de Windows se escribe `.\benchmark.exe`. Si no se pasan argumentos, se usan 30 repeticiones, n máximo de 100 000 y la salida `results/benchmark.csv`.

**Corrida completa** (genera los resultados del artículo; la parte de Inserción con n = 100 000 es la más lenta y toda la corrida puede tardar varios minutos):

```bash
./benchmark.exe 30 100000 results/benchmark.csv 7b3eca4
```

**Prueba piloto rápida** (3 repeticiones, n hasta 10 000):

```bash
./benchmark.exe 3 10000 results/piloto_3rep_n10000.csv 7b3eca4
```

## Cómo regenerar tablas y figuras

```bash
python analisis/analisis.py results/benchmark.csv
```

Esto crea en `results/analisis/`:

| Archivo | Contenido |
|---|---|
| `tabla1_tiempos.md` | Tabla 1: media, mediana, desviación estándar, mínimo y máximo del tiempo (n = 100 000) |
| `tabla2_metricas.md` | Tabla 2: comparaciones, intercambios y profundidad de recursión (n = 100 000) |
| `figura1_boxplot.png` | Figura 1: boxplot del tiempo por algoritmo y patrón |
| `figura2_curvas.png` | Figura 2: curvas n vs. tiempo por patrón |
| `estadisticas_tiempo.csv` | Estadísticas del tiempo para todos los n |
| `estadisticas_metricas.csv` | Métricas para todos los n |

## Formato de los resultados

Columnas de `results/benchmark.csv`:

`algoritmo, n, patron, repeticion, tiempo_ms, comparaciones, intercambios, profundidad_recursion, semilla, commit_hash`

## Entorno experimental

| Elemento | Valor |
|---|---|
| CPU | Intel(R) Core(TM) i3-6006U, 2 núcleos / 4 hilos, 2.00 GHz base, caché L3 de 3 MB |
| RAM | 11.9 GB |
| Sistema operativo | Windows 10 Home Single Language, versión 22H2 (compilación 19045.6466) |
| Compilador | g++ (Rev8, Built by MSYS2 project) 15.2.0 |
| Opciones de compilación | `-O2 -std=c++17` |
| Python (análisis) | 3.14.3, con pandas y matplotlib |

Todas las ejecuciones se hicieron en el mismo equipo: estuvo en margen el uso de visual studio code, git bash, python y obs studio. 

## Versión y reproducibilidad

- **Commit del código que generó los resultados:** `7b3eca4`
- **Commit de la versión final del repositorio:** b3bac50

Con el mismo compilador y la misma biblioteca estándar, las comparaciones, los intercambios y la profundidad de recursión deben coincidir exactamente con los de `results/benchmark.csv`, porque las semillas son fijas. Los tiempos dependen del equipo y variarán entre ejecuciones.

## Entregables

- **Artículo científico:** `docs/manuscrito.pdf`
- **Video de demostración (YouTube, no listado):** https://youtu.be/-t7QMwpXnIs
- **Diapositivas:** 
