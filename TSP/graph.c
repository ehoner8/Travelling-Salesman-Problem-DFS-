#include "graph.h"

#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct graph {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;
    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));
    g->weights = calloc(vertices, sizeof(g->weights[0]));

    for (uint32_t i = 0; i < vertices; i++) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }

    return g;
}

void graph_free(Graph **gp) {
    if (gp != NULL && *gp != NULL) {
        for (uint32_t i = 0; i < (*gp)->vertices; i++) {
            if ((*gp)->weights[i]) {
                free((*gp)->weights[i]);
                (*gp)->weights[i] = NULL;
            }
        }
        if ((*gp)->weights) {
            free((*gp)->weights);
            (*gp)->weights = NULL;
        }
        for (uint32_t i = 0; i < (*gp)->vertices; i++) {
            if ((*gp)->names[i]) {
                free((*gp)->names[i]);
                (*gp)->names[i] = NULL;
            }
        }
        if ((*gp)->names) {
            free((*gp)->names);
            (*gp)->names = NULL;
        }
        if ((*gp)->visited) {
            free((*gp)->visited);
            (*gp)->visited = NULL;
        }

        free(*gp);
    }

    if (gp != NULL) {
        *gp = NULL;
    }
}

uint32_t graph_vertices(const Graph *g) {
    return g->vertices;
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    if (g->names[v])
        free(g->names[v]);
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    return g->names[v];
}

char **graph_get_names(const Graph *g) {
    return g->names;
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    g->weights[start][end] = weight;
    if (!(g->directed)) {
        g->weights[end][start] = weight;
    }
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    return g->weights[start][end];
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = false;
}

bool graph_visited(const Graph *g, uint32_t v) {
    if (g->visited[v] == true)
        return true;
    return false;
}

void graph_print(const Graph *g) {
    printf("The graph contains %u verticces\n", g->vertices);
    if (g->directed) {
        printf("The graph is directed\n");
    } else {
        printf("The graph is undirected\n");
    }
    printf("The vertices with the following indices are visited:");
    for (uint32_t v = 0; v < g->vertices; v++) {
        if (graph_visited(g, v)) {
            printf(" %u", v);
        }
    }
    printf("\n");
    printf("Here are the names of the vertices:\n");
    char **names_list = graph_get_names(g);
    for (uint32_t n = 0; n < g->vertices; n++) {
        printf("%s\n", names_list[n]);
    }
    printf("Now the weights\n");
    for (uint32_t s = 0; s < g->vertices; s++) {
        for (uint32_t e = 0; e < g->vertices; e++) {
            if (graph_get_weight(g, s, e)) {
                printf("%s(%u) to %s(%u): %u\n", g->names[s], s, g->names[e], e, g->weights[s][e]);
            }
        }
    }
}

/*
Graph *read_new_graph(char *input_filename, bool directed){
	FILE *infile = stdin;
	char line[PATH_MAX];
	char *token;
	uint32_t s;
	uint32_t e;
	uint32_t w;
	Graph *g;
	infile = fopen(input_filename, "r");
	if (infile == NULL){
		return NULL;
	}
	fgets(line, PATH_MAX, infile);
	uint32_t a = (uint32_t) atoi(line);
	printf("%d\n", a);
	g = graph_create(a, directed);
	for (uint32_t i = 0; i<a; i++){
		fgets(line, PATH_MAX, infile);
		printf("%s", line);
		graph_add_vertex(g, line, i); 
	}

	fgets(line, PATH_MAX, infile);
	uint32_t b = (uint32_t) atoi(line);
	for (uint32_t i = 0; i<b; i++){
		fgets(line, PATH_MAX, infile);
		token = strtok(line, " ");
		s = (uint32_t) atoi(token);
		token = strtok(NULL, " ");
		e = (uint32_t) atoi(token);
		token = strtok(NULL, " ");
		w = (uint32_t) atoi(token);
		graph_add_edge(g, s, e, w);
		if (!directed){
			graph_add_edge(g, e, s, w);
		}
	}		
		
	fclose(infile);

	return g;	

}
*/
