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

    int t = fast_expo(base, exp / 2);
    t = (t * t) % mod;

    if (exp % 2)
        return t;
    else 
        return ((base % mod) * t) % mod;
}

int main(void)
{
    FILE *in_file;
    FILE *out_file;
    in_file = fopen("input.txt", "r");
    out_file = fopen("output.txt", "w");  // 'w' mode to write, not 'r' which is for reading

    if (in_file == NULL || out_file == NULL) {
        return -1;
    }

    // Inizio logica principale del problema 
    int N;
    if (fscanf(in_file, "%d", &N) != 1) {  // Check that the fscanf was successful
        return -1;
    }

    while (N > 0) {
        int base, exp;
        if (fscanf(in_file, "%d %d", &base, &exp) != 2) {  // Check both values
            return -1;
        }
        
        int result = fast_expo(base, exp);  // Store the result of fast_expo
        if (fprintf(out_file, "%d\n", result) < 0) {  // Write the result to the output file
            return -1;
        }

        N--;
    }

    // Fine

    if (fclose(in_file) != 0 || fclose(out_file) != 0) {
        return -2;
    }

    return 0;
}
