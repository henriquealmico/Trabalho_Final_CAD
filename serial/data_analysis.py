import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('serial/serial_data.csv')

plt.figure()
plt.plot(df['n'], df['tempo'], marker='o')
plt.title('Serial: Tempo x N')
plt.xlabel('Tamanho do array (N)')
plt.ylabel('Tempo (s)')
plt.grid(True)
plt.savefig('serial/serial_tempo_vs_N.png')
plt.close()

print('Gráfico serial gerado em serial/serial_tempo_vs_N.png')
