#include "node.h"

#define SIZE_OF_NODE 10

double get_command(double, double, char*);
Node* find_nearest_node(double, double, Nodemap*)
void update_node(Node*);
void clean_nodemap(Nodemap*);

double get_command(double my_x, double my_y, double target_x, double target_y, char* nodemap_pointer)
{
    Nodemap *nodemap = nodemap_pointer;
    double command = 0;
    for (int i=0; i<nodemap->numnodes; i++)
    {
        if (abs(nodemap->nodes[i].x - x) < SIZE_OF_NODE && abs(nodemap->nodes[i].y - y) < SIZE_OF_NODE)
        {
            // We are colliding with a node
            // Find a path and return a command

            // First find nearest node to target point
            Node *target = find_nearest_node(target_x, target_y, nodemap);
            // Make all nodes search
            nodemap->nodes[i].path_length = 0;
            update_node(nodemap->nodes[i]);
            // Take the path of the target node
            // and find the first command
            for (int j=0; j<nodemap->nodes[i].numconnections; j++)
            {
                if (nodemap->nodes[i].connected_nodes[j] == target->path_history.nodes[1])
                {
                    // This is the connection we want
                    command = nodemap->nodes[i].connected_commands[j];
                }
            }

            // Clean up and ready the nodemap for the next pathfinding
            clean_nodemap();

            break;
        }
    }

    return command
}


Node* find_nearest_node(double x, double y, Nodemap *nodemap)
{
    Node *nearest_node = NULL;
    float smallest_dist = 1000000, xdist, ydist, dist;
    for (int i=0; i<nodemap->numnodes; i++)
    {
        xdist = abs(x - nodemap->nodes[i].x);
        ydist = abs(y - nodemap->nodes[i].y);
        dist = xdist*xdist + ydist*ydist;// Who cares about the sqrt
        if (dist < smallest_dist)
        {
            smallest_dist = dist;
            nearestNode = &(nodemap->nodes[i]);
        }
    }
    return nearest_node;
}


void update_node(Node *currentnode)
{

}















NodeLinkedList* search_connections(Node*, Node*);

NodeLinkedList* find_path(Node *sourcenode, Node *targetnode)
{
    if (targetnode == sourcenode)
    {
        printf("Target node == source node");
        NodeLinkedList a;
        return a;
    }

    NodeLinkedList *path;
    path = search_connections(sourcenode, targetnode);
    if path->value == 0
    {
        print("No path possible");
        NodeLinkedList a;
        return a;
    }
}

NodeLinkedList* search_connections(Node* currentnode, Node* targetnode)
{
    NodeLinkedList *history_searcher = currentnode->path_history;
    while (true)
    {
        if (history_searcher->next == 0)
        {
            history_searcher->value = currentnode;
            history_searcher->next = history_searcher[1];
            break;
        }
        else
        {
            history_searcher = history_searcher[1];
        }
    }

    if (currentnode == targetnode)
    {
        return currentnode->path_history;
    }

    int i=0;
    NodeLinkedList *othernode_link = currentnode->connected_nodes;
    NodeLinkedList *path;
    while (true)
    {
        if ((othernode_link->value)->path_length > currentnode->path_length + currentnode->connected_distances[i])
        {
            othernode_link->path_length = currentnode->path_length + currentnode->connected_distances[i];
            *(othernode_link->path_history) = *(currentnode->path_history);
            path = search_connections(othernode_link->value, targetnode);
            if (path.value != 0)
            {
                return path;
            }
        }
        othernode_link = othernode_link->next;
        if (othernode_link == 0)
        {
            break;
        }
        i++;
    }
    // Return no path found here
    a = NodeLinkedList;
    return a;
}
