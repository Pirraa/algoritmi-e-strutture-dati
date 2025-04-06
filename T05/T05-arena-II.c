#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

// Dimensione massima dello stack
#define MAX_STACK_SIZE 100

// Stack per i calcoli
int stack[MAX_STACK_SIZE];
int top = -1;

// Funzione per mettere un elemento nello stack
void push(int value) {
    if (top < MAX_STACK_SIZE - 1) {
        stack[++top] = value;
    } else {
        printf("Stack overflow\n");
        exit(1);
    }
}

// Funzione per estrarre un elemento dallo stack
int pop() {
    if (top >= 0) {
        return stack[top--];
    } else {
        printf("Stack underflow\n");
        exit(1);
    }
}

// Funzione per gestire l'operatore '.'
void handle_dot_operator() {
    int b = pop();
    int a = pop();
    push(a + b);
}

// Funzione per gestire l'operatore '@'
void handle_at_operator() {
    int c = pop();
    int b = pop();
    int a = pop();
    push(a * (b - c));
}

// Funzione per gestire l'operatore '#'
void handle_hash_operator() {
    int b = pop();
    int a = pop();
    int result = 1;
    for (int i = 0; i < a; i++) {
        result *= b;
    }
    push(result);
}

// Funzione per gestire l'operatore '$'
void handle_dollar_operator() {
    int b = pop();
    int a = pop();
    push((a - 1) * (b - 1));
}

// Funzione per gestire l'operatore 'M' (max tra più valori)
void handle_M_operator() {
    int max_val = INT_MIN;  // Iniziamo con il valore minimo possibile
    int temp_stack[MAX_STACK_SIZE];
    int count = 0;

    // Copiamo tutti gli elementi nello stack temporaneo
    while (top >= 0) {
        temp_stack[count++] = pop();
    }

    // Troviamo il massimo tra tutti i valori
    for (int i = 0; i < count; i++) {
        if (temp_stack[i] > max_val) {
            max_val = temp_stack[i];
        }
    }

    // Rimettiamo il massimo nello stack
    push(max_val);
}

// Funzione per gestire l'operatore 'm' (minimo di più valori)
void handle_m_operator() {
    int min_val = INT_MAX;  // Iniziamo con il valore massimo possibile
    int temp_stack[MAX_STACK_SIZE];
    int count = 0;

    // Copiamo tutti gli elementi nello stack temporaneo
    while (top >= 0) {
        temp_stack[count++] = pop();
    }

    // Troviamo il minimo tra tutti i valori
    for (int i = 0; i < count; i++) {
        if (temp_stack[i] < min_val) {
            min_val = temp_stack[i];
        }
    }

    // Rimettiamo il minimo nello stack
    push(min_val);
}

// Funzione principale per valutare l'espressione in RPN
void evaluate_rpn(char* expression) {
    char* token = strtok(expression, " ");

    while (token != NULL) {
        if (isdigit(token[0])) { // Se il token è un numero
            push(atoi(token));  // Converte il token in intero
        } else if (token[0] == '.') {
            handle_dot_operator();
        } else if (token[0] == '@') {
            handle_at_operator();
        } else if (token[0] == '#') {
            handle_hash_operator();
        } else if (token[0] == '$') {
            handle_dollar_operator();
        } else if (token[0] == 'M') {
            handle_M_operator();
        } else if (token[0] == 'm') {
            handle_m_operator();
        }

        token = strtok(NULL, " ");
    }
}

// Funzione per leggere l'input e scrivere l'output
int main() {
    char expression[256];
    
    // Leggi l'input dal file "input.txt"
    FILE* input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        printf("Errore nell'aprire il file di input.\n");
        return 1;
    }
    
    if (fgets(expression, sizeof(expression), input_file) == NULL) {
        // Gestione dell'errore: se fgets restituisce NULL, significa che c'è stato un errore nella lettura
        printf("Errore nella lettura del file di input.\n");
        fclose(input_file);
        return 1;
    }
    fclose(input_file);

    // Rimuovi il carattere di newline se presente
    expression[strcspn(expression, "\n")] = 0;

    // Calcola l'espressione
    evaluate_rpn(expression);

    // Scrivi il risultato su "output.txt"
    FILE* output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Errore nell'aprire il file di output.\n");
        return 1;
    }
    
    fprintf(output_file, "%d\n", stack[top]); // L'ultimo valore nello stack è il risultato
    fclose(output_file);

    return 0;
}
