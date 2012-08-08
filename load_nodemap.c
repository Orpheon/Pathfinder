// Works, unit tested
#include <stdio.h>
#include <stdint.h>
#include "node.h"
#include "bufferoperations.h"

char* readfile(char* name);

__declspec(dllexport) double load_nodemap(char* filename)
{
    unsigned char *buffer = readfile(filename);
    int offset = 0;

    offset++;// This one skipped byte is the version byte, I don't think we really need this yet

    int numnodes = read_uint(buffer, offset);
    offset += 4;

    Nodemap *nodemap = calloc(1, sizeof(Nodemap));
    nodemap->numnodes = numnodes;
    nodemap->nodes = calloc(numnodes, sizeof(Node));

    if (nodemap == NULL || nodemap->nodes == NULL)
    {
        printf("ERROR: Could not allocate memory for the nodemap!");
        return -1;
    }

    int i, x, y;
    // Create all the nodes and put them into one huge array
    for (i=0; i<numnodes; i++)
    {
        x = read_uint(buffer, offset);
        offset += 4;
        y = read_uint(buffer, offset);
        offset += 4;
        nodemap->nodes[i].x = x;
        nodemap->nodes[i].y = y;
    }

    // All nodes created, now start depacking connection information
    unsigned int numconnections, j, counter=0;

    for (i=0; i<numnodes; i++)
    {
        numconnections = buffer[offset++];
        counter += numconnections;
        nodemap->nodes[i].numconnections = numconnections;
        for (j=0; j<numconnections; j++)
        {
            nodemap->nodes[i].connected_nodes[j] = nodemap->nodes + read_uint(buffer, offset);
            offset += 4;
            nodemap->nodes[i].connected_distances[j] = read_uint(buffer, offset);
            offset += 4;
            nodemap->nodes[i].connected_commands[j] = buffer[offset++];
        }
        nodemap->nodes[i].path_length = -1;
        nodemap->nodes[i].path_history = calloc(1, sizeof(Nodemap));
        nodemap->nodes[i].path_history->nodes = calloc(numnodes, sizeof(Node));
        if (nodemap->nodes[i].path_history == NULL || nodemap->nodes[i].path_history->nodes == NULL)
        {
            printf("ERROR: Could not allocate memory for path_history!");
            return -1;
        }
    }
    printf("Num connections:%i", counter);

    double r = (uintptr_t)nodemap;
    return r;
}

char* readfile(char *name)
{
	FILE *file;
	char *buffer;
	unsigned long fileLen;

	//Open file
	file = fopen(name, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", name);
		return;
	}

	//Get file length
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate memory
	buffer=(unsigned char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
        fclose(file);
		return;
	}

	//Read file contents into buffer
	fread(buffer, fileLen, 1, file);
	fclose(file);

    return buffer;
}
