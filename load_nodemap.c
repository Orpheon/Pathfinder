// Works, unit tested
#include <stdio.h>
#include "node.h"
#include "bufferoperations.h"

char* readfile(char* name);

char* load_nodemap(char* filename)
{
    unsigned char *buffer = readfile(filename);
    int offset = 0;

    offset++;// This one skipped byte is the version byte, I don't think we really need this yet

    int numnodes = read_uint(buffer, offset);
    offset += 4;

    Nodemap *nodemap = calloc(1, sizeof(Nodemap));
    nodemap->numnodes = numnodes;
    nodemap->nodes = calloc(numnodes, sizeof(Node));

    if (nodemap == NULL)
    {
        printf("ERROR: Could not allocate memory for calloc!");
        return;
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
    unsigned int numconnections, j;

    for (i=0; i<numnodes; i++)
    {
        numconnections = buffer[offset++];
        nodemap->nodes[i].numconnections = numconnections;
        for (j=0; j<numconnections; j++)
        {
            nodemap->nodes[i].connected_nodes[j] = nodemap->nodes + read_uint(buffer, offset);
            offset += 4;
            nodemap->nodes[i].connected_distances[j] = read_uint(buffer, offset);
            offset +=4;
            nodemap->nodes[i].connected_commands[j] = buffer[offset++];
        }
        // Yes, I know, this is a lot, but it's necessary
        nodemap->nodes[i].path_history = calloc(1, sizeof(Nodemap));
        nodemap->nodes[i].path_history->nodes = calloc(numnodes, sizeof(Node*));
    }

    char* string_pointer = nodemap;
    return string_pointer;
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
