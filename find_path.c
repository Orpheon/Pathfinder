#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "node.h"

#define SIZE_OF_NODE 20
#define sign(x) ((0<x) - (x<0))

__declspec(dllexport) double get_command(double, double, double, double, double);
Node* find_nearest_node(double, double, Nodemap*);
Nodemap* find_path(Node*, Node*, Nodemap*);

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

        // Do an A* through the nodemap
        Nodemap* path = find_path(node, target, nodemap);
        if (path == 0)
        {
            // No path was found
            return 3;
        }

        // Take the path of the target node
        // and find the first command
        for (i=0; i<node->numconnections; i++)
        {
            if (node->connected_nodes[i]->x == path->nodes[1].x && node->connected_nodes[i]->y == path->nodes[1].y)
            {
                // This is the connection we want
                command = node->connected_commands[i];
            }
        }
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


Nodemap* find_path(Node *sourcenode, Node *targetnode, Nodemap *nodemap)
{
    // Standard A* algorithm, implemented after http://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode

    // Precalculate the heuristic for each node
    int i, tmp, tmp2;
    for (i=0; i<nodemap->numnodes; i++)
    {
        tmp = targetnode->x - nodemap->nodes[i].x;
        nodemap->nodes[i].distance_to_target = sign(tmp)*tmp;// sign(tmp)*tmp = abs(tmp)
        nodemap->nodes[i].path_length = -1;
        nodemap->nodes[i].path_history->numnodes = 0;
        nodemap->nodes[i].is_active = 0;
    }

    sourcenode->is_active = 1;
    sourcenode->path_length = 0;

    // The main loop
    Node *currentnode = 0, *othernode = 0;
    while (1)
    {
        currentnode = 0;
        tmp = INT_MAX;
        tmp2 = 0;
        for (i=0; i<nodemap->numnodes; i++)
        {
            if (nodemap->nodes[i].is_active == 1)
            {
                tmp2 = 1;
                if (nodemap->nodes[i].distance_to_target + nodemap->nodes[i].path_length < tmp)
                {
                    currentnode = nodemap->nodes + i;
                    tmp = currentnode->distance_to_target + currentnode->path_length;
                }
            }
        }

        // If there aren't any active nodes in the set anymore, break and return failure
        if (tmp2 == 0)
        {
            return 0;
        }

        // Currentnode is now the node with the lowest estimated cost that still should be evaluated

        // Add ourselves to the history
        currentnode->path_history->nodes[currentnode->path_history->numnodes++] = *currentnode;

        // If we got to the target node, return success!
        if (currentnode == targetnode)
        {
            return targetnode->path_history;
        }

        // Remove currentnode from the active nodes
        currentnode->is_active = 0;

        // Go through all neighbours
        for (i=0; i<currentnode->numconnections; i++)
        {
            othernode = currentnode->connected_nodes[i];
            // Find estimated cost
            tmp = currentnode->path_length + currentnode->connected_distances[i];

            // If the othernode wasn't checked yet or this path is shorter than the previous ones
            if ((othernode->path_length == -1) || (tmp < othernode->path_length))
            {
                // Activate the othernode again
                othernode->is_active = 1;
                // Update both the new path length and the new history
                othernode->path_length = tmp;
                othernode->path_history->numnodes = currentnode->path_history->numnodes;
                memcpy(othernode->path_history->nodes, currentnode->path_history->nodes, currentnode->path_history->numnodes*sizeof(Node));
            }
        }
    }
}
