#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int indice = 0;

int min(int a, int b, int c)
{
    if (a <= b && a <= c)
        return a;
    if (b <= a && b <= c)
        return b;
    return c;
}

int levenshtein_distance(char *sequenza_ricercatori, char *sequenza_database)
{
    indice++;
    // printf("=============================================\n\t%d\n", indice);

    // printf("Ricerca:\t%s\n", sequenza_ricercatori);
    // printf("Database:\t%s\n", sequenza_database);

    int len_r = strlen(sequenza_ricercatori);
    int len_d = strlen(sequenza_database);

    int dp[len_r + 1][len_d + 1];

    for (int i = 0; i <= len_r; i++)
        dp[i][0] = i;
    for (int j = 0; j <= len_d; j++)
        dp[0][j] = j;

    for (int i = 1; i <= len_r; i++)
    {
        for (int j = 1; j <= len_d; j++)
        {
            int costo = sequenza_ricercatori[i - 1] == sequenza_database[j - 1] ? 0 : 1;

            dp[i][j] = min(
                dp[i - 1][j] + 1,
                dp[i][j - 1] + 1,
                dp[i - 1][j - 1] + costo);
        }
    }

    int result = dp[len_r][len_d];

    // printf("Risultato: %d\n", result);

    return result;
}

int main(int argc, char **argv)
{
    FILE *input, *output;

    input = fopen("input.txt", "r");
    if (input == NULL)
    {
        printf("[Errore]: Apertura file di input.");
        exit(1);
    }

    output = fopen("output.txt", "w");
    if (output == NULL)
    {
        printf("[Errore]: Apertura file di output.");
        exit(1);
    }

    int n = 0;

    int result = fscanf(input, "%d", &n);

    if (result != 1)
    {
        printf("[Errore]: Lettura numero di righe.");
        exit(1);
    }

    if (n <= 0)
    {
        printf("[Errore]: Numero di righe da leggere errato.");
        exit(1);
    }

    int m = 0;

    result = fscanf(input, "%d", &m);

    if (result != 1)
    {
        printf("[Errore]: Lettura del numero della sequenza dei ricercatori.");
        exit(1);
    }

    char *sequenza_ricercatori = (char *)malloc(m * sizeof(char));

    result = fscanf(input, "%s", sequenza_ricercatori);

    if (result != 1)
    {
        printf("[Errore]: Lettura della sequenza dei ricercatori.");
        exit(1);
    }

    int min = __INT_MAX__;
    int result_levenshtein_distance = 0;

    for (int i = 0; i < n; i++)
    {
        int m = 0;

        result = fscanf(input, "%d", &m);

        if (result != 1)
        {
            printf("[Errore]: Lettura del numero della sequenza del database.");
            exit(1);
        }

        char *sequenza_database = (char *)malloc(m * sizeof(char));

        result = fscanf(input, "%s", sequenza_database);

        if (result != 1)
        {
            printf("[Errore]: Lettura della sequenza del database.");
            exit(1);
        }

        result_levenshtein_distance = levenshtein_distance(sequenza_ricercatori, sequenza_database);

        if (result_levenshtein_distance < min)
        {
            min = result_levenshtein_distance;
        }
    }

    fprintf(output, "%d", min);

    if (fclose(input) != 0)
    {
        printf("[Errore]: Chiusura file di input.");
        exit(1);
    }

    if (fclose(output) != 0)
    {
        printf("[Errore]: Chiusura file di output.");
        exit(1);
    }

    return 0;
}