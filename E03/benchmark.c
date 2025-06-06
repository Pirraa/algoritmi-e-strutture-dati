#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// Funzioni del Segment Tree (ottimizzate)
void build(long long arr[], long long tree[], int node, int start, int end) {
    if (start == end) {
        tree[node] = arr[start];  
    } else {
        int mid = (start + end) / 2;
        build(arr, tree, 2 * node, start, mid);
        build(arr, tree, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
}

void propagate(long long tree[], long long lazy[], int node, int start, int end) {
    if (lazy[node] != 0) {
        tree[node] += (long long)(end - start + 1) * lazy[node];
        if (start != end) {
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        lazy[node] = 0;
    }
}

void update_range(long long tree[], long long lazy[], int node, int start, int end, int l, int r, long long val) {
    propagate(tree, lazy, node, start, end);
    if (r < start || end < l) return; 
    if (l <= start && end <= r) {
        lazy[node] += val;
        propagate(tree, lazy, node, start, end);
        return;
    }
    int mid = (start + end) / 2;
    update_range(tree, lazy, 2 * node, start, mid, l, r, val);
    update_range(tree, lazy, 2 * node + 1, mid + 1, end, l, r, val);
    //propagate(tree, lazy, 2 * node, start, mid);
    //propagate(tree, lazy, 2 * node + 1, mid + 1, end);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

long long Query(long long tree[], long long lazy[], int node, int start, int end, int l, int r) {
    propagate(tree, lazy, node, start, end);
    if (r < start || end < l) return 0; 
    if (l <= start && end <= r) return tree[node];
    int mid = (start + end) / 2;
    //long long left_sum = Query(tree, lazy, 2 * node, start, mid, l, r);
    //long long right_sum = Query(tree, lazy, 2 * node + 1, mid + 1, end, l, r);
    //return left_sum + right_sum;
    return Query(tree, lazy, 2 * node, start, mid, l, r) +
           Query(tree, lazy, 2 * node + 1, mid + 1, end, l, r);
}

// Funzione per ottenere il tempo in millisecondi
double get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

void cleanup(long long *arr, long long *tree, long long *lazy, FILE *input, FILE *output) {
    if (arr) free(arr);
    if (tree) free(tree);
    if (lazy) free(lazy);
    if (input) fclose(input);
    if (output) fclose(output);
}

// Generatore di test integrato
void generate_test(int num_celle, int num_query) {
    printf("=== GENERAZIONE TEST ===\n");
    printf("Elementi: %d, Query: %d\n", num_celle, num_query);
    
    double start_time = get_time_ms();
    
    FILE *input = fopen("input.txt", "w");
    if (!input) {
        printf("Errore nell'apertura del file input.txt\n");
        return;
    }
    
    srand(12345); // Seed fisso per risultati riproducibili
    
    fprintf(input, "%d %d\n", num_celle, num_query);
    
    // Genera array
    for (int i = 0; i < num_celle; i++) {
        long long val = rand() % 1000 + 1;
        fprintf(input, "%lld", val);
        if (i < num_celle - 1) fprintf(input, " ");
    }
    fprintf(input, "\n");
    
    // Genera query
    for (int i = 0; i < num_query; i++) {
        if (rand() % 100 < 60) {  // 60% update, 40% query
            int l = rand() % num_celle;
            int r = l + rand() % (num_celle - l);
            long long val = (rand() % 201) - 100;
            fprintf(input, "A %d %d %lld\n", l, r, val);
        } else {
            int l = rand() % num_celle;
            int r = l + rand() % (num_celle - l);
            fprintf(input, "Q %d %d\n", l, r);
        }
    }
    
    fclose(input);
    
    double gen_time = get_time_ms() - start_time;
    printf("Test generato in %.2f ms\n", gen_time);
    printf("Dimensione file: ~%.1f MB\n\n", 
           (num_celle * 8 + num_query * 20) / (1024.0 * 1024.0));
}

// Esegue il benchmark
void run_benchmark() {
    printf("=== ESECUZIONE BENCHMARK ===\n");
    
    double start_time = get_time_ms();
    
    FILE *input = NULL;
    FILE *output = NULL;
    long long *arr = NULL;
    long long *tree = NULL;
    long long *lazy = NULL;

    input = fopen("input.txt", "r");
    if (!input) {
        printf("Errore: impossibile aprire input.txt\n");
        return;
    }
    
    output = fopen("output.txt", "w");
    if (!output) {
        printf("Errore: impossibile creare output.txt\n");
        fclose(input);
        return;
    }

    int num_celle, num_query;
    if (fscanf(input, "%d %d", &num_celle, &num_query) != 2) {
        printf("Errore nella lettura dei parametri\n");
        cleanup(NULL, NULL, NULL, input, output);
        return;
    }

    printf("Processando %d elementi e %d query...\n", num_celle, num_query);

    // Allocazione memoria
    arr = malloc(num_celle * sizeof(long long));
    tree = calloc(4 * num_celle, sizeof(long long));
    lazy = calloc(4 * num_celle, sizeof(long long));
    
    if (!arr || !tree || !lazy) {
        printf("Errore nell'allocazione della memoria\n");
        cleanup(arr, tree, lazy, input, output);
        return;
    }

    double alloc_time = get_time_ms();
    printf("Memoria allocata in %.2f ms\n", alloc_time - start_time);

    // Lettura array
    for (int i = 0; i < num_celle; i++) {
        if (fscanf(input, "%lld", &arr[i]) != 1) {
            printf("Errore nella lettura dell'elemento %d\n", i);
            cleanup(arr, tree, lazy, input, output);
            return;
        }
    }

    double read_time = get_time_ms();
    printf("Array letto in %.2f ms\n", read_time - alloc_time);

    // Build segment tree
    build(arr, tree, 1, 0, num_celle - 1);

    double build_time = get_time_ms();
    printf("Segment tree costruito in %.2f ms\n", build_time - read_time);

    // Buffer per I/O più veloce
    setvbuf(output, NULL, _IOFBF, 8192);
    
    int updates = 0, queries = 0;
    
    // Process query
    for (int i = 0; i < num_query; i++) {
        char query_type;
        if (fscanf(input, " %c", &query_type) != 1) {
            printf("Errore nella lettura della query %d\n", i);
            cleanup(arr, tree, lazy, input, output);
            return;
        }

        if (query_type == 'A') {
            int l, r;
            long long val;
            if (fscanf(input, "%d %d %lld", &l, &r, &val) != 3) {
                printf("Errore nei parametri della query A %d\n", i);
                cleanup(arr, tree, lazy, input, output);
                return;
            }
            update_range(tree, lazy, 1, 0, num_celle - 1, l, r, val);
            updates++;
        }
        else if (query_type == 'Q') {
            int l, r;
            if (fscanf(input, "%d %d", &l, &r) != 2) {
                printf("Errore nei parametri della query Q %d\n", i);
                cleanup(arr, tree, lazy, input, output);
                return;
            }
            long long result = Query(tree, lazy, 1, 0, num_celle - 1, l, r);
            fprintf(output, "%lld\n", result);
            queries++;
        }
        
        // Progress indicator per test lunghi
        if ((i + 1) % 10000 == 0) {
            printf("Processate %d/%d query (%.1f%%)\n", 
                   i + 1, num_query, 100.0 * (i + 1) / num_query);
        }
    }
    
    double total_time = get_time_ms() - start_time;
    double query_time = get_time_ms() - build_time;
    
    printf("\n=== RISULTATI BENCHMARK ===\n");
    printf("Tempo totale: %.2f ms (%.3f s)\n", total_time, total_time / 1000.0);
    printf("Tempo costruzione: %.2f ms\n", build_time - start_time);
    printf("Tempo esecuzione query: %.2f ms\n", query_time);
    printf("Updates eseguiti: %d\n", updates);
    printf("Query eseguite: %d\n", queries);
    printf("Velocità media: %.2f operazioni/ms\n", num_query / query_time);
    printf("Memoria utilizzata: ~%.1f MB\n", 
           (num_celle * 3 * sizeof(long long)) / (1024.0 * 1024.0));
    
    if (total_time > 1000) {
        printf("⚠️  ATTENZIONE: Tempo > 1 secondo!\n");
    } else {
        printf("✅ Test completato entro 1 secondo\n");
    }
    
    cleanup(arr, tree, lazy, input, output);
}

int main() {
    printf("=== BENCHMARK SEGMENT TREE ===\n\n");
    
    // Test con diverse dimensioni
    int test_sizes[][2] = {
        {10000, 10000},    // Small: 10K elementi, 10K query
        {50000, 50000},    // Medium: 50K elementi, 50K query  
        {100000, 100000},  // Large: 100K elementi, 100K query
        {100000, 1000000}   // Extra Large: 1M elementi, 1M query
    };
    
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);
    
    for (int t = 0; t < num_tests; t++) {
        printf("=== TEST %d/%d ===\n", t + 1, num_tests);
        
        generate_test(test_sizes[t][0], test_sizes[t][1]);
        run_benchmark();
        
        printf("\n");
        if (t < num_tests - 1) {
            printf("Premi Invio per continuare con il test successivo...\n");
            getchar();
        }
    }
    
    return 0;
}