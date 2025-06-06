#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    
    FILE *input = fopen("input.txt", "w");
    if (!input) {
        printf("Errore nell'apertura del file input.txt\n");
        return 1;
    }
    // Parametri del test
    int num_celle = 10000;    // 10K elementi
    int num_query = 1000000;    // 1M operazioni

    printf("Generando test con %d celle e %d query...\n", num_celle, num_query);
    
    // Scrivi header
    fprintf(input, "%d %d\n", num_celle, num_query);
    
    // Genera array iniziale con valori casuali
    for (int i = 0; i < num_celle; i++) {
        long long val = rand() % 1000 + 1;  // Valori da 1 a 1000
        fprintf(input, "%lld", val);
        if (i < num_celle - 1) fprintf(input, " ");
    }
    fprintf(input, "\n");
    
    // Genera query miste (70% aggiornamenti, 30% query)
    for (int i = 0; i < num_query; i++) {
        if (rand() % 100 < 70) {  // 70% probabilità di update
            // Query di aggiornamento: A l r val
            int l = rand() % num_celle;
            int r = l + rand() % (num_celle - l);  // r >= l
            long long val = (rand() % 201) - 100;  // Valori da -100 a +100
            fprintf(input, "A %d %d %lld\n", l, r, val);
        } else {
            // Query di somma: Q l r
            int l = rand() % num_celle;
            int r = l + rand() % (num_celle - l);  // r >= l
            fprintf(input, "Q %d %d\n", l, r);
        }
    }
    
    fclose(input);
    printf("Test generato con successo in input.txt\n");
    printf("Dimensione approssimativa: ~%.1f MB\n", 
           (num_celle * 8 + num_query * 20) / (1024.0 * 1024.0));
    
    return 0;
}