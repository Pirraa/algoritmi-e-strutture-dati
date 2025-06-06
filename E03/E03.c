#include <stdio.h>
#include <stdlib.h>


//sia il segment tree che il lazy tree hanno 4* numero elemnti array originale
//segmente tree contiene la somma degli intervalli, i figli di un nodo contengono la somma degli elementi nella metà destra e nella metà sinistra
//lazy tree contiene i ritardi degli aggiornamenti, per evitare di aggiornare ogni volta il segmento, si aggiorna solo quando serve

// Costruzione iniziale del Segment Tree
//node in tutte le funzioni è l'indice del nodo corrente nel Segment Tree
//start e end sono gli indici dell'intervallo corrente del Segment Tree
void build(int arr[], long long tree[], int node, int start, int end) {
    //caso base: se l'intervallo è un singolo elemento
    if (start == end) {
        tree[node] = arr[start];  
    } else {
        //casi ricorsivi: dividiamo l'intervallo in due metà
        int mid = (start + end) / 2;
        build(arr, tree, 2 * node, start, mid);
        build(arr, tree, 2 * node + 1, mid + 1, end);
        //aggiorniamo il nodo corrente con la somma dei suoi figli
        //2*node e 2*node+1 sono gli indici dei figli nel Segment Tree
        //2*node è il figlio sinistro, 2*node+1 è il figlio destro
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
}

// Funzione per propagare i ritardi
void propagate(long long tree[], long long lazy[], int node, int start, int end) {
    // il lazy è inizalizzato a 0, se non c'è 0 ho qualcosa da propagare
    if (lazy[node] != 0) {
        // Applichiamo l'aggiornamento al nodo corrente
        //corrisponde al numero di nodi sotto di lui * il valore da propagare 
        //lazy[node] contiene il valore da aggiungere a tutti gli elementi nell'intervallo [start, end]
        tree[node] += (end - start + 1) * lazy[node];
        if (start != end) {
            // Propaghiamo ai figli nel lazy
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        //ho fatto l'aggiornamento e rimetto a 0
        lazy[node] = 0;
    }
}

// Aggiorna range [l, r] aggiungendo val
void update_range(long long tree[], long long lazy[], int node, int start, int end, int l, int r, long long val) {
    propagate(tree,lazy,node, start, end);

    // Controllo se l'intervallo corrente è fuori dall'intervallo [l, r]
    //se r è prima di start o end è dopo r, allora non devo fare nulla
    if (r < start || end < l) return; 
    //se l'intervallo corrente è completamente dentro l'intervallo [l, r]
    if (l <= start && end <= r) {
        //segno la propagazioned a effettuare
        //lazy[node] contiene il valore da aggiungere a tutti gli elementi nell'intervallo [start, end]
        lazy[node] += val;
        //propaghiamo il valore ai figli nel lazy
        propagate(tree,lazy,node, start, end);
        return;
    }
    //aggiorno il figlio sinistro e destro del nodo corrente nel segment tree
    int mid = (start + end) / 2;
    update_range(tree,lazy,2 * node, start, mid, l, r, val);
    update_range(tree,lazy,2 * node + 1, mid + 1, end, l, r, val);
    //Propago sui figli prima di aggiornare il nodo corrente
    //propagate(tree, lazy, 2 * node, start, mid);
    //propagate(tree, lazy, 2 * node + 1, mid + 1, end);
    // Dopo aver aggiornato i figli, aggiorniamo il nodo corrente
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// Query somma in intervallo [l, r]
long long Query(long long tree[], long long lazy[], int node, int start, int end, int l, int r) {
    propagate(tree,lazy,node, start, end);

    // Controllo se l'intervallo corrente è fuori dall'intervallo [l, r]
    if (r < start || end < l) return 0; 
    // Controllo se l'intervallo corrente è completamente dentro l'intervallo [l, r]
    if (l <= start && end <= r) return tree[node];

    //calcolo somma a sinistra e a destra
    int mid = (start + end) / 2;
    //long long left_sum = Query(tree,lazy,2 * node, start, mid, l, r);
    //long long right_sum = Query(tree,lazy,2 * node + 1, mid + 1, end, l, r);
    // Restituisco la somma dei risultati delle due metà
    //return left_sum + right_sum;
    return Query(tree,lazy,2 * node, start, mid, l, r) +
           Query(tree,lazy,2 * node + 1, mid + 1, end, l, r);
}

// Esempio d'uso
int main() 
{
    FILE *input;
    FILE *output;

    input = fopen("input.txt", "r");
    if (input == NULL)
    {
        printf("Error opening input file.\n");
        return 1;
    }
    output = fopen("output.txt", "w");
    if (output == NULL)
    {
        printf("Error opening output file.\n");
        fclose(input);
        return 1;
    }

    int num_celle,num_query;


    if (fscanf(input, "%d %d", &num_celle, &num_query) != 2)
    {
        printf("Error reading number of celle and query from input file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }


    int *arr = malloc(num_celle * sizeof(int));
    long long *tree = calloc(4 * num_celle, sizeof(long long));
    long long *lazy = calloc(4 * num_celle, sizeof(long long));

    for (int i = 0; i < num_celle; i++)
    {
        if (fscanf(input, "%d", &arr[i]) != 1)
        {
            printf("Error reading cell from input file.\n");
            fclose(input);
            fclose(output);
            free(tree);
            free(arr);
            free(lazy);
            return 1;
        }
    }

    
    build(arr, tree, 1, 0, num_celle - 1);

    for (int i = 0; i < num_query; i++)
    {
        char query;
        if (fscanf(input, " %c", query) != 1)
        {
            printf("Error reading query from input file.\n");
            fclose(input);
            fclose(output);
            free(arr);
            free(tree);
            free(lazy);
            return 1;
        }

        if (query == 'A')
        {
            int params[3];
            if (fscanf(input, "%d %d %d", &params[0], &params[1], &params[2]) != 3)
            {
                printf("Error reading parameters for query A from input file.\n");
                fclose(input);
                fclose(output);
                free(arr);
                free(tree);
                free(lazy);
                return 1;
            }
            //inizio la ricorsione dal nodo 1 e con indici dell'intervallo [0, num_celle - 1]
            //params[0] e params[1] sono gli indici dell'intervallo [l, r], params[2] è il valore da aggiungere
            update_range(tree, lazy, 1, 0, num_celle - 1, params[0], params[1], params[2]);
        }
        else if(query == 'Q')
        {
            int params[2];
            if (fscanf(input, "%d %d", &params[0], &params[1]) != 2)
            {
                printf("Error reading parameters for query Q from input file.\n");
                fclose(input);
                fclose(output);
                free(arr);
                free(tree);
                free(lazy);
                return 1;
            }
            //inizio la ricorsione dal nodo 1 e con indici dell'intervallo [0, num_celle - 1]
            //params[0] e params[1] sono gli indici dell'intervallo [l, r]
            long long result = Query(tree, lazy, 1, 0, num_celle - 1, params[0], params[1]);
            fprintf(output, "%lld\n", result);
        }
    }
    free(arr);
    free(tree);
    free(lazy);
    fclose(input);
    fclose(output);
}
