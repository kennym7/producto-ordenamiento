import sys
import os
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
 
ruta = sys.argv[1] if len(sys.argv) > 1 else "results/benchmark.csv"
df = pd.read_csv(ruta)
salida = "results/analisis"
os.makedirs(salida, exist_ok=True)
 
ALGS = ["insercion", "quicksort"]
PATRONES = ["aleatorio", "ascendente", "descendente", "casi_ordenado", "duplicados"]
COLORES = {"insercion": "#d95f02", "quicksort": "#1b9e77"}
n_max = df["n"].max()
 
print(f"Filas leidas: {len(df)}")
print(f"Repeticiones por condicion: {df.groupby(['algoritmo','n','patron']).size().unique()}")
 
est_t = (df.groupby(["algoritmo", "patron", "n"])["tiempo_ms"]
           .agg(media="mean", mediana="median", desv_est="std", minimo="min", maximo="max")
           .reset_index())
est_t.to_csv(f"{salida}/estadisticas_tiempo.csv", index=False)
 
est_m = (df.groupby(["algoritmo", "patron", "n"])
           .agg(comparaciones=("comparaciones", "mean"),
                intercambios=("intercambios", "mean"),
                profundidad_max=("profundidad_recursion", "max"))
           .reset_index())
est_m.to_csv(f"{salida}/estadisticas_metricas.csv", index=False)
 
def a_markdown(tabla, formato):
    cols = list(tabla.columns)
    lineas = ["| " + " | ".join(cols) + " |", "|" + "---|" * len(cols)]
    for _, fila in tabla.iterrows():
        celdas = []
        for c in cols:
            v = fila[c]
            celdas.append(formato.get(c, "{}").format(v))
        lineas.append("| " + " | ".join(celdas) + " |")
    return "\n".join(lineas)
 
t1 = est_t[est_t["n"] == n_max].drop(columns="n")
t1 = t1.sort_values(["patron", "algoritmo"])
fmt_t = {c: "{:.3f}" for c in ["media", "mediana", "desv_est", "minimo", "maximo"]}
with open(f"{salida}/tabla1_tiempos.md", "w", encoding="utf-8") as f:
    f.write(f"Tabla 1. Tiempo de ejecucion (ms) con n = {n_max:,}\n\n")
    f.write(a_markdown(t1, fmt_t) + "\n")
 
t2 = est_m[est_m["n"] == n_max].drop(columns="n")
t2 = t2.sort_values(["patron", "algoritmo"])
fmt_m = {"comparaciones": "{:,.0f}", "intercambios": "{:,.0f}", "profundidad_max": "{:.0f}"}
with open(f"{salida}/tabla2_metricas.md", "w", encoding="utf-8") as f:
    f.write(f"Tabla 2. Comparaciones, intercambios y profundidad de recursion con n = {n_max:,}\n\n")
    f.write(a_markdown(t2, fmt_m) + "\n")
 
fig, ax = plt.subplots(figsize=(11, 5.5))
datos, posiciones, colores = [], [], []
for i, pat in enumerate(PATRONES):
    for j, alg in enumerate(ALGS):
        serie = df[(df.n == n_max) & (df.patron == pat) & (df.algoritmo == alg)]["tiempo_ms"]
        datos.append(serie.values)
        posiciones.append(i * 3 + j)
        colores.append(COLORES[alg])
bp = ax.boxplot(datos, positions=posiciones, widths=0.8, patch_artist=True,
                medianprops=dict(color="black"))
for caja, col in zip(bp["boxes"], colores):
    caja.set_facecolor(col)
    caja.set_alpha(0.8)
ax.set_xticks([i * 3 + 0.5 for i in range(len(PATRONES))])
ax.set_xticklabels(PATRONES)
ax.set_yscale("log")
ax.set_ylabel("Tiempo (ms, escala logaritmica)")
ax.set_title(f"Figura 1. Distribucion del tiempo por algoritmo y patron (n = {n_max:,})")
ax.legend(handles=[plt.Rectangle((0, 0), 1, 1, color=COLORES[a], alpha=0.8) for a in ALGS],
          labels=["Insercion", "Quicksort"], loc="best")
ax.grid(axis="y", alpha=0.3)
fig.tight_layout()
fig.savefig(f"{salida}/figura1_boxplot.png", dpi=200)
plt.close(fig)
 
fig, axes = plt.subplots(2, 3, figsize=(13, 7.5))
axes = axes.flatten()
for k, pat in enumerate(PATRONES):
    ax = axes[k]
    for alg in ALGS:
        s = est_t[(est_t.patron == pat) & (est_t.algoritmo == alg)].sort_values("n")
        ax.errorbar(s["n"], s["media"], yerr=s["desv_est"], marker="o", capsize=3,
                    color=COLORES[alg], label="Insercion" if alg == "insercion" else "Quicksort")
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_title(pat)
    ax.set_xlabel("n")
    ax.set_ylabel("Tiempo medio (ms)")
    ax.grid(alpha=0.3, which="both")
    ax.legend()
axes[5].axis("off")
fig.suptitle("Figura 2. Tiempo medio vs. tamano de entrada (escala log-log, barras = desv. estandar)")
fig.tight_layout()
fig.savefig(f"{salida}/figura2_curvas.png", dpi=200)
plt.close(fig)
 
print(f"\nListo. Archivos generados en {salida}/")
for nombre in sorted(os.listdir(salida)):
    print("  -", nombre)
