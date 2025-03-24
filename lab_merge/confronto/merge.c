#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define HYBRID_MERGESORT_K 110

/**
 * Parametri di un esperimento.
 *
 * min_size: dimensione dell'array nel primo esperimento.
 * max_size: dimensione dell'array nell'ultimo esperimento.
 * step: incremento di dimensione dell'array tra un esperimento e il successivo.
 * repetitions: numero di volte in cui un esperimento è ripetuto, 
 *              al fine di ottenere tempi statisticamente validi.
 * seed: seme del generatore di numeri pseudocasuali,
 *              al fine di garantire la riproducibilità degli esperimenti.
 */
struct configuration 
{
    int min_size;
    int max_size;
    int step;
    int repetitions;
    int seed;
};

/**
 * Inizializza i parametri di un esperimento.
 * Vedi anche: `struct configuration`.
 */
void init(struct configuration* config) {
    printf("Inserire min_size (ad esempio, 10): ");
    scanf("%d", &config->min_size);

    printf("Inserire max_size (ad esempio, 1000): ");
    scanf("%d", &config->max_size);

    printf("Inserire step (ad esempio, 10): ");
    scanf("%d", &config->step);

    printf("Inserire repetitions (ad esempio, 50): ");
    scanf("%d", &config->repetitions);

    printf("Inserire seed (ad esempio, 362372): ");
    scanf("%d", &config->seed);

    // TODO: controllare che i valori inseriti siano sensati
}

void merge(int arr[],int start,int mid,int end)
{
    //ar[start:mid] e arr[mid+1:end] sono ordinati
    //posso comporli in un unico array ordinato
    //mantenendo un indice indipendente per ciascuna metà
    int n1=mid-start+1;
    int n2=end-mid;
    int L[n1];
    int R[n2];
    for(int i=0;i<n1;i++) L[i]=arr[start+i];
    for(int j=0;j<n2;j++) R[j]=arr[mid+j+1];
    int i=0;
    int j=0;
    for(int k=start;k<=end;k++)
    {
        if(i<n1)
        {
            if(j<n2)
            {
                if(L[i]<=R[j])
                {
                    arr[k]=L[i];
                    i++;
                }else
                {
                    arr[k]=R[j];
                    j++;
                }
            }else
            {
                arr[k]=L[i];
                i++;
            }
        }else
        {
            arr[k]=R[j];
            j++;
        }
    }
}

void merge_sort(int arr[],int start,int end)
{
    //se l'array contiene almeno due elementi richiama ricorsivamente sulle due metà
    //dopo la chiamata, le due metà sono ordinate e possono essere unite con merge
    if(start<end)
    {
        int mid=start+(end-start)/2;
        merge_sort(arr,start,mid);
        merge_sort(arr,mid+1,end);
        merge(arr,start,mid,end);
    }
}


/**
    TODO: scrivi la funzione antagonista che controlla
    se un array `arr` è ordinato tra `start` ed `end`.
*/
int check(int *arr, int start, int end) {
    for (int i = start + 1; i < end; i++) {
        // Se un elemento è maggiore del successivo, l'array non è ordinato
        if (arr[i - 1] > arr[i]) {
            return 0;  // Array non ordinato
        }
    }
    return 1;  // Array ordinato
}

/**
 * Esegue un numero di misurazioni di tempi pari a `repetitions`.
 * Ogni misurazione riguarda l'applicazione di un certo algoritmo
 * di ordinamento ad un array di dimensione `size`, inizializzato
 * con numeri pseudocasuali.
 * Restituisce un valore `double`, contenente il tempo medio di esecuzione.
 */
double run(int size, int repetitions) {
    double elapsed_time = 0.0;

    for (int i=0; i<repetitions; i++) {
        int arr[size];

        for (int j=0; j<size; j++) {
            arr[j] = rand();
        }

        clock_t start, end;
        start = clock();
        merge_sort(arr, 0, size-1);
        end = clock();

        if (!check(arr, 0, size-1)) {
           printf("ERRORE ERRORE GRAVE GRAVE");
           return 1;
        }          
        
        // Accumulo il numero di clock trascorsi, e divido
        // per la costante `CLOCKS_PER_SEC` per portarmi
        // sulla scala dei secondi.
        elapsed_time += (end-start) / (double)CLOCKS_PER_SEC;
    }

    // Ecco che faccio la media dei tempi trascorsi
    return elapsed_time / repetitions;
}

/**
 * Manager degli esperimenti.
 * 
 * Chiama un esperimento più volte, gestendo di volta in volta
 * l'incremento della dimensione dell'array soggetto degli esperimenti.
 * 
 * Si occupa anche di inizializzare e variare il generatore di numeri
 * pseudocasuali, e di stampare a video i risultati.
 */
void run_experiments(struct configuration config)
{
    FILE *file = fopen("merge.csv", "w");
    // l'iteratore `i` scorre sulle varia dimensioni;
    // essenzialmente è "un'indice che si focalizza su una x precisa
    // del plot che disegneremo";
    // per ogni x (cioè per ognuna di queste i) calcoleremo una y
    // (ovvero i tempi corrispondenti).
    // Scrivi l'intestazione del CSV
    fprintf(file, "size,elapsed_time\n");
    for (int i=config.min_size; i<=config.max_size; i+=config.step) {
        srand(config.seed);
        
        double elapsed_time = run(i, config.repetitions);
        
        ++config.seed;
        printf("%g\n", elapsed_time);
        fprintf(file, "%d,%g\n", i, elapsed_time);
    }
    fclose(file);
}

int main(void) 
{
    struct configuration config;
    init(&config);

    run_experiments(config);

    return 0;
}