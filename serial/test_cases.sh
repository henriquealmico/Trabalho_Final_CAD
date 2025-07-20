ARRAYS=(1000 5000 10000 50000 100000)
REPS=3
SRC=odd_even_serial.c
BIN=odd_even_serial
OUT=serial_data.csv

# Compila o serial
gcc -O2 -Wall -o ./$BIN $SRC || { echo "Erro de compilação"; exit 1; }

# Cabeçalho CSV
echo "n,tempo" > $OUT

# Loop principal
for n in "${ARRAYS[@]}"; do
  sum=0
  for i in $(seq 1 $REPS); do
    # Executa garantindo o caminho relativo
    line=$(./$BIN $n 2>/dev/null | tail -1)
    # Se não vier no formato esperado, assume zero
    t=$(echo "$line" | grep -Eo '[0-9]+\.[0-9]+' || echo 0)
    echo "N=$n, run #$i: $t s"
    sum=$(awk "BEGIN {print $sum + $t}")
  done
  avg=$(awk "BEGIN {printf \"%.6f\", $sum / $REPS}")
  echo "$n,$avg" >> $OUT
done

echo "Resultados em $OUT"
