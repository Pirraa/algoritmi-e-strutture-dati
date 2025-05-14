#include <stdio.h>
#include <stdlib.h>

//fenwick tree struttura ottimizzata per calcolare somma in un range di indici di un array e per effettuare update su un range di indici su un array
//si basa su array segment tree (albero che contiene la somma di un range di indici che si dimezza sempre)
//da segment tree elimina i nodi intermedi e mantiene solo i nodi padre
//l'array Fenwick è 1-based, quindi l'indice parte da 1

//https://medium.com/carpanese/a-visual-introduction-to-fenwick-tree-89b82cac5b3c
void createFenwick(int *fenwick, int *celle, int num_celle)
{
    for (int i = 1; i <= num_celle; i++) {
        int index = i;
        while (index <= num_celle) {
            fenwick[index] += celle[i-1];
            index += (index & -index);
        }
    }
}

//come alla creazione ma in questo caso aggiorno un range di celle
void updateFenwick(int *fenwick, int start, int end, int value, int num_celle)
{
    for (int i = start; i <= end; i++) {
        int index = i;
        while (index <= num_celle) {
            fenwick[index] += value;
            //aggiorno 4 00100 dopo passo all'indice 8 01000
            index += (index & -index); // Passa al nodo padre mettendo 1 sul bit più a sinistra del bit più significativo 
        }
    }
}

// Funzione per calcolare la somma prefissa in un array Fenwick, corrisponde alla query
// La somma prefissa fino a un certo indice idx
// è la somma di tutti gli elementi da 1 a idx
// La funzione Fenwick tree è 1-based, quindi l'indice parte da 1
int prefixSum(int *fenwick, int idx)
{
    int sum = 0;
    while (idx > 0) {
        sum += fenwick[idx];
        idx -= (idx & -idx);//inverto l'ultimo bit settato a 1 cioè quello più a destra
        //esempio 7=00111, -7=11001, 00111 & 11001 = 00001, 00111 - 00001 = 00110 = 6
    }
    return sum;
}

// Funzione per calcolare la somma tra due indici in un array Fenwick
int sumFenwick(int *fenwick, int start, int end)
{
    //start -1 perché l'array Fenwick è 1-based
    return prefixSum(fenwick, end) - prefixSum(fenwick, start - 1);
}

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

    int num_celle, num_query;

    if (fscanf(input, "%d %d", &num_celle, &num_query) != 2)
    {
        printf("Error reading number of celle and query.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    // Aggiungo +1 per l'indice che parte da 1
    int *celle = malloc((num_celle + 1) * sizeof(int));
    int *fenwick = calloc((num_celle + 1), sizeof(int)); // inizializza a 0

    for (int i = 1; i <= num_celle; i++)
    {
        if (fscanf(input, "%d", &celle[i]) != 1)
        {
            printf("Error reading cell from input.\n");
            fclose(input);
            fclose(output);
            free(celle);
            free(fenwick);
            return 1;
        }
    }

    createFenwick(fenwick, celle, num_celle);

    for (int i = 0; i < num_query; i++)
    {
        char query[10];
        if (fscanf(input, "%s", query) != 1)
        {
            printf("Error reading query.\n");
            break;
        }

        char query_type = query[0];
        if (query_type == 'A')
        {
            int start, end, value;
            if (fscanf(input, "%d %d %d", &start, &end, &value) != 3)
            {
                printf("Error reading A query parameters.\n");
                break;
            }
            // Converti indici da 0-based a 1-based
            updateFenwick(fenwick, start + 1, end + 1, value, num_celle);
        }
        else if (query_type == 'Q')
        {
            int start, end;
            if (fscanf(input, "%d %d", &start, &end) != 2)
            {
                printf("Error reading Q query parameters.\n");
                break;
            }
            // Converti indici da 0-based a 1-based
            int result = sumFenwick(fenwick, start + 1, end + 1);
            fprintf(output, "%d\n", result);
        }
    }

    fclose(input);
    fclose(output);
    free(celle);
    free(fenwick);

    return 0;
}
