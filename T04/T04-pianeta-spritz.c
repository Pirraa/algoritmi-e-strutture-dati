// Attenzione, compilare con -lm per linkare math.h
// ad esempio: gcc pianeta-spritz.c -lm

#include <math.h>  
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 2.2204460492503131e-16

/**
 * ATTENZIONE
 * Questa funzione viene passata come argomento a `qsort` in `pianeta_spritz`.
 * Puoi ignorare questa funzione, semplicemente sostituisci `qsort` in `pianeta_spritz` 
 * con un altro algoritmo di ordinamento visto nel corso (merge sort o quick sort) per 
 * ordinare l'array delle distance `distance`.
 */
int is_equal(double a, double b) {
    if (fabs(a - b) <= 1000 * EPSILON * fabs(a + b)) {
        return 1;
    }
    return 0;
}

/**
 * ATTENZIONE
 * Vale lo stesso messaggio di `is_equal`.
 */
int compare(const void *x, const void *y) {
    double a = *(double *)x;
    double b = *(double *)y;

    if (is_equal(a,b)) {
        return 0;
    }

    // a is bigger
    if((a - b) >
        ((fabs(a) < fabs(b) ? fabs(b) :
        fabs(a)) * EPSILON * 1000)) {
        return 1;
    }

    // b is bigger
    return -1;
}

/**
 * @brief Implementazione della binary search
 * 
 * @param distance array di distanze (deve essere ordinato in ordine crescente)
 * @param N dimensione di `distance`
 * @param radius raggio della sfera, ovvero una singola query (leggi il problema)
 * @return int posizione dell'ultima distanza valida entro la sfera
 */
int binary_search(double *distance, int N, double radius) {
    int left = 0;
    int right = N;
    int mid;

    while (left < right) {
        mid = left + (right - left) / 2;

        if (distance[mid] <= radius) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return left;
}

void pianeta_spritz(FILE *in_file, FILE *out_file) {
    int N, Q;
    if (fscanf(in_file, "%d %d", &N, &Q) != 2) {
        fprintf(stderr, "Error reading N and Q from input file.\n");
        exit(EXIT_FAILURE);
    }
    
    double distance[N];

    // per ogni coordinata in input, calcola la sua distanza da (0,0,0);
    // attenzione, ogni coordinata può essere fino a 2^30 quindi usare int non ci basta.
    for (size_t i=0; i<N; i++) {
        long long int x,y,z;
        if (fscanf(in_file, "%lld %lld %lld", &x, &y, &z) != 3) {
            fprintf(stderr, "Error reading coordinates from input file.\n");
            exit(EXIT_FAILURE);
        }
        distance[i] = sqrt(x*x + y*y + z*z);
    }

    // ordina l'array delle distanze;
    // puoi sostituire `qsort` della libreria standard, con un algoritmo implementato da te
    qsort(distance, N, sizeof(distance[0]), compare);

    // rispondi ad ogni query con una binary search
    for (int i=0; i<Q; i++) {
        int query;
        if (fscanf(in_file, "%d", &query) != 1) {
            fprintf(stderr, "Error reading query from input file.\n");
            exit(EXIT_FAILURE);
        }
        fprintf(out_file, "%d\n", binary_search(distance, N, (double)query));
    }
}

int main(void) {
    FILE *in_file = fopen("input.txt", "r");
    FILE *out_file = fopen("output.txt", "w");

    pianeta_spritz(in_file, out_file);

    return 0;
}

