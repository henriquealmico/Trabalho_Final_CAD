ARRAYS=(1000 5000 10000 50000 100000)
PROCS=(1 2 4 8)
REPS=3
OUT=mpi_varied.csv

echo "n,procs,tempo_total,comm_time,overhead" > $OUT

mpicc -O2 -o odd_even_mpi odd_even_mpi.c || { echo "MPI compilation failed"; exit 1; }

for n in "${ARRAYS[@]}"; do
  for p in "${PROCS[@]}"; do
    echo "=== Testing N=$n with $p processes ==="
    sum_total=0
    sum_comm=0
    sum_ov=0
    valid_runs=0

    for run in $(seq 1 $REPS); do
      echo -n "  Run #$run: "
      # Launch with mpiexec -n (MPICH style)
      line=$(mpiexec -n $p ./odd_even_mpi $n 2>/dev/null)
      if [ $? -ne 0 ] || [[ -z "$line" ]]; then
        echo "FAILED"
        continue
      fi

      # Expect line = MPI,<procs>,<t_total>,<comm_time>,<overhead>%
      echo "$line"
      IFS=',' read -r tag procs t_total comm ovp <<<"$line"
      ov=${ovp%\%}

      sum_total=$(awk "BEGIN {print $sum_total + $t_total}")
      sum_comm=$(awk "BEGIN {print $sum_comm + $comm}")
      sum_ov=$(awk "BEGIN {print $sum_ov + $ov}")
      valid_runs=$((valid_runs+1))
    done

    if [ "$valid_runs" -gt 0 ]; then
      avg_total=$(awk "BEGIN {printf \"%.6f\", $sum_total / $valid_runs}")
      avg_comm=$(awk "BEGIN {printf \"%.6f\", $sum_comm / $valid_runs}")
      avg_ov=$(awk "BEGIN {printf \"%.2f\", $sum_ov / $valid_runs}")
      echo "$n,$p,$avg_total,$avg_comm,$avg_ov" >> $OUT
    else
      echo "  No successful runs for N=$n, p=$p; skipping."
    fi
  done
done

echo "MPI results saved to $OUT"
