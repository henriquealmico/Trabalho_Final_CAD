# Varia N, threads e políticas de schedule, coleta tempo médio e gera CSV.

# parâmetros
ARRAYS=(1000 5000 10000 50000 100000)
THREADS=(1 2 4 8)
SCHEDS=(static dynamic guided)
CHUNK=1000
REPS=3
OUT=openmp_data.csv

echo "n,threads,schedule,chunk,tempo" > $OUT
gcc -O2 -fopenmp -o odd_even_openmp odd_even_openmp.c

for n in "${ARRAYS[@]}"; do
  for sch in "${SCHEDS[@]}"; do
    for t in "${THREADS[@]}"; do
      echo "=== N=$n, sched=$sch, threads=$t ==="
      total=0
      for i in $(seq 1 $REPS); do
        t_run=$(./odd_even_openmp $n $t $sch $CHUNK | awk -F, '{print $4}')
        echo "  run #$i: $t_run s"
        total=$(awk "BEGIN {print $total + $t_run}")
      done
      avg=$(awk "BEGIN {printf \"%.6f\", $total / $REPS}")
      echo "$n,$t,$sch,$CHUNK,$avg" >> $OUT
    done
  done
done

echo "Result saved to $OUT"
