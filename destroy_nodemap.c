#include <stdint.h>
#include "node.h"

__declspec(dllexport) double destroy_nodemap(double nodemap_pointer)
{
    Nodemap *nodemap = (Nodemap*)(uintptr_t)nodemap_pointer;

    int i;
    for(i=0; i<nodemap->numnodes; i++)
    {
        free(nodemap->nodes[i].path_history->nodes);
        free(nodemap->nodes[i].path_history);
    }
    free(nodemap->nodes);
    free(nodemap);

    return 0;
}
