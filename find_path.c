#include "node.h"

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
