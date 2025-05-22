#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000

int tree[4 * MAX_SIZE];
int lazy[4 * MAX_SIZE];

// Costruzione base
void build(int arr[], int node, int start, int end) {
    if (start == end) {
        tree[node] = arr[start];
    } else {
        int mid = (start + end) / 2;
        build(arr, 2 * node, start, mid);
        build(arr, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
}

// Funzione per propagare i ritardi
void propagate(int node, int start, int end) {
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
void update_range(int node, int start, int end, int l, int r, int val) {
    propagate(node, start, end);

    if (r < start || end < l) return; // fuori intervallo
    if (l <= start && end <= r) {
        lazy[node] += val;
        propagate(node, start, end);
        return;
    }

    int mid = (start + end) / 2;
    update_range(2 * node, start, mid, l, r, val);
    update_range(2 * node + 1, mid + 1, end, l, r, val);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// Query somma in intervallo [l, r]
int query(int node, int start, int end, int l, int r) {
    propagate(node, start, end);

    if (r < start || end < l) return 0; // fuori intervallo
    if (l <= start && end <= r) return tree[node];

    int mid = (start + end) / 2;
    int left_sum = query(2 * node, start, mid, l, r);
    int right_sum = query(2 * node + 1, mid + 1, end, l, r);
    return left_sum + right_sum;
}

// Esempio d'uso
int main() {
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    build(arr, 1, 0, n - 1);

    printf("Somma [1,3] = %d\n", query(1, 0, n - 1, 1, 3)); // 3+5+7=15

    update_range(1, 0, n - 1, 1, 3, 10); // aggiungi 10 da 1 a 3

    printf("Somma [1,3] dopo update = %d\n", query(1, 0, n - 1, 1, 3)); // (3+10)+(5+10)+(7+10) = 45

    return 0;
}
