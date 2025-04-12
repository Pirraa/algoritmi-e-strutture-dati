#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/**
 * TODO: scrivi commento
 */
#define MAX_ALGO 16
#define MAX_ALGO_BUFF 64

/**
 * Lunghezza k sotto il quale conviene switchare da merge sort ad insertion sort 
 * ottenendo un nuovo algoritmo, chiamato `hybrid_merge_sort`.
 */
#define HYBRID_MS_K 110
#define HYBRID_QS_K 80

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

    // TODO: tieni traccia degli algoritmi con cui fare gli esperimenti

    int n_algorithms;
    char algorithms[MAX_ALGO][MAX_ALGO_BUFF];
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

    int n_algorithms = 7;
    char *algos[] = {"MERGE", "HYBRIDMERGE","QUICK","MOTQUICK","TAILQUICK","HEAPSORT", "INTROSORT"};

    config->n_algorithms = n_algorithms;
    for (int algo_idx=0; algo_idx<n_algorithms; algo_idx++) {
        strcpy(config->algorithms[algo_idx], algos[algo_idx]);
    }
}

/**
 * Implementazione di `insertion sort`.
 */
void insertion_sort(int *arr, int start, int end) {
    int key, i;

    for (int j = start; j < end+1; j++) {
        key = arr[j];
        i = j-1;

        while(i>=start && arr[i] > key) {
            arr[i+1] = arr[i];
            i--;
        }
        arr[i+1] = key;
    }
}

/**
 * Sottoprocedura di `merge_sort` per unire due range.  
 */
void merge(int *arr, int start, int mid, int end) {
    int n1 = mid - start + 1;
    int n2 = end - mid;

    int i, j, left[n1], right[n2];

    for (i=0; i<n1; i++) {
        left[i] = arr[start+i];
    }

    for (j=0; j<n2; j++) {
        right[j] = arr[mid+1+j];
    }

    i = j = 0;

    for (int k=start; k<=end; k++) {
        if (i < n1) {
            if (j < n2) {
                arr[k] = (left[i] <= right[j]) ? left[i++] : right[j++];
            } else {
                arr[k] = left[i++];
            }
        } else {
            arr[k] = right[j++];
        }
    }
}

/**
 * Implementazione di `merge sort`.
 */
void merge_sort(int *arr, int start, int end) {
    if (start < end) {
        int mid = start + (end-start) / 2;
        merge_sort(arr, start, mid);
        merge_sort(arr, mid+1, end);
        merge(arr, start, mid, end);
    }
}

/**
 * Implementazione di `merge sort` ibridato con `insertion sort`.
 */
void hybrid_merge_sort(int *arr, int start, int end) {
    if (end-start > HYBRID_MS_K) {
        int mid = start + (end-start) / 2;
        hybrid_merge_sort(arr, start, mid);
        hybrid_merge_sort(arr, mid+1, end);
        merge(arr, start, mid, end);
    } else {
        insertion_sort(arr, start, end);
    }
}

void swap(int *arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int partition(int *arr,int start,int end)
{
    int x=arr[end];
    int i=start-1;
    for(int j=start;j<end-1;j++)
    {
        if(arr[j]<=x)
        {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[end];
    arr[end] = temp;
    return i + 1;
}

void quick(int *arr,int start,int end)
{
    if(start<end)
    {
        int q=partition(arr,start,end);
        quick(arr,start,q-1);
        quick(arr,q+1,end);
    }
}

int median_of_three(int *arr, int a, int b, int c) {
    if (arr[a] > arr[b]) {
        if (arr[b] > arr[c])      return b;
        else if (arr[a] < arr[c]) return a;
        else                      return c;
    } else {
        if (arr[a] > arr[c])      return a;
        else if (arr[b] < arr[c]) return b;
        else                      return c;
    }
}

/**
 * Come `partition` ma il pivot viene scelto con mediana di tre valori. 
 */
int mot_partition(int *arr, int start, int end) {
    int median = median_of_three(
        arr, start, end, start + (end-start)/2);
    swap(arr, median, end);
    return partition(arr, start, end);
}

/**
 * Implementazione di `quick sort` con mediana di tre.
 */
void mot_quick_sort(int *arr, int start, int end) {
    if (start < end) {
        int mid = mot_partition(arr, start, end);
        mot_quick_sort(arr, start, mid-1);
        mot_quick_sort(arr, mid+1, end);
    }
}

void tail_quick_sort(int *arr,int start,int end)
{
    while(start<end)
    {
        int pivot=mot_partition(arr,start,end);
        //se parte sinistra più lunga della parte destra
        if(pivot-start>end-pivot)
        {
            tail_quick_sort(arr,start,pivot-1);
            start=pivot+1;
        }else{
            //se parte destra più lunga della parte sinistra
            tail_quick_sort(arr,pivot+1,end);
            end=pivot-1;
        }
    }
}

void max_heapify(int *arr,int n,int root)
{
    int largest=root;
    int left=2*root+1;
    int right=2*root+2;

    if(left<n && arr[left]>arr[largest]){
        largest=left;
    }
    if(right<n && arr[right]>arr[largest]){
        largest=right;
    }
    //se la radice ha figli più grandi scambiala e richiama maxheapify
    if(largest!=root)
    {
        swap(arr,root,largest);
        max_heapify(arr,n,largest);
    }
}

void heap_sort(int *arr,int start, int end)
{
    //lunghezza intervallo da ordinare
    int n=end-start+1;
    //questa prima parte è buildmaxheap, costruisco una maxheap
    for(int i=n/2-1;i>=0;i--)
    {
        max_heapify(arr,n,i);
    }

    for(int i=n-1;i>=0;i--)
    {
        swap(arr,start,start+i);
        max_heapify(arr,i,start);
    }

}

void _intro_sort(int *arr,int start,int end,int max_dep)
{
    if(end-start<=HYBRID_QS_K)
        insertion_sort(arr,start,end);
    else if(max_dep==0)
        heap_sort(arr,start,end);
    else{
        while(start<end)
        {
            int mid=mot_partition(arr,start,end);
            if(mid-start>end-mid)
                _intro_sort(arr,start,mid-1,max_dep-1);
            else
                _intro_sort(arr,mid+1,end,max_dep-1);
        }
    }
}

void intro_sort(int *arr,int start,int end)
{
    return _intro_sort(arr,start,end,2*log2(end-start+1));
}

/**
    Funzione antagonista che controlla se un array `arr` di dimensione `size` è ordinato.
*/
int check(int *arr, int size) {
    for (int i=1; i<size; i++) {
        if (arr[i] < arr[i-1]) {
            return -1;
        }
    }

    return 1;
}

/**
 * TODO: scrivere commento
 */
typedef void (*algo_ptr)(int*, int, int);

/**
 * TODO: aggiungi commento
 */
algo_ptr select_algorithm(char *algo_name) {
    if (strcmp(algo_name, "INSERTION") == 0) {
        return &insertion_sort;
    } else if(strcmp(algo_name, "MERGE") == 0) {
        return &merge_sort;
    } else if(strcmp(algo_name, "HYBRIDMERGE") == 0) {
        return &hybrid_merge_sort;
    } else if(strcmp(algo_name, "QUICK") == 0) {
        return &quick;
    } else if(strcmp(algo_name, "MOTQUICK") == 0) {
        return &mot_quick_sort;
    } else if(strcmp(algo_name, "TAILQUICK") == 0) {
        return &tail_quick_sort;
    } else if(strcmp(algo_name, "HEAPSORT") == 0) {
        return &heap_sort;
    } else if(strcmp(algo_name, "INTROSORT") == 0) {
        return &intro_sort;
    } else {
        printf("Errore - l'algoritmo selezionato non è disponibile.");
        exit(-1);
    }
}

/**
 * Esegue un numero di misurazioni di tempi pari a `repetitions`.
 * Ogni misurazione riguarda l'applicazione dell'algoritmo `algorithm` 
 * (vedi `select_algorithm`) di ordinamento ad un array di dimensione `size`, 
 * inizializzato con numeri pseudocasuali.
 * Restituisce un valore `double`, contenente il tempo medio di esecuzione.
 */
double run(int size, int repetitions, algo_ptr algorithm) {
    double elapsed_time = 0.0;

    for (int i=0; i<repetitions; i++) {
        int arr[size];

        for (int j=0; j<size; j++) {
            arr[j] = rand();
        }

        clock_t start, end;
        start = clock();

        algorithm(arr, 0, size-1);

        end = clock();

        if (!check(arr, size)) {
            printf("Errore: ordinamento non corretto per dimensione %d.", size);
            exit(-1);
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
void run_experiments(struct configuration config, FILE *report)
{
    // stampo l'asse delle x sul file di report
    for (int i=config.min_size; i<=config.max_size; i+=config.step) {
        fprintf(report, "%d ", i);
    }
    fprintf(report, "\n");

    // teniamo traccia del seed che è stato inserito dall'utente
    int initial_seed = config.seed;

    // ripeti tutti gli esperimenti per ogni algoritmo caricato nella configurazione
    for (int algo_idx=0; algo_idx<config.n_algorithms; algo_idx++) {

        // carichiamo il seed salvato in precedenza, in modo da partire dallo stesso seed
        // con ogni algoritmo di ordinamento
        int mutable_seed = initial_seed;

        // l'iteratore `i` scorre sulle varia dimensioni;
        // essenzialmente è "un'indice che si focalizza su una x precisa
        // del plot che disegneremo";
        // per ogni x (cioè per ognuna di queste i) calcoleremo una y
        // (ovvero i tempi corrispondenti).
        for (int i=config.min_size; i<=config.max_size; i+=config.step) {
            srand(mutable_seed);
            
            double elapsed_time = run(
                i, 
                config.repetitions, 
                select_algorithm(config.algorithms[algo_idx])
            );
            
            ++mutable_seed;
    
            fprintf(report, "%g ", elapsed_time);
        }
        fprintf(report, "\n");
    }

}

int main(void) 
{
    // carichiamo la configurazione degli esperimenti da stdin
    struct configuration config;
    init(&config);

    // creiamo un file su cui stampare i risultati
    FILE *report;
    report = fopen("report.txt", "w");
    if (report == NULL) {
        printf("Errore durante l'apertura del file.");
        return -1;
    }

    // logica principale degli esperimenti
    run_experiments(config, report);

    if (fclose(report) != 0) {
        printf("Errore durante la chiusura del file.");
        return -1;
    }

    return 0;
}