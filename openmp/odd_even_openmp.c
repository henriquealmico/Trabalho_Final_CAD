/*
 * odd_even_openmp.c
 *
 * Implementação paralela do algoritmo Odd–Even Transposition Sort usando OpenMP
 *
 * Uso: ./odd_even_openmp <tamanho_array> <num_threads> <tipo_schedule> <chunk_size>
 *   tamanho_array   : número de elementos a ordenar
 *   num_threads     : número de threads OpenMP
 *   tipo_schedule   : static | dynamic | guided
 *   chunk_size      : tamanho do chunk para a política de scheduling
 *
 * Exemplo de compilação:
 *   gcc -O2 -fopenmp -o odd_even_openmp odd_even_openmp.c
 *
 * Exemplo de execução:
 *   ./odd_even_openmp 100000 4 dynamic 1000
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void generate_random_array(int *arr, int n, int max_val) {
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) arr[i] = rand() % max_val;
}

int is_sorted(int *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        if (arr[i] > arr[i + 1]) return 0;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <tamanho_array> <num_threads> <tipo_schedule> <chunk_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    char *schedule_type = argv[3];
    int chunk = atoi(argv[4]);

    int *arr = malloc(n * sizeof(int));
    if (!arr) { perror("malloc"); return EXIT_FAILURE; }
    generate_random_array(arr, n, 100000);

    // configurar política de scheduling via runtime
    omp_sched_t kind;
    if (strcmp(schedule_type, "static") == 0)      kind = omp_sched_static;
    else if (strcmp(schedule_type, "dynamic") == 0) kind = omp_sched_dynamic;
    else if (strcmp(schedule_type, "guided") == 0)  kind = omp_sched_guided;
    else {
        fprintf(stderr, "Schedule inválido. Use static, dynamic ou guided.\n");
        return EXIT_FAILURE;
    }
    omp_set_schedule(kind, chunk);

    omp_set_num_threads(num_threads);
    double start = omp_get_wtime();

    // Odd–Even Transposition Sort paralelo
    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            #pragma omp parallel for schedule(runtime)
            for (int i = 1; i < n; i += 2)
                if (arr[i-1] > arr[i]) swap(&arr[i-1], &arr[i]);
        } else {
            #pragma omp parallel for schedule(runtime)
            for (int i = 1; i < n-1; i += 2)
                if (arr[i] > arr[i+1]) swap(&arr[i], &arr[i+1]);
        }
    }

    double end = omp_get_wtime();
    printf("%d,%s,%d,%.6f\n", num_threads, schedule_type, chunk, end-start);
    // imprime CSV: threads,schedule,chunk,tempo

    // verificar ordenação
    // printf("Ordenado? %s\n", is_sorted(arr, n) ? "Sim" : "Nao");

    free(arr);
    return EXIT_SUCCESS;
}
