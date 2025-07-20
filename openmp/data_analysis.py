import pandas as pd
import matplotlib.pyplot as plt

# 1) ler dados
df = pd.read_csv('openmp_data.csv')

# 2) gráfico de tempo vs threads (para cada N e cada schedule)
for n, group_n in df.groupby('n'):
    plt.figure()
    for sch, grp in group_n.groupby('schedule'):
        plt.plot(grp['threads'], grp['tempo'], marker='o', label=sch)
    plt.title(f'Tempo x Threads (N={n})')
    plt.xlabel('Threads')
    plt.ylabel('Tempo médio (s)')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'tempo_N{n}.png')

# 3) gráfico de speedup vs threads
# Speedup = T(1) / T(p) para cada N e schedule
plt.figure()
for (n, sch), grp in df.groupby(['n','schedule']):
    t1 = grp[grp.threads == 1]['tempo'].values[0]
    plt.plot(grp['threads'], t1 / grp['tempo'], marker='^',
             label=f'N={n}, {sch}')
plt.title('Speedup x Threads')
plt.xlabel('Threads')
plt.ylabel('Speedup')
plt.legend(fontsize='small')
plt.grid(True)
plt.savefig('speedup_all.png')

# 4) gráfico de eficiência vs threads
# Eficiência = Speedup / p
plt.figure()
for (n, sch), grp in df.groupby(['n','schedule']):
    t1 = grp[grp.threads == 1]['tempo'].values[0]
    speedup = t1 / grp['tempo']
    eff = speedup / grp['threads']
    plt.plot(grp['threads'], eff, marker='x',
             label=f'N={n}, {sch}')
plt.title('Eficiência x Threads')
plt.xlabel('Threads')
plt.ylabel('Eficiência')
plt.legend(fontsize='small')
plt.grid(True)
plt.savefig('efficiency_all.png')

print("Plots gerados: tempo_N*.png, speedup_all.png, efficiency_all.png")
