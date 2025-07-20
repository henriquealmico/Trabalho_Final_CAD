#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void odd_even_sort_serial(int arr[], int n) {
    int phase, i;
    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            // Fase par: compara (i-1, i) para i ímpar
            for (i = 1; i < n; i += 2) {
                if (arr[i-1] > arr[i]) {
                    swap(&arr[i-1], &arr[i]);
                }
            }
        } else {
            // Fase ímpar: compara (i, i+1) para i ímpar
            for (i = 1; i < n-1; i += 2) {
                if (arr[i] > arr[i+1]) {
                    swap(&arr[i], &arr[i+1]);
                }
            }
        }
    }
}

void generate_random_array(int arr[], int n, int max_val) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % max_val;
    }
}

int is_sorted(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        if (arr[i] > arr[i+1]) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <tamanho_array>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    int *arr = malloc(n * sizeof(int));
    if (!arr) { perror("malloc"); return 1; }

    // Gerar array aleatório e mostrar (até 20 elementos)
    generate_random_array(arr, n, 1000);
    if (n <= 20) {
        for (int i = 0; i < n; i++) printf("%d ", arr[i]);
        printf("\n");
    } else {
        for (int i = 0; i < 20; i++) printf("%d ", arr[i]);
        printf("... (exibindo apenas os 20 primeiros elementos)\n");
    }

    // Medição de tempo serial
    clock_t t0 = clock();
    odd_even_sort_serial(arr, n);
    clock_t t1 = clock();
    double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;

    // Mostrar resultado (até 20 elementos) e se está ordenado
    if (n <= 20) {
        for (int i = 0; i < n; i++) printf("%d ", arr[i]);
        printf("\n");
    } else {
        for (int i = 0; i < 20; i++) printf("%d ", arr[i]);
        printf("... (exibindo apenas os 20 primeiros elementos)\n");
    }
    printf("Array está ordenado: %s\n", is_sorted(arr, n) ? "Sim" : "Não");

    // Para coleta de dados automatizada imprimimos CSV: n,tempo
    printf("%d,%.6f\n", n, elapsed);

    free(arr);
    return 0;
}
