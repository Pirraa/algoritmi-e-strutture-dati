#include<stdio.h>
#include<stdlib.h>

// Calcola l'array dei prefissi
void calcola_prefissi(int *celle, int num_celle, int *prefix_sum) {
    prefix_sum[0] = 0;  // prefix_sum[0] è sempre 0, rappresenta la somma fino all'indice -1
    for (int i = 1; i <= num_celle; i++) {
        prefix_sum[i] = prefix_sum[i - 1] + celle[i - 1];
    }
}


int execute_query1(int *celle, int num_celle, char query_type, int *params)
{
    int somma=0;
    if (query_type == 'A')
    {
        for(int i=params[0];i<=params[1];i++)
        {
           celle[i]+=params[2];
        }
    }
    else if (query_type == 'Q')
    {
        
        for(int i=params[0];i<=params[1];i++)
        {
            somma+=celle[i];
        }
    }
    return somma;
}

int execute_query(int *celle, int num_celle, char query_type, int *params,int *prefix_sum)
{
    int somma = 0;

    if (query_type == 'A') // Aggiunta
    {
        for (int i = params[0]; i <= params[1]; i++) {
            celle[i] += params[2];
        }

        // Dopo l'aggiornamento, bisogna aggiornare l'array prefisso
        for (int i = params[0]; i <= num_celle; i++) {
            prefix_sum[i] = prefix_sum[i - 1] + celle[i - 1]; // Ricalcola i prefissi
        }
    }
    else if (query_type == 'Q') // Query somma
    {
        // Usando l'array dei prefissi per calcolare la somma dell'intervallo
        somma = prefix_sum[params[1] + 1] - prefix_sum[params[0]]; // somma tra params[0] e params[1]
    }

    return somma;
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

    int num_celle,num_query;


    if (fscanf(input, "%d %d", &num_celle, &num_query) != 2)
    {
        printf("Error reading number of celle and query from input file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }
    printf("num celle e query lette %d %d\n", num_celle, num_query);

    int *celle = malloc(num_celle * sizeof(int));
    int *prefix_sum=malloc(num_celle*sizeof(int)+1);

    for(int i=0;i<num_celle;i++)
    {
        if (fscanf(input, "%d", &celle[i]) != 1)
        {
            printf("Error reading cell from input file.\n");
            fclose(input);
            fclose(output);
            free(celle);
            return 1;
        }
    }
    for(int i=0;i<num_celle;i++)
    {
        printf("cella letta %d \n", celle[i]);
    }
    calcola_prefissi(celle,num_celle,prefix_sum);
    for(int i=0;i<num_query;i++)
    {
        char query[10];
        if(fscanf(input, "%s", query) != 1)
        {
            printf("Error reading query from input file.\n");
            fclose(input);
            fclose(output);
            free(celle);
            return 1;
        }
        char query_type = query[0];
        printf("query letta %s\n", query);
        printf("query type %c\n", query_type);
        if(query_type == 'A')
        {
            int params[3];
            if (fscanf(input, "%d %d %d", &params[0], &params[1], &params[2]) != 3)
            {
                printf("Error reading parameters for query A from input file.\n");
                fclose(input);
                fclose(output);
                free(celle);
                return 1;
            }
            printf("parametri letti %d %d %d\n", params[0], params[1], params[2]);
            execute_query(celle, num_celle, query_type, params,prefix_sum);
        }
        else if(query_type == 'Q')
        {
            int params[2];
            if (fscanf(input, "%d %d", &params[0], &params[1]) != 2)
            {
                printf("Error reading parameters for query Q from input file.\n");
                fclose(input);
                fclose(output);
                free(celle);
                return 1;
            }
            printf("parametri letti %d %d\n", params[0], params[1]);
            int result = execute_query(celle, num_celle, query_type, params,prefix_sum);
            fprintf(output, "%d\n", result);
        }
    }
    fclose(input);
    fclose(output);
}