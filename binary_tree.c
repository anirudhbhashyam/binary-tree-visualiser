#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// 2^10 nodes max capacity.
#define NODE_POOL_CAPACITY 1024

typedef struct Node Node;
struct Node
{
    int32_t data;
    Node* left;
    Node* right;
};

// Stack memory for tree.
static Node node_pool[NODE_POOL_CAPACITY] = { 0 };
static size_t pool_size = 0;

Node* get_node()
{
    assert(pool_size < NODE_POOL_CAPACITY);
    return &node_pool[pool_size++];
}

void insert_node(Node* root, int32_t data)
{
    assert(root != NULL);

    if (data <= root->data)
    {
        if (root->left == NULL)
        {
            root->left = get_node();
            root->left->data = data;
        }
        else
        {
            insert_node(root->left, data);
        }
    }
    else
    {
        if (root->right == NULL)
        {
            root->right = get_node();
            root->right->data = data;
        }
        else
        {
            insert_node(root->right, data);
        }
    }

    pool_size++;
}

Node* delete_node(Node* root, int32_t data)
{
    if (root == NULL) return root;

    if (data < root->data) 
    {
        root->left = delete_node(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = delete_node(root->right, data);
    }
    else
    {
        // No children.
        if (root->left == NULL && root->right == NULL) return NULL;

        // Only left child.
        if (root->left != NULL && root->right == NULL) return root->left;

        // Only right child.
        if (root->left == NULL && root->right != NULL) return root->right;

        // Both children.
        if (root->left != NULL && root->right != NULL)
        {
            // Need to find a leaf from here.
            Node* ptr = root->right;
            while (ptr->left != NULL) ptr = ptr->left;
            root->data = ptr->data;
            root->right = delete_node(root->right, ptr->data);
        }
    }

    pool_size--;

    return root;
}

Node* search_tree(Node* root, int32_t data)
{
    if (root == NULL || root->data == data) return root;

    if (root->data < data) search_tree(root->right, data);

    return search_tree(root->left, data);
}

void print_tree(Node* root)
{
    if (root == NULL) return;

    printf("%d\n", root->data);
    print_tree(root->left);
    print_tree(root->right);
}

void dump_tree(FILE* dst, Node* root)
{
    if (root == NULL) return;

    if (root->left != NULL)
        fprintf(dst, "\t%d -> %d\n", root->data, root->left->data);
    if (root->right != NULL)
        fprintf(dst, "\t%d -> %d\n", root->data, root->right->data);

    dump_tree(dst, root->left);
    dump_tree(dst, root->right);
}

Node* create_tree_from_file(const char* file)
{
    FILE* f = fopen(file, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Could not open file %s: %s", file, strerror(errno));
        exit(1);
    }

    char line[256];
    size_t n_nodes = 0;
    while (fgets(line, sizeof(line), f))
    {
        n_nodes += 1;
    }

    if (n_nodes > NODE_POOL_CAPACITY)
    {
        fprintf(stderr, "Maximum capacity exceeded. Reduce the number of nodes.");
        exit(1);
    }

    rewind(f);

    Node* root = get_node();
    fgets(line, sizeof(line), f);
    root->data = atoi(line);

    while (fgets(line, sizeof(line), f))
    {
        insert_node(root, atoi(line));
    }

    fclose(f);    

    return root;
}

int main(void)
{
    Node* root = create_tree_from_file("data.txt");

    const char* dot_file = "bst.dot";
    FILE* f = fopen(dot_file, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Could not open file %s: %s", dot_file, strerror(errno));
        exit(1);
    }
    fprintf(f, "digraph {\n");
    dump_tree(f, root);
    fprintf(f, "}");
    if (fclose(f) != 0)
    {
        fprintf(stderr, "Could not open file %s: %s", dot_file, strerror(errno));
        exit(1);
    }
    return 0;
}
