// Università di Ferrara
// Soluzione "A03-Pasathon"
// Complessità in tempo: O(MNK)
// Complessità in spazio: O(1) memoria aggiuntiva

#include <stdio.h>
#include <stdlib.h>

// Usiamo gli unsigned long long int per gestire numeri fino a 2^64-1
#define ull unsigned long long int

// Inizializza `grid` ad una matrice `MxN` di valori `val`
void init_grid(ull **grid, size_t M, size_t N, ull val)
{
    for (size_t i = 0; i < M; i++) {
        for (size_t j = 0; j < N; j++) {
            grid[i][j] = val;
        }
    }
}

// Accumula i valori in `grid`
ull sum(ull **grid, size_t M, size_t N)
{
    ull count = 0;
    
    for (size_t i = 0; i < M; i++) {
        for (size_t j = 0; j < N; j++) {
            count += grid[i][j];
        }   
    }

    return count;
}

// Accumula i valori adiacenti del generico elemento `grid[i][j]`, 
// dove `grid` è una griglia di dimensione `MxN`.
ull adjsum(ull **grid, size_t M, size_t N, int i, int j)
{
    ull count = 0;

    for (int x=i-1; x<=i+1; x++) {
        for (int y=j-1; y<=j+1; y++) {
            if (x == i && y == j)
                continue;
            
            if (x<0 || x>=M || y<0 || y>=N)
                continue;
            
            count += grid[x][y];
        }
    }

    return count;
}

// Ciclo per aggiornare `grid` con le nuove risposte di un sottoproblema
// (vedi descrizione in `main`).
void simulate(ull **grid, size_t M, size_t N)
{
   // Crea una matrice temporanea per segnare i nuovi valori che deve avere `grid`
    ull dp[M][N];

   // Calcola i nuovi valori che deve avere `grid`, salvandoli in `dp`
    for (size_t i = 0; i < M; i++) {
        for (size_t j = 0; j < N; j++) {
            dp[i][j] = adjsum(grid, M, N, i, j);
        }
    }
 
   // Riversa i valori di `dp` in `grid`
    for (size_t i = 0; i < M; i++) {
        for (size_t j = 0; j < N; j++) {
            grid[i][j] = dp[i][j];
        }    
    }
}

int main(void)
{
    FILE *input_file, *output_file;
    input_file = fopen("input.txt", "r");
    output_file = fopen("output.txt", "w");

    if (input_file == NULL || output_file == NULL) {
        return -1;
    }

    // Prendi l'input
    size_t M, N;
    int K;
    fscanf(input_file, "%zu %zu %d", &M, &N, &K);

    // Crea una griglia MxN
    ull **grid = (ull **)malloc(M * sizeof(ull *));
    for (size_t i = 0; i < M; i++) {
        grid[i] = (ull *)malloc(N * sizeof(ull));
    }
    init_grid(grid, M, N, 1);

    // Considera il sottoproblema in cui il livello di sobrietà è k=1:
    // la chiamata a `simulate` aggiorna i valori nelle celle di `grid`,
    // in modo che ogni cella contenga il numero di visite possibili alle osterie vicine.
    //
    // Gli angoli avranno sempre valore 3, la celle senza bordi attorno 8, e le celle
    // con un bordo attorno 5 (vedi esempio nel pdf).
    //
    // Ottenuta la matrice per k=1, puoi usarla per risolvere facilmente il problema
    // nel caso k=2, e poi k=3, e via così fino a k=K;
    // in altre parole vado iterativamente avanti fino a consumare tutti i punti sobrietà
    for (size_t i = 0; i < K; i++) {
        simulate(grid, M, N);
    }

    // Sputa fuori la risposta
    fprintf(output_file, "%llu", sum(grid, M, N));

    // Dealloca
    for (size_t i = 0; i < M; i++) {
        free(grid[i]);
    }
    free(grid);
    
    if (fclose(input_file) != 0 || fclose(output_file)) {
        return -2;
    }

    return 0;
}