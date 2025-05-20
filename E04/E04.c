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

struct Mulinello{
    int destination;
    int weight;
};

int weights_comparator(const void *x, const void *y) {
    const struct Edge *a = x;
    const struct Edge *b = y;

    return a->weight < b->weight;
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

void kruskal(struct Node *nodes, struct Edge *edges, int n_edges ) 
{
    qsort(edges, n_edges, sizeof(struct Edge), weights_comparator);

    // Creazione dell'albero di copertura minimo
    for (int i = 0; i < n_edges; i++) {
        if (!same(edges[i].from, edges[i].to, nodes)) {
            merge(edges[i].from, edges[i].to, nodes);
        }
    }

}

/*
* Dopo aver costruito l'MST, puoi rappresentarlo come un grafo (ad esempio con una lista di adiacenza).
* Poi, per trovare il peso del percorso tra due nodi nell'MST, puoi fare una DFS o BFS e sommare i pesi degli archi lungo il percorso.
* Qui sotto c'è un esempio di costruzione della lista di adiacenza dell'MST.
*/
// Costruisci la lista di adiacenza dell'MST
struct AdjNode {
    int to;
    int weight;
    struct AdjNode *next;
};

    
// Funzione per trovare il peso del percorso tra due nodi nell'MST (DFS)
int dfs(int curr, int dest, int parent, struct AdjNode **adj, int *found, int acc) {
    if (curr == dest) {
        *found = 1;
        return acc;
    }
    for (struct AdjNode *p = adj[curr]; p != NULL; p = p->next) {
        if (p->to != parent) {
            int res = dfs(p->to, dest, curr, adj, found, acc + p->weight);
            if (*found) return res;
        }
    }
    return 0;
// Esempio di utilizzo:
// int found = 0;
// int peso = dfs(sorgente, destinazione, -1, adj, &found, 0);
// peso ora contiene il peso del percorso MST tra sorgente e destinazione

}



int main(void) {
    FILE *in_file = fopen("input.txt", "r");
    FILE *out_file = fopen("output.txt", "w");

    if (in_file == NULL || out_file == NULL) {
        printf("Errore durante l'apertura di un file");
        return -1;
    }

    int nodi,archi,nmulinelli;
    if(fscanf(in_file, "%d %d %d", &nodi, &archi, &nmulinelli) != 3) {
        printf("Errore durante la lettura del file");
        fclose(in_file);
        fclose(out_file);
        return -1;
    }

    struct Node nodes[nodi];
    struct Edge edges[archi];
    struct Mulinello mulinelli[nmulinelli];

    // Inizializzazione dei nodi
    for (int i=0; i<nodi; i++) {
        nodes[i].representative = i;
        nodes[i].rank = 1;
    }
    
    for(int i = 0; i < archi; i++) {
        if(fscanf(in_file, "%d %d %d", &edges[i].from, &edges[i].to, &edges[i].weight) != 3) {
            printf("Errore durante la lettura del file");
            fclose(in_file);
            fclose(out_file);
            return -1;
        }
    }

    for(int i = 0; i < nmulinelli; i++) {
        if(fscanf(in_file, "%d %d", &mulinelli[i].destination, &mulinelli[i].weight) != 2) {
            printf("Errore durante la lettura del file");
            fclose(in_file);
            fclose(out_file);
            return -1;
        }
    }

    //richiamo kruskal e calcolo mst
    kruskal(nodes, edges, archi);
    
    /*per ogni mulinello calcolo a partire ma mst peso del percorso
    dalla sorgente a mulinello.destination e lo confronto con il peso
    del mulinello, se il peso del mulinello è maggiore incremento il contatore
    di mulinelli svantaggiosi
    */

    struct AdjNode *adj[nodi];
    for (int i = 0; i < nodi; i++) adj[i] = NULL;

    // Ricostruisci MST: aggiungi solo gli archi effettivamente usati
    for (int i = 0; i < archi; i++) {
        if (find(edges[i].from, nodes) == find(edges[i].to, nodes)) {
            // Aggiungi arco in entrambe le direzioni
            struct AdjNode *new1 = malloc(sizeof(struct AdjNode));
            new1->to = edges[i].to;
            new1->weight = edges[i].weight;
            new1->next = adj[edges[i].from];
            adj[edges[i].from] = new1;

            struct AdjNode *new2 = malloc(sizeof(struct AdjNode));
            new2->to = edges[i].from;
            new2->weight = edges[i].weight;
            new2->next = adj[edges[i].to];
            adj[edges[i].to] = new2;
        }
    }
    int svant=0;
    //confronta peso di ogni mulinello con peso del percorso
    for (int i = 0; i < nmulinelli; i++) {
        int found = 0;
        int peso = dfs(0, mulinelli[i].destination, -1, adj, &found, 0);
        if (peso > mulinelli[i].weight) {
            // Incrementa il contatore di mulinelli svantaggiosi
            svant++;
        }
    }
    // Stampa il risultato
    fprintf(out_file, "%d\n", svant);


    if (fclose(in_file)!= 0 || fclose(out_file)!= 0) {
        printf("Errore durante la chiusura di un file");
        return -1;    
    }

    return 0;
}