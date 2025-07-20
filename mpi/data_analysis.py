#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt

# 1) Ler os dados
df = pd.read_csv('mpi_data.csv')

# 2) Para cada métrica, plotar curvas com N no eixo X e linhas para cada p
procs = sorted(df['procs'].unique())
Ns = sorted(df['n'].unique())

# 3) Tempo vs N
plt.figure()
for p in procs:
    sub = df[df['procs'] == p].set_index('n').loc[Ns]
    plt.plot(Ns, sub['tempo_total'], marker='o', label=f'{p} proc')
plt.title('MPI: Tempo x N')
plt.xlabel('Tamanho do array (N)')
plt.ylabel('Tempo total (s)')
plt.legend(title='Processos')
plt.grid(True)
plt.savefig('mpi_tempo_vs_N.png')
plt.close()

# 4) Speedup vs N
# Speedup = T(1) / T(p)
t1 = df[df['procs'] == 1].set_index('n')['tempo_total']
plt.figure()
for p in procs:
    sub = df[df['procs'] == p].set_index('n').loc[Ns]
    speedup = t1.loc[Ns] / sub['tempo_total']
    plt.plot(Ns, speedup, marker='^', label=f'{p} proc')
plt.title('MPI: Speedup x N')
plt.xlabel('Tamanho do array (N)')
plt.ylabel('Speedup')
plt.legend(title='Processos')
plt.grid(True)
plt.savefig('mpi_speedup_vs_N.png')
plt.close()

# 5) Eficiência vs N
# Eficiência = Speedup / p
plt.figure()
for p in procs:
    sub = df[df['procs'] == p].set_index('n').loc[Ns]
    speedup = t1.loc[Ns] / sub['tempo_total']
    eff = speedup / p
    plt.plot(Ns, eff, marker='x', label=f'{p} proc')
plt.title('MPI: Eficiência x N')
plt.xlabel('Tamanho do array (N)')
plt.ylabel('Eficiência')
plt.legend(title='Processos')
plt.grid(True)
plt.savefig('mpi_efficiency_vs_N.png')
plt.close()

# 6) Overhead vs N
plt.figure()
for p in procs:
    sub = df[df['procs'] == p].set_index('n').loc[Ns]
    plt.plot(Ns, sub['overhead'], marker='s', label=f'{p} proc')
plt.title('MPI: Overhead de Comunicação x N')
plt.xlabel('Tamanho do array (N)')
plt.ylabel('Overhead (%)')
plt.legend(title='Processos')
plt.grid(True)
plt.savefig('mpi_overhead_vs_N.png')
plt.close()

print("MPI plots gerados:")
print(" - mpi_tempo_vs_N.png")
print(" - mpi_speedup_vs_N.png")
print(" - mpi_efficiency_vs_N.png")
print(" - mpi_overhead_vs_N.png")
