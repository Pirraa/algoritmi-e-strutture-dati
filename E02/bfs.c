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

void stampa(Lista l)
{
    while (l)
    {
        printf("incontro con %d ", l->pugile);
        l = l->next;
    }
    printf("\n");
}

int bfsBipartito(Lista *grafo, int *colore, int start, int n)
{
    int *queue = malloc(n * sizeof(int));
    int front = 0, rear = 0;

    //front: indice elemento da processare (esce dalla coda)
    //rear: indice elemento da inserire (entra nella coda)
    
    //setta colore sorgente a 0 e la inserisce in coda(enqueue)
    colore[start] = 0;
    queue[rear++] = start;

    //finchè ci sono elementi nella coda 
    while (front < rear)
    {
        //estraggo nodo dalla coda (dequeue)
        int u = queue[front++];
        //il for scorre la lista di adiacenza del nodo u
        for (Nodo *p = grafo[u]; p != NULL; p = p->next)
        {
            //prendo indice del pugile dal nodo e controllo il colore
            int v = p->pugile;
            //se il pugile non è stato colorato, lo coloro con il colore opposto a quello di u e lo inserisco in coda
            if (colore[v] == -1)
            {
                colore[v] = 1 - colore[u]; //1-0=1 1-1=0
                queue[rear++] = v;
            }
            //se il pugile è già colorato e ha lo stesso colore di u, il grafo non è bipartito
            //se il pugile è già colorato e ha un colore diverso da u, non faccio nulla
            else if (colore[v] == colore[u])
            {
                free(queue);
                return 0; // non bipartito
            }
        }
    }

    free(queue);
    return 1; // bipartito
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
        printf("Error reading number of pugili.\n");
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
        printf("Error reading number of incontri.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    for (int i = 0; i < num_incontri; i++)
    {
        int pugile1, pugile2;
        if (fscanf(input, "%d %d", &pugile1, &pugile2) != 2)
        {
            printf("Error reading incontro %d.\n", i);
            fclose(input);
            fclose(output);
            return 1;
        }
        insTesta(&incontri[pugile1], pugile2);
        insTesta(&incontri[pugile2], pugile1);
    }

    // Array colori per bipartizione (-1 = non assegnato)
    //non metto il colore nel nodo perchè il colore si riferisce ai nodi del grafo e non agli archi
    //la lista di adiacenza è un array di liste, quindi non posso mettere il colore nel nodo
    //nella lista di adiacenza posso avere più nodi che fanno riferimento allo stesso pugile (ogni arco rappresentato due volte)
    //se salvo il colore nel nodo, non posso avere più nodi con lo stesso pugile ma con colori diversi, quindi incoerenza nei dati
    int *colore = malloc(num_pugili * sizeof(int));
    for (int i = 0; i < num_pugili; i++)
        colore[i] = -1;

    int bipartito = 1;

    // gestisce grafi non connessi
    //cavallo di troia come depthfirstsearch che richiama la visita per ogni componente connessa nel caso il grafo sia sconnesso
    // per ogni nodo non visitato, chiama bfsBipartito
    for (int i = 0; i < num_pugili && bipartito; i++)
    {
        if (colore[i] == -1)
        {
            if (!bfsBipartito(incontri, colore, i, num_pugili))
                bipartito = 0;
        }
    }

    if (bipartito)
    {
        fprintf(output, "TRUE");
        /*printf("È possibile dividere i pugili in due gruppi.\n");
        printf("Gruppo 0:\n");
        for (int i = 0; i < num_pugili; i++)
            if (colore[i] == 0)
                printf("%d ", i);
        printf("\nGruppo 1:\n");
        for (int i = 0; i < num_pugili; i++)
            if (colore[i] == 1)
                printf("%d ", i);
        printf("\n");*/
    }
    else
    {
        fprintf(output, "FALSE");
        /*printf("Non è possibile dividere i pugili in due gruppi.\n");
        printf("Non bipartibile\n");*/
    }

    // stampa debug (facoltativa)
    /*
    for (int i = 0; i < num_pugili; i++)
    {
        printf("pugile %d: ", i);
        stampa(incontri[i]);
    }
    */

    // Libera memoria
    for (int i = 0; i < num_pugili; i++)
    {
        Nodo *corr = incontri[i];
        while (corr)
        {
            Nodo *tmp = corr;
            corr = corr->next;
            free(tmp);
        }
    }
    free(incontri);
    free(colore);
    fclose(input);
    fclose(output);
    return 0;
}
