#include <stdio.h>
#include <stdlib.h>

struct Node {
    // Vedi appunti sui disjoint set.
    int representative;
    int rank;
};

struct Edge {
    int from;
    int to;
    int weight;
};

int weights_comparator(const void *x, const void *y) {
    const struct Edge *a = x;
    const struct Edge *b = y;

    return a->weight < b->weight;
}

struct Cart {
    // Larghezza del carrello.
    int value;

    // Promemoria: quando ti ho preso in input?
    // Questo  mi serve più avanti per stampare 
    // l'output nell'ordine corretto.
    int original_idx;
};

int carts_comparator(const void *x, const void *y) {
    const struct Cart *a = x;
    const struct Cart *b = y;

    return a->value < b->value;
}

int find(int n, struct Node *nodes) {
    // Compressione del percorso
    while (n != nodes[n].representative) {
        n = nodes[n].representative;
    }
    return n;
}

int same(int a, int b, struct Node *nodes) {
    return find(a, nodes) == find(b, nodes);
}

void merge(int a, int b, struct Node *nodes) {
    a = find(a, nodes);
    b = find(b, nodes);

    if (nodes[a].rank < nodes[b].rank) {
        // swap a and b
        a = a ^ b;
        b = a ^ b;
        a = b ^ a;
    }
    nodes[a].rank += nodes[b].rank;
    nodes[b].representative = a;
}

void kruskal(
    struct Node *nodes, 
    struct Edge *edges, 
    int n_edges, 
    int current_value, 
    int *last_index, 
    int *last_answer
) {
    // Se non posso più visitare archi, esco:
    // conosco già di quanti rimpicciolitori ho bisogno (last_answer)
    while (*last_index < n_edges) {
        // Se ho un carrello troppo largo per la galleria che sto guardando
        // non posso fare altro che uscire
        if (current_value > edges[*last_index].weight) {
            return;
        }

        // Se posso percorrere l'arco che sto guardando, lo aggiungo
        // ad un nuovo disjoint set
        int from_id = edges[*last_index].from;
        int to_id = edges[*last_index].to;

        if (!same(from_id, to_id, nodes)) { // find(from_id, nodes) == find(to_id, nodes)
            merge(from_id, to_id, nodes);
            (*last_answer)--;
        }

        // Passo al prossimo edge
        (*last_index)++;
    }
}

void castori(FILE *in_file, FILE *out_file) {
    int N; // Numero di case (nodi)
    int K; // Numero di gallerie (archi)
    int T; // Numero di carrelli (ciascuno con peso W)

    fscanf(in_file, "%d %d %d", &N, &K, &T);

    struct Node nodes[N];
    struct Edge edges[K];
    struct Cart carts[T];

    // Qui mi salvo gli output mano a mano che considero le query;
    // dato un carrello C, C.original_idx ci aiuta a capire
    // in che ordine dovremmo stampare queste risposte.
    int answers[T];

    // Inizializziamo i nodi
    for (int i=0; i<N; i++) {
        nodes[i].representative = i;
        nodes[i].rank = 1;
    }

    // Prendiamo in input i vari archi e li ordiniamo dal più largo al più stretto
    for (int i=0; i<K; i++) {
        fscanf(in_file, "%d %d %d", 
            &edges[i].from, &edges[i].to, &edges[i].weight);
    }
    qsort(edges, K, sizeof(struct Edge), weights_comparator);

    // Prendiamo i carrelli e li ordiniamo dal più largo al più stretto
    for (int i=0; i<T; i++) {
        fscanf(in_file, "%d", &carts[i].value);
        carts[i].original_idx = i;
    }
    qsort(carts, T, sizeof(struct Cart), carts_comparator);

    // Vogliamo rimbalzare avanti e indietro con la funzione che implementa Kruskal:
    // la prima volta che chiamiamo Kruskal, prendiamo il carrellino C più largo di tutti
    // e consideriamo tutti gli archi che potremmo attraversare con C.
    // 
    // Per ogni arco "partenza -> arrivo" considerato, uniremo i disjoint set associati ai
    // nodi "partenza" e "arrivo".
    // 
    // Quando ci imbattiamo in un arco che è troppo piccolo per essere attraversato con C,
    // sappiamo quanti utilizzi di rimpicciolitore ci servono (numero_set_rimasti - 1).
    //
    // Torniamo nel main, selezioniamo il prossimo carrellino e torniamo in Kruskal 
    // ma non ripartiamo a scorrere tutti gli archi: possiamo ripartire da dove c'eravamo 
    // lasciati.
    //
    // Ovvero dal checkpoint "last_index"
    int last_index = 0;

    // Questa è la risposta ad una nostra query; si abbassa mano a mano che facciamo union
    // tra disjoint set distinti.
    int last_answer = N-1;

    for (int i=0; i<T; i++) {
        int current_value = carts[i].value;
        kruskal(nodes, edges, K, current_value, &last_index, &last_answer);
        
        // Attenzione: l'ordine in cui dobbiamo stampare gli output non è l'ordine in cui 
        // li calcoliamo! Dobbiamo tenere traccia di questa cosa.
        answers[carts[i].original_idx] = last_answer;    
    }
    
    for (int i=0; i<T; i++) {
        fprintf(out_file, "%d\n", answers[i]);
    }
}

int main(void) {
    FILE *in_file = fopen("input.txt", "r");
    FILE *out_file = fopen("output.txt", "w");

    if (in_file == NULL || out_file == NULL) {
        printf("Errore durante l'apertura di un file");
        return -1;
    }

    castori(in_file, out_file);

    if (fclose(in_file)!= 0 || fclose(out_file)!= 0) {
        printf("Errore durante la chiusura di un file");
        return -1;    
    }

    return 0;
}