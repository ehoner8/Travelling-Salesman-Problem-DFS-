#include "path.h"

#include "graph.h"
#include "stack.h"

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->total_weight = 0;
    p->vertices = stack_create(capacity);
    //p->vertices = path_create(capacity);
    return p;
}

void path_free(Path **pp) {
    if (pp != NULL && *pp != NULL) {
        if ((*pp)->vertices) {
            stack_free(&((*pp)->vertices));
        }

        free(*pp);
    }

    if (pp != NULL) {
        *pp = NULL;
    }
}

uint32_t path_vertices(const Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_distance(const Path *p) {
    return p->total_weight;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    uint32_t a;
    assert(stack_size(p->vertices) < stack_capacity(p->vertices));
    if (stack_empty(p->vertices)) {
        stack_push(p->vertices, val);
        return;
    }
    assert(stack_peek(p->vertices, &a));
    stack_push(p->vertices, val);
    p->total_weight += graph_get_weight(g, a, val);
}

uint32_t path_remove(Path *p, const Graph *g) {
    assert(!(stack_empty(p->vertices)));
    uint32_t removed;
    uint32_t peeked;
    stack_pop(p->vertices, &removed);
    if (stack_size(p->vertices) == 0) {
        return removed;
    }
    assert(stack_peek(p->vertices, &peeked));
    p->total_weight -= graph_get_weight(g, peeked, removed);
    return removed;
}

void path_copy(Path *dst, const Path *src) {
    dst->total_weight = src->total_weight;
    stack_copy(dst->vertices, src->vertices);
}

void path_print(const Path *p, FILE *outfile, const Graph *g) {
    //FILE *of = stdout;
    //of = fopen(outfile, "w");
    /*
	for (uint32_t i=0; i < graph_vertices(g); i++){
		fprintf(outfile, "%s\n", (graph_get_names(g))[i]);
	}*/

    stack_print(p->vertices, outfile, graph_get_names(g));

    //fclose(outfile);
}
