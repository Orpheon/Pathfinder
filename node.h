#ifndef NODE_H
#define NODE_H

typedef struct Node Node;
typedef struct Nodemap Nodemap;

struct Node
{
    unsigned char numconnections;
    Node* connected_nodes[4];
    unsigned int connected_distances[4];
    char connected_commands[4];
    unsigned int x;
    unsigned int y;
    // Temporary variables
    int path_length;
    Nodemap *path_history;
};

struct Nodemap
{
    Node *nodes;
    int numnodes;
};

#endif
