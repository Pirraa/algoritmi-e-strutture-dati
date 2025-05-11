#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 1000

// Implementiamo un albero di ricerca binario

struct node {
    char name[MAX_LEN];

    int n_left;
    int n_right;

    struct node* left;
    struct node* right;
};

struct node* make_node(char* name) {
    struct node* node = (struct node*)malloc(sizeof(struct node));

    strncpy(node->name, name, MAX_LEN);

    node->n_left = node->n_right = 0;
    node->left = node->right = NULL;

    return node;
}

struct node* rotate_right(struct node* root) {
    if (root == NULL || root->left == NULL) {
        return root;
    }

    struct node* node = root->left;
    root->left = node->right;
    node->right = root;

    // attenzione a come aggiorniamo il numero di figli sinistri e destri dopo una rotazione
    root->n_left = (root->left ? root->left->n_left + root->left->n_right + 1 : 0);
    node->n_right = (node->right ? node->right->n_left + node->right->n_right + 1 : 0);

    return node;
}

struct node* rotate_left(struct node* root) {
    if (root == NULL || root->right == NULL) {
        return root;
    }

    struct node* node = root->right;
    root->right = node->left;
    node->left = root;

    root->n_right = (root->right ? root->right->n_left + root->right->n_right + 1 : 0);
    node->n_left = (node->left ? node->left->n_left + node->left->n_right + 1 : 0);

    return node;
}

struct node* insert(struct node* root, struct node* new_node)
{
    if (root == NULL) {
        return new_node;
    }

    // Inserimento nell'albero;
    // rendiamo il nostro albero un RBST (random binary search tree), aggiungendo delle 
    // rotazioni randomiche mentre scendiamo nell'albero per fare l'inserimento.
    // Senza questo trucchetto, c'è sempre un problema dietro l'angolo: quello di ricevere
    // un input cattivo, che ci rende l'albero sbilanciatissimo, cioè lo fa degenerare in una lista.

    // Commentando questo if, la complessità passa da QlogN a N^2
    int size = root->n_left + root->n_right;
    if (rand() % (size+1) == 0) {
        if (strcmp(new_node->name, root->name) < 0) {
            root->left = insert(root->left, new_node);
            root = rotate_right(root);
        } else {
            root->right = insert(root->right, new_node);
            root = rotate_left(root);
        }
        return root;
    }

    // Se non ruotiamo, allora facciamo l'inserimento standard.
    if (strcmp(new_node->name, root->name) < 0) {        
        root->left = insert(root->left, new_node);
        ++root->n_left;
    } else {
        root->right = insert(root->right, new_node);
        ++root->n_right;
    }

    return root;
}

struct node* search_k_smallest(struct node* root, int k)
{
    if (k == root->n_left + 1) {
        return root;
    }

    if (k <= root->n_left) {
        return search_k_smallest(root->left, k);
    } else {
        return search_k_smallest(root->right, k - root->n_left - 1);
    }
}

void free_tree(struct node* root) 
{
    if (root!= NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main(void)
{ 
    FILE *in_file = fopen("input.txt", "r");
    FILE *out_file = fopen("output.txt", "w");

    int N;
    int k;
    char name[MAX_LEN];
    struct node* root = NULL;

    fscanf(in_file, "%d", &N);

    for (int i=0; i<N; i++) {
        char command;
        fscanf(in_file, " %c", &command);

        // L'input cambia a seconda del primo carattere, A oppure C.
        if (command == 'A') {
            fscanf(in_file, "%1000s", name); 
            root = insert(root, make_node(name));
        } else {
            fscanf(in_file, "%d", &k);

            struct node* target = search_k_smallest(root, k);
            fprintf(out_file, "%s\n", target->name);
        }
    }

    free_tree(root);

    fclose(in_file);
    fclose(out_file);

    return 0;
}
