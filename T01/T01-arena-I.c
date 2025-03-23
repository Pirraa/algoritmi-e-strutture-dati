#include <stdlib.h>
#include <stdio.h>

#define mod 1000000007

// https://en.wikipedia.org/wiki/Exponentiation_by_squaring
int fast_expo(int base, int exp)
{
    if (exp == 0)
        return 1;

    if (exp == 1)
        return base;

    int t = fast_expo(base, exp/2);
    t = (t*t) % mod;

    if (exp % 2)
        return t;
    else 
        return ((base % mod) * t) % mod;
}

int main(void)
{
    FILE* in_file;
    FILE* out_file;
    in_file = fopen("input.txt", "r");
    out_file = fopen("output.txt", "r");

    if (in_file == NULL || out_file == NULL) {
        return -1;
    }

    // Inizio logica principale del problema 
    int N;
    fscanf(in_file, "%d", &N);
    while(N > 0) {
        int base, exp;
        fscanf(in_file, "%d %d", &base, &exp);
        fscanf(out_file, "%d", fast_expo(base, exp));
        N--;
    }
    // Fine

    if (fclose(in_file) != 0 || fclose(out_file) != 0) {
        return -2;
    }

    return 0;
}