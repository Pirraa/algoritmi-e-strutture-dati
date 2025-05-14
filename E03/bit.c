#include <stdio.h>
#include <stdlib.h>

#define lsb(i) ((i) & -(i))

void add(int *bit, int n, int idx, int val) {
    while (idx <= n) {
        bit[idx] += val;
        idx += lsb(idx);
    }
}

//aggiunge val a tutti gli indici da l a r
void updateRange(int *BIT1, int *BIT2, int n, int l, int r, int val) {
    add(BIT1, n, l, val);
    add(BIT1, n, r + 1, -val);
    add(BIT2, n, l, val * (l - 1));
    add(BIT2, n, r + 1, -val * r);
}

void buildBIT(int *BIT1, int *BIT2, int *arr, int n) {
    for (int i = 1; i <= n; i++) {
        updateRange(BIT1, BIT2, n, i, i, arr[i - 1]); // inizializza ogni valore
    }
}



//somma da 1 a idx
int prefixSum(int *bit, int idx) {
    int sum = 0;
    while (idx > 0) {
        sum += bit[idx];
        idx -= lsb(idx);
    }
    return sum;
}

int queryPrefix(int *BIT1, int *BIT2, int idx) {
    return prefixSum(BIT1, idx) * idx - prefixSum(BIT2, idx);
}

//somma elementi da l a r
int queryRange(int *BIT1, int *BIT2, int l, int r) {
    return queryPrefix(BIT1, BIT2, r) - queryPrefix(BIT1, BIT2, l - 1);
}




int main() {
    int n = 8;
    int arr[] = {5, 2, 6, 3, 7, 4, 1, 2};

    int *BIT1 = calloc(n + 1, sizeof(int));
    int *BIT2 = calloc(n + 1, sizeof(int));

    buildBIT(BIT1, BIT2, arr, n);

    // Somma da 2 a 5
    printf("Somma da 2 a 5: %d\n", queryRange(BIT1, BIT2, 2, 5)); // Output: 2+6+3+7 = 18

    // Aggiungi +3 da 3 a 6
    updateRange(BIT1, BIT2, n, 3, 6, 3);

    // Nuova somma da 2 a 5
    printf("Dopo update, somma da 2 a 5: %d\n", queryRange(BIT1, BIT2, 2, 5)); // Output: 2 + (6+3) + (3+3) + (7+3)

    free(BIT1);
    free(BIT2);
    return 0;
}
