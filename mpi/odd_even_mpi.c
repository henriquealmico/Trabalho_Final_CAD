#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int tmp = *a; *a = *b; *b = tmp;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank==0) fprintf(stderr, "Uso: %s <tamanho_array>\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    int n = atoi(argv[1]);

    // calcular distribuição
    int base = n / size;
    int rem  = n % size;
    int local_n = base + (rank < rem ? 1 : 0);
    int *local = malloc(local_n * sizeof(int));
    int *full = malloc(n * sizeof(int));

    // root inicializa array completo
    int *arr = NULL;
    if (rank == 0) {
        arr = malloc(n * sizeof(int));
        srand(0);
        for (int i = 0; i < n; i++) arr[i] = rand() % 100000;
    }

    // montar counts e displs
    int *counts = malloc(size * sizeof(int));
    int *displs  = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        counts[i] = base + (i < rem ? 1 : 0);
        displs[i] = i*base + (i < rem ? i : rem);
    }

    // scatter local
    MPI_Scatterv(arr, counts, displs, MPI_INT,
                 local, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();
    double comm_time = 0.0;

    int left  = rank - 1;
    int right = rank + 1;
    for (int phase = 0; phase < n; phase++) {
        int even = (phase % 2 == 0);
        int start = even ? 1 : 0;
        // ordenação local
        for (int i = start; i + 1 < local_n; i += 2) {
            if (local[i] > local[i+1]) swap(&local[i], &local[i+1]);
        }
        // comunicação de fronteira
        int neighbor = even ? right : left;
        if (neighbor >= 0 && neighbor < size) {
            int send_val = even ? local[local_n-1] : local[0];
            int recv_val;
            double c0 = MPI_Wtime();
            MPI_Sendrecv(&send_val, 1, MPI_INT, neighbor, 0,
                         &recv_val, 1, MPI_INT, neighbor, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            comm_time += MPI_Wtime() - c0;
            if (even) {
                if (recv_val < local[local_n-1]) local[local_n-1] = recv_val;
            } else {
                if (recv_val > local[0]) local[0] = recv_val;
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t_total = MPI_Wtime() - t_start;

    // gather final
    MPI_Allgatherv(local, local_n, MPI_INT,
                   full, counts, displs, MPI_INT,
                   MPI_COMM_WORLD);

    // reduzir e imprimir
    double max_total, sum_comm;
    MPI_Reduce(&t_total, &max_total, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&comm_time, &sum_comm,  1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        double overhead = (sum_comm / max_total) * 100.0;
        printf("MPI,%d,%.6f,%.6f,%.2f%%\n", size, max_total, sum_comm, overhead);
    }

    free(local); free(counts); free(displs);
    if (rank==0) { free(arr); free(full); }
    MPI_Finalize();
    return 0;
}