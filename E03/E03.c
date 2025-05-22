#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000

//int tree[4 * MAX_SIZE];
//int lazy[4 * MAX_SIZE];

// Costruzione base
void build(int arr[], int tree[], int node, int start, int end) {
    if (start == end) {
        tree[node] = arr[start];  // ✅ usa l'array originale
    } else {
        int mid = (start + end) / 2;
        build(arr, tree, 2 * node, start, mid);
        build(arr, tree, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
}

// Funzione per propagare i ritardi
void propagate(int tree[],int lazy[],int node, int start, int end) {
    if (lazy[node] != 0) {
        // Applichiamo l'aggiornamento al nodo corrente
        tree[node] += (end - start + 1) * lazy[node];
        if (start != end) {
            // Propaghiamo ai figli
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        lazy[node] = 0;
    }
}

// Aggiorna range [l, r] aggiungendo val
void update_range(int tree[],int lazy[],int node, int start, int end, int l, int r, int val) {
    propagate(tree,lazy,node, start, end);

    if (r < start || end < l) return; // fuori intervallo
    if (l <= start && end <= r) {
        lazy[node] += val;
        propagate(tree,lazy,node, start, end);
        return;
    }

    int mid = (start + end) / 2;
    update_range(tree,lazy,2 * node, start, mid, l, r, val);
    update_range(tree,lazy,2 * node + 1, mid + 1, end, l, r, val);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// Query somma in intervallo [l, r]
int Query(int tree[],int lazy[],int node, int start, int end, int l, int r) {
    propagate(tree,lazy,node, start, end);

    if (r < start || end < l) return 0; // fuori intervallo
    if (l <= start && end <= r) return tree[node];

    int mid = (start + end) / 2;
    int left_sum = Query(tree,lazy,2 * node, start, mid, l, r);
    int right_sum = Query(tree,lazy,2 * node + 1, mid + 1, end, l, r);
    return left_sum + right_sum;
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
    printf("num celle e query lette %d %d\n", num_celle, num_query);

    int *arr = malloc(num_celle * sizeof(int));
    int *tree = malloc(4 * num_celle * sizeof(int));
    int *lazy = calloc(4 * num_celle, sizeof(int)); 

    for (int i = 0; i < num_celle; i++)
    {
        if (fscanf(input, "%d", &arr[i]) != 1)
        {
            printf("Error reading cell from input file.\n");
            fclose(input);
            fclose(output);
            free(tree);
            return 1;
        }
    }
    for (int i = 0; i < num_celle; i++)
    {
        printf("cella letta %d \n", arr[i]);
    }
    //calcola_prefissi(celle,num_celle,prefix_sum);
    build(arr, tree, 1, 0, num_celle - 1);
    for (int i = 0; i < num_query; i++)
    {
        char query[10];
        if (fscanf(input, "%s", query) != 1)
        {
            printf("Error reading query from input file.\n");
            fclose(input);
            fclose(output);
            free(tree);
            return 1;
        }
        char query_type = query[0];
        printf("query letta %s\n", query);
        printf("query type %c\n", query_type);
        if (query_type == 'A')
        {
            int params[3];
            if (fscanf(input, "%d %d %d", &params[0], &params[1], &params[2]) != 3)
            {
                printf("Error reading parameters for query A from input file.\n");
                fclose(input);
                fclose(output);
                free(tree);
                return 1;
            }
            printf("parametri letti %d %d %d\n", params[0], params[1], params[2]);
            update_range(tree, lazy, 1, 0, num_celle - 1, params[0], params[1], params[2]);
        }
        else if(query_type == 'Q')
        {
            int params[2];
            if (fscanf(input, "%d %d", &params[0], &params[1]) != 2)
            {
                printf("Error reading parameters for query Q from input file.\n");
                fclose(input);
                fclose(output);
                free(tree);
                return 1;
            }
            printf("parametri letti %d %d\n", params[0], params[1]);
            int result = Query(tree, lazy, 1, 0, num_celle - 1, params[0], params[1]);
            fprintf(output, "%d\n", result);
        }
    }
    free(arr);
    free(tree);
    free(lazy);
    fclose(input);
    fclose(output);
}
