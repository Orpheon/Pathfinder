#ifndef NODE_H
#define NODE_H

typedef struct Node Node;
typedef struct NodeLinkedList NodeLinkedList;

struct Node
{
    NodeLinkedList *connected_nodes = 0;
    unsigned int connected_distances[4];
    char connected_commands[4];
    unsigned int x;
    unsigned int y;
    // Temporary variables
    unsigned int path_length;
    NodeLinkedList *path_history;
};

struct NodeLinkedList
{
    Node *value = 0;
    NodeLinkedList *next = 0;
};

#endif
