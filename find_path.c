#include "node.h"

#define SIZE_OF_NODE 10

double get_command(double, double, double, double, char*);
Node* find_nearest_node(double, double, Nodemap*);
void update_node(Node*);
void clean_nodemap(Nodemap*);

double get_command(double my_x, double my_y, double target_x, double target_y, char* nodemap_pointer)
{
    Nodemap *nodemap = nodemap_pointer;
    double command = 0;
    int i, j;
    for (i=0; i<nodemap->numnodes; i++)
    {
        if (abs(nodemap->nodes[i].x - my_x) < SIZE_OF_NODE && abs(nodemap->nodes[i].y - my_y) < SIZE_OF_NODE)
        {
            // We are colliding with a node
            // Find a path and return a command

            // First find nearest node to target point
            Node *target = find_nearest_node(target_x, target_y, nodemap);
            // Make all nodes search
            nodemap->nodes[i].path_length = 0;
            update_node(&nodemap->nodes[i]);
            // Take the path of the target node
            // and find the first command
            for (j=0; j<nodemap->nodes[i].numconnections; j++)
            {
                if (nodemap->nodes[i].connected_nodes[j] == &target->path_history->nodes[1])
                {
                    // This is the connection we want
                    command = nodemap->nodes[i].connected_commands[j];
                }
            }

            // Clean up and ready the nodemap for the next pathfinding
            clean_nodemap(nodemap);

            break;
        }
    }

    return command;
}


Node* find_nearest_node(double x, double y, Nodemap *nodemap)
{
    Node *nearest_node = 0;
    float smallest_dist = 1000000, xdist, ydist, dist;
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
        // If the othernode hasn't been considered yet or the path via currentnode is shorter than the path the othernode already has
        if (othernode->path_length == -1 || currentnode->connected_distances[i] + currentnode->path_length < othernode->path_length)
        {
            othernode = currentnode->connected_nodes[i];
            // Update the othernode
            othernode->path_history->numnodes = currentnode->path_history->numnodes;
            memcpy(othernode->path_history->nodes, currentnode->path_history->nodes, currentnode->path_history->numnodes * sizeof(Node*));
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
