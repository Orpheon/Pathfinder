#include <stdio.h>
#include "node.h"
#include "bufferoperations.h"

char* readfile(char* name);

double load_nodemap(char* filename)
{
    unsigned char *buffer = readfile(filename);
    int offset = 0;

    offset++;// This one skipped byte is the version byte, I don't think we really need this yet

    int numnodes = read_uint(buffer, offset);
    offset += 4;

    NodeLinkedList *nodemap = malloc(numnodes*sizeof(NodeLinkedList));
    Node *nodearray = malloc(numnodes*sizeof(Node));

    if (nodemap == NULL)
    {
        printf("ERROR: Could not allocate memory for malloc!");
        return;
    }

    int i, x, y;
    // Create all the nodes and put them into one huge array
    for (i=0; i<numnodes; i++)
    {
        x = read_uint(buffer, offset);
        offset += 4;
        nodearray[i].x = x;
        y = read_uint(buffer, offset);
        offset += 4;
        nodearray[i].y = y;

        nodemap[i].value = &(nodearray[i]);
        if (i<numnodes+1)
        {
            nodemap[i].next = &(nodemap[i+1]);
        }
    }

    // All nodes exist, now start depacking connection information
    NodeLinkedList *connections;
    unsigned int numconnections, j;

    for (i=0; i<numnodes; i++)
    {
        numconnections = buffer[offset++];
        connections = malloc(numconnections * sizeof(*NodeLinkedList));
        nodearray[i].connected_nodes = connections;
        for (j=0; j<numconnections; j++)
        {
            connections->value = &(nodearray[read_uint(buffer, offset)]);
            connections->next = connections+sizeof(NodeLinkedList);
            connections = connections->next;
            offset += 4;
            nodearray[i].connected_distances[j] = read_uint(buffer, offset);
            offset +=4;
            nodearray[i].connected_commands[j] = buffer[offset++];
        }
        // Yes, I know, this is a lot, but it's necessary
        nodearray[i].path_history = malloc(sizeof(NodeLinkedList)*numnodes);
    }

    return (double) nodemap;
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
