#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <assert.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "hdi:o:"

Graph *read_new_graph(FILE *infile, bool directed) {
    //FILE *infile = stdin;
    char line[PATH_MAX];
    char *token;
    uint32_t s;
    uint32_t e;
    uint32_t w;
    Graph *g;
    //infile = fopen(input_filename, "r");
    if (infile == NULL) {
        return NULL;
    }
    fgets(line, PATH_MAX, infile);
    uint32_t a = (uint32_t) atoi(line);
    //printf("%d\n", a);
    g = graph_create(a, directed);
    for (uint32_t i = 0; i < a; i++) {
        fgets(line, PATH_MAX, infile);
        line[strlen(line) - 1] = 0;
        //printf("%s", line);
        graph_add_vertex(g, line, i);
    }

    fgets(line, PATH_MAX, infile);
    uint32_t b = (uint32_t) atoi(line);
    for (uint32_t i = 0; i < b; i++) {
        fgets(line, PATH_MAX, infile);
        token = strtok(line, " ");
        s = (uint32_t) atoi(token);
        token = strtok(NULL, " ");
        e = (uint32_t) atoi(token);
        token = strtok(NULL, " ");
        w = (uint32_t) atoi(token);
        graph_add_edge(g, s, e, w);
        if (!directed) {
            graph_add_edge(g, e, s, w);
        }
    }

    //fclose(infile);

    return g;
}

void dfs(Path *cp, Path *bpp, uint32_t n, uint32_t start, Graph *g) {
    graph_visit_vertex(g, n);
    path_add(cp, n, g);
    if (path_vertices(cp) == graph_vertices(g)) {
        if (graph_get_weight(g, n, start) > 0) {
            if (path_distance(bpp) == 0) {
                path_add(cp, start, g);
                path_copy(bpp, cp);
                path_remove(cp, g);
            }
            if (path_distance(cp) + graph_get_weight(g, n, start) < path_distance(bpp)) {
                path_add(cp, start, g);
                path_copy(bpp, cp);
                path_remove(cp, g);
            }
        }
    }
    for (uint32_t i = 0; i < graph_vertices(g); i++) {
        if (graph_get_weight(g, n, i) > 0) {
            if (!(graph_visited(g, i))) {
                dfs(cp, bpp, i, start, g);
            }
        }
    }
    graph_unvisit_vertex(g, n);
    path_remove(cp, g);
}

int main(int argc, char **argv) {

    char helpmessage[]
        = "Usage: tsp [options]\n\n-i infile    Specify the input file path containing the cities "
          "and edges\n             of a graph. If not specified, the default input should be\n     "
          "        set as stdin.\n\n-i infile    Specify the input file path containing the cities "
          "and edges of a graph. If not specified,\n             the default input should be set "
          "as stdin.\n\n-o outfile   Specify the output file path to print to. If not specified, "
          "the default output should be set as stdout.\n\n-d           Specifies the graph to be "
          "directed.\n\n-h           Prints out a help message describing the purpose of the\n     "
          "        graph and the command-line options it accepts, exiting the\n             "
          "program afterwards.";
    int opt;
    opterr = 0;
    bool directed = false;
    int inp = 0;
    int outp = 0;
    char *inputf;
    char *outputf;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': printf("%s\n", helpmessage); return 0;
        case 'd': directed = true; break;
        case 'i':
            inp = 1;
            inputf = optarg;
            break;
        case 'o':
            outp = 1;
            outputf = optarg;
            break;
        default:
            fprintf(stderr, "tsp:  unknown or poorly formatted option -%c\n", optopt);
            return 1;
        }
    }

    Graph *g;
    Path *cp;
    Path *bpp;
    if (inp == 1) {
        FILE *in = fopen(inputf, "r");
        g = read_new_graph(in, directed);
        fclose(in);
    } else {
        g = read_new_graph(stdin, directed);
    }
    cp = path_create(graph_vertices(g) + 1);
    bpp = path_create(graph_vertices(g) + 1);
    uint32_t start = START_VERTEX;
    dfs(cp, bpp, start, start, g);
    if (path_distance(bpp) == 0) {
        if (outp == 1) {
            FILE *outfail = fopen(outputf, "w");
            fprintf(outfail, "No path found! Alissa is lost!\n");
            fclose(outfail);
            return 0;
        }
        printf("No path found! Alissa is lost!\n");
        return 0;
    }

    if (outp == 1) {
        FILE *out = fopen(outputf, "w");
        fprintf(out, "Alissa starts at:\n");
        path_print(bpp, out, g);
        fprintf(out, "Total Distance: %u\n", path_distance(bpp));
        fclose(out);
    } else {
        printf("Alissa starts at:\n");
        path_print(bpp, stdout, g);
        printf("Total Distance: %u\n", path_distance(bpp));
    }

    path_free(&bpp);
    path_free(&cp);
    graph_free(&g);

    //printf("Hello World\n");

    return 0;
}
