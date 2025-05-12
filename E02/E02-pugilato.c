#include <stdio.h>
#include <stdlib.h>

typedef struct nodo
{
    int pugile;
    struct nodo *next;
} Nodo;

typedef Nodo *Lista;

void nuovaLista(Lista *pl) { *pl = NULL; }

void insTesta(Lista *pl, int d)
{
    Nodo *aux = malloc(sizeof(Nodo));
    aux->pugile = d;
    aux->next = *pl;
    *pl = aux;
}

/*typedef struct Nodo {
    int v;
    struct Nodo* next;
} Nodo;

Nodo* grafo[MAX];

void aggiungiArco(int u, int v) {
    Nodo* nuovo = malloc(sizeof(Nodo));
    nuovo->v = v;
    nuovo->next = grafo[u];
    grafo[u] = nuovo;

    nuovo = malloc(sizeof(Nodo));
    nuovo->v = u;
    nuovo->next = grafo[v];
    grafo[v] = nuovo;
}*/

void stampa(Lista l)
{
    while (l)
    {
        printf("incontro con %d ", l->pugile);
        l = l->next;
    }
    printf("\n");
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
    int num_pugili, num_incontri;

    if (fscanf(input, "%d", &num_pugili) != 1)
    {
        printf("Error reading number of pugili from input file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    Lista *incontri = malloc(num_pugili * sizeof(Lista));

    for (int i = 0; i < num_pugili; i++)
    {
        nuovaLista(&incontri[i]);
    }

    if (fscanf(input, "%d", &num_incontri) != 1)
    {
        printf("Error reading number of incontri from input file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }
    printf("%d %d\n", num_pugili, num_incontri);
    for (int i = 0; i < num_incontri; i++)
    {
        int pugile1, pugile2;
        if (fscanf(input, "%d", &pugile1) != 1)
        {
            printf("Error reading pugile1 from input file.\n");
            fclose(input);
            fclose(output);
            return 1;
        }
        if (fscanf(input, "%d", &pugile2) != 1)
        {
            printf("Error reading pugile2 from input file.\n");
            fclose(input);
            fclose(output);
            return 1;
        }
        insTesta(&incontri[pugile1], pugile2);
        insTesta(&incontri[pugile2], pugile1);
    }

    for (int i = 0; i < num_pugili; i++)
    {
        printf("pugile %d: ", i);
        stampa(incontri[i]);
    }
}