#include <stdio.h>
#include <stdlib.h>

void insertionSort(int numeri[]) {
    for (int i = 1; i < 4; i++) {
        int key = numeri[i];
        int j = i - 1;

        while ((j >= 0) && (numeri[j] > key)) {
            numeri[j + 1] = numeri[j];
            j = j - 1;
        }
        numeri[j + 1] = key;
    }
}

int kaprekar(int numero) {
    int primo_numero = 0;
    int secondo_numero = 0;
    int counter = 0;

    do {
        int numbers[4];

        for (int i = 0; i < 4; i++) {
            numbers[i] = numero % 10;
            numero = numero / 10;
        }

        insertionSort(numbers);

        primo_numero = numbers[0] * 1000 + numbers[1] * 100 + numbers[2] * 10 + numbers[3];
        secondo_numero = numbers[3] * 1000 + numbers[2] * 100 + numbers[1] * 10 + numbers[0];

        numero = secondo_numero - primo_numero;

        counter++;

    } while ((secondo_numero - primo_numero) != 6174);

    return counter;
}

int main(int argc, char **argv) {
    FILE *input, *output;

    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    if (input == NULL || output == NULL) {
        return -1;  // Error opening files
    }

    int n = 0;
    if (fscanf(input, "%d", &n) != 1) {
        return -1;  // Error reading n
    }

    for (int i = 0; i < n; i++) {
        int numero = 0;
        if (fscanf(input, "%d", &numero) != 1) {
            return -1;  // Error reading number
        }

        int n_cicli = kaprekar(numero);

        // Scrive il risultato nel file di output
        fprintf(output, "%d\n", n_cicli);
    }

    fclose(input);
    fclose(output);

    return 0;
}
