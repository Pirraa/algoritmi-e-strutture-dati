#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_N 100005
#define ull unsigned long long int

// DEFINIZIONE GRAFO (come lista di adiacenza)

typedef struct adj_list_node
{
    // chi è
    int dest;

    // quanto pesa l'arco per arrivarci
    ull weight;

    // flag per indicare se questo arco rappresenta un mulinello
    int is_whirlpool;

    // poi ci servono i riferimenti agli altri vicini
    struct adj_list_node *next;
} adj_list_node;

// crea un nodo
adj_list_node *new_adj_list_node(int dest, ull weight, int is_whirlpool)
{
    adj_list_node *new_node = (adj_list_node *)malloc(sizeof(adj_list_node));

    new_node->dest = dest;
    new_node->weight = weight;
    new_node->is_whirlpool = is_whirlpool;
    new_node->next = NULL;

    return new_node;
}

// definizione del grafo
typedef struct adj_list
{
    adj_list_node *heads[MAX_N];
} adj_list;

// crea il grafo come lista di adiacenza
adj_list *init_adj_list()
{
    adj_list *graph = (adj_list *)malloc(sizeof(adj_list));

    for (int i = 0; i < MAX_N; i++)
    {
        graph->heads[i] = NULL;
    }

    return graph;
}

// inserisci un arco (u,v) con peso w nel grafo
void insert_edge(adj_list *graph, int u, int v, ull w, int is_whirlpool)
{
    adj_list_node *new_node = new_adj_list_node(v, w, is_whirlpool);

    new_node->next = graph->heads[u];
    graph->heads[u] = new_node;
}

// elimina il grafo in memoria
void free_graph(adj_list *graph)
{
    for (int i = 0; i < MAX_N; i++)
    {
        adj_list_node *node = graph->heads[i];

        while (node)
        {
            adj_list_node *temp = node;
            node = node->next;
            free(temp);
        }
    }

    free(graph);
}

// DEFINIZIONE HEAP

// per ogni elemento nella heap, dobbiamo sapere:
typedef struct min_heap_node
{
    // chi è
    int v;

    // qual è la chiave associata
    ull weight;
} min_heap_node;

// crea un nuovo elemento nella heap
min_heap_node *init_min_heap_node(int v, ull weight)
{
    min_heap_node *heap_node = (min_heap_node *)malloc(sizeof(min_heap_node));

    heap_node->v = v;
    heap_node->weight = weight;

    return heap_node;
}

// definizione della heap
typedef struct min_heap
{
    // numero elementi contenuti nella heap
    int size;

    // array per mappare l'id di un nodo alla sua posizione nella heap
    int *pos;

    // questa è l'effettiva heap, contenente un array di puntatori a "nodi heap" (vedi sopra)
    min_heap_node **array;
} min_heap;

// crea una min heap
min_heap *init_min_heap()
{
    min_heap *heap = (min_heap *)malloc(sizeof(min_heap));

    heap->pos = (int *)malloc(MAX_N * sizeof(int));
    heap->size = 0;

    heap->array = (min_heap_node **)malloc(MAX_N * sizeof(min_heap_node *));

    return heap;
}

// piccola uility per scambiare di posizione due elementi della heap
void swap_min_heap_nodes(min_heap_node **a, min_heap_node **b)
{
    min_heap_node *temp = *a;
    *a = *b;
    *b = temp;
}

// garantisce che la heap sia min heap
void min_heapify(min_heap *heap, int idx)
{
    int smallest = idx;
    int left = idx * 2 + 1;
    int right = idx * 2 + 2;

    if (left < heap->size && heap->array[left]->weight < heap->array[smallest]->weight)
    {
        smallest = left;
    }

    if (right < heap->size && heap->array[right]->weight < heap->array[smallest]->weight)
    {
        smallest = right;
    }

    if (smallest != idx)
    {
        min_heap_node *smallest_node = heap->array[smallest];
        min_heap_node *idx_node = heap->array[idx];

        heap->pos[smallest_node->v] = idx;
        heap->pos[idx_node->v] = smallest;

        swap_min_heap_nodes(&heap->array[smallest], &heap->array[idx]);
        min_heapify(heap, smallest);
    }
}

// estrai il minimo dalla heap e riarrangia i nodi mantenendo la proprietà di min heap
min_heap_node *extract_min(min_heap *heap)
{
    if (heap->size == 0)
    {
        return NULL;
    }

    min_heap_node *root = heap->array[0];
    min_heap_node *last_node = heap->array[heap->size - 1];

    heap->array[0] = last_node;
    heap->pos[root->v] = heap->size - 1;
    heap->pos[last_node->v] = 0;

    --heap->size;

    min_heapify(heap, 0);

    return root;
}

// aggiorna una chiave nella heap
void decrease_key(min_heap *heap, int v, ull weight)
{
    int i = heap->pos[v];
    heap->array[i]->weight = weight;

    while (i && heap->array[i]->weight < heap->array[(i - 1) / 2]->weight)
    {
        int parent_i = (i - 1) / 2;

        heap->pos[heap->array[i]->v] = parent_i;
        heap->pos[heap->array[parent_i]->v] = i;
        swap_min_heap_nodes(&heap->array[i], &heap->array[parent_i]);
        i = parent_i;
    }
}

// LOGICA PRINCIPALE

void dijkstra_with_whirlpools(adj_list *graph, int src, int N, ull *dist, int *reached_by_whirlpool)
{
    // la coda di priorità è vuota all'inizio
    min_heap *heap = init_min_heap();
    // per ogni nodo, ipotizzo che la distanza per raggiungerlo sia non abbordabile
    // (non potrà che migliorare); in più, lo aggiungo alla heap.
    for (int i = 0; i < MAX_N; i++)
    {
        dist[i] = ULLONG_MAX;
        //reached_by_whirlpool[i] = 0; // inizialmente nessun nodo è raggiunto da mulinello
        heap->array[i] = init_min_heap_node(i, dist[i]);
        heap->pos[i] = i;
    }
    // il nodo di partenza è un'eccezione: la distanza per raggiungerlo è 0 ed è pertanto
    // la radice della heap.
    dist[src] = 0;
    heap->array[src] = init_min_heap_node(src, dist[src]);
    heap->pos[src] = src;
    heap->size = MAX_N;
    decrease_key(heap, src, dist[src]);
    
    // fin tanto che ho nodi nella heap, estraggo l'elemento più promettente (la radice)
    while (heap->size)
    {
        min_heap_node *heap_node = extract_min(heap);
        int u = heap_node->v;
        adj_list_node *neighbor = graph->heads[u];
        
        while (neighbor)
        {
            int v = neighbor->dest;
            ull new_dist = dist[u] + neighbor->weight;
            
            // Considera TUTTI gli archi, inclusi i mulinelli
            if (new_dist < dist[v])
            {
                dist[v] = new_dist;
                decrease_key(heap, v, dist[v]);
                // aggiorna se questo nodo è raggiunto tramite mulinello
                reached_by_whirlpool[v] = neighbor->is_whirlpool;
            }
            else if (new_dist == dist[v])
            {
                // Stesso tempo: se arrivo ora con un arco normale e prima era con mulinello, 
                // preferisco l'arco normale
                if (reached_by_whirlpool[v] == 1 && neighbor->is_whirlpool == 0)
                {
                    reached_by_whirlpool[v] = 0;
                }
            }
            
            neighbor = neighbor->next;
        }
       
        //free(heap_node);
    }
    
    free(heap->pos);
    free(heap->array);
    free(heap);
}

// DRIVER MODIFICATO
void solve(FILE *input_file, FILE *output_file)
{
    int N, M, K;
    adj_list *graph = init_adj_list();
    
    if (fscanf(input_file, "%d %d %d", &N, &M, &K) != 3)
    {
        fprintf(stderr, "Errore lettura N, M, K\n");
        exit(1);
    }

    int reached_by_whirlpool[MAX_N];
    // Leggi gli archi normali del grafo
    for (int i = 0; i < M; i++)
    {
        int u, v;
        ull w;
        if (fscanf(input_file, "%d %d %llu", &u, &v, &w) != 3)
        {
            fprintf(stderr, "Errore lettura arco %d\n", i);
            exit(1);
        }
        insert_edge(graph, u, v, w, 0); // 0 = non è mulinello
        insert_edge(graph, v, u, w, 0); // grafo non direzionato
         // Inizializza il raggiungimento tramite mulinello
        reached_by_whirlpool[i] = 0; // inizialmente, il nodo s è raggiunto tramite mulinello
    }

   
    // Leggi e AGGIUNGI i mulinelli direttamente al grafo
    for (int i = 0; i < K; i++)
    {
        int s;
        ull y;
        if (fscanf(input_file, "%d %llu", &s, &y) != 2)
        {
            fprintf(stderr, "Errore lettura mulinello %d\n", i);
            exit(1);
        }
        
        // Aggiungi i mulinelli come archi dal nodo 0 (bidirezionali)
        insert_edge(graph, 0, s, y, 1); // 1 = è mulinello
        insert_edge(graph, s, 0, y, 1);
       
    }
    
    // Calcola le distanze minime considerando TUTTI gli archi
    ull dist[MAX_N];
   
    dijkstra_with_whirlpools(graph, 0, N, dist, reached_by_whirlpool);
    
    // Conta quanti nodi NON sono stati raggiunti tramite mulinelli
    // Questo rappresenta i mulinelli inutili
    int useful_whirlpools = 0;
    for (int i = 0; i < N; i++)
    {
        if (reached_by_whirlpool[i])
        {
            useful_whirlpools++;
        }
    }
    
    int inconvenient_count = K - useful_whirlpools;
    fprintf(output_file, "%d", inconvenient_count);
    
    free_graph(graph);
}

int main(void)
{
    FILE *input_file, *output_file;
    input_file = fopen("input.txt", "r");
    output_file = fopen("output.txt", "w");

    if (input_file == NULL || output_file == NULL)
    {
        fprintf(stdout, "IO Error!");
        return -1;
    }

    solve(input_file, output_file);

    if (fclose(input_file) != 0 || fclose(output_file) != 0)
    {
        fprintf(stdout, "IO Error!");
        return -1;
    }

    return 0;
}