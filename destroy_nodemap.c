#include "node.h"

double destroy_nodemap(double nodemap_pointer)
{
    NodeLinkedList *node = nodemap_pointer, nextnode;
    while (true)
    {
        free((node->value)->connected_nodes);
        free((node->value)->connected_distances);
        free((node->value)->connected_commands);
        free((node->value)->path_history);
        free(node->value);
        nextnode = node->next;
        free(node);
        if (nextnode == 0)
        {
            break;
        }
        else
        {
            node = nextnode;
        }
    }
}
