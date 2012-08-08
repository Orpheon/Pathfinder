#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "node.h"

#define SIZE_OF_NODE 20

__declspec(dllexport) double get_command(double, double, double, double, double);
Node* find_nearest_node(double, double, Nodemap*);
void update_node(Node*);
void clean_nodemap(Nodemap*);

__declspec(dllexport) double get_command(double my_x, double my_y, double target_x, double target_y, double nodemap_pointer)
{
    Nodemap *nodemap = (Nodemap*)(uintptr_t)nodemap_pointer;
    double command = 0;
    int i;
    Node *node;
    node = find_nearest_node(my_x, my_y, nodemap);
    if (abs(node->x - my_x) < SIZE_OF_NODE && abs(node->y - my_y) < SIZE_OF_NODE)
    {
        // We are colliding with a node
        // Find a path and return a command

        // First find nearest node to target point
        Node *target = find_nearest_node(target_x, target_y, nodemap);
        // Make all nodes search
        node->path_length = 0;
        update_node(node);

        // Take the path of the target node
        // and find the first command
        for (i=0; i<node->numconnections; i++)
        {
            if (node->connected_nodes[i]->x == target->path_history->nodes[1].x && node->connected_nodes[i]->y == target->path_history->nodes[1].y)
            {
                // This is the connection we want
                command = node->connected_commands[i];
            }
        }

        // Clean up and ready the nodemap for the next pathfinding
        clean_nodemap(nodemap);
    }

    return command;
}


Node* find_nearest_node(double x, double y, Nodemap *nodemap)
{
    Node *nearest_node = 0;
    float smallest_dist = INT_MAX, xdist, ydist, dist;
    int i;
    for (i=0; i<nodemap->numnodes; i++)
    {
        xdist = abs(x - nodemap->nodes[i].x);
        ydist = abs(y - nodemap->nodes[i].y);
        dist = xdist*xdist + ydist*ydist;// Who cares about the sqrt
        if (dist < smallest_dist)
        {
            smallest_dist = dist;
            nearest_node = nodemap->nodes + i;
        }
    }
    return nearest_node;
}


void update_node(Node *currentnode)
{
    int i;
    Node *othernode;

    // Let currentnode add itself to it's history
    currentnode->path_history->nodes[currentnode->path_history->numnodes++] = *currentnode;

    for (i=0; i<currentnode->numconnections; i++)
    {
        othernode = currentnode->connected_nodes[i];
        // If the othernode hasn't been considered yet or the path via currentnode is shorter than the path the othernode already has
        if ((othernode->path_length == -1) || (currentnode->connected_distances[i] + currentnode->path_length < othernode->path_length))
        {
            othernode = currentnode->connected_nodes[i];
            // Update the othernode
            othernode->path_history->numnodes = currentnode->path_history->numnodes;
            memcpy(othernode->path_history->nodes, currentnode->path_history->nodes, currentnode->path_history->numnodes * sizeof(Node));
            othernode->path_length = currentnode->path_length + currentnode->connected_distances[i];
            // Let it update it's neighbours
            update_node(othernode);
        }
    }
}


void clean_nodemap(Nodemap *nodemap)
{
    int i;
    for (i=0; i<nodemap->numnodes; i++)
    {
        // Clear the slate
        nodemap->nodes[i].path_length = -1;
        nodemap->nodes[i].path_history->numnodes = 0;
    }
}
