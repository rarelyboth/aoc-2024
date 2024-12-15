#pragma once

#include "array.h"

typedef struct vertex_t vertex_t;
typedef struct edge_t edge_t;
typedef struct graph_t graph_t;

vertex_t * vertex_t_new(graph_t * graph, void const * value);
size_t vertex_t_size(graph_t const * graph);
bool vertex_t_equality(void const * first, void const * second);

edge_t * edge_t_new(graph_t * graph, vertex_t const * from_vertex, vertex_t const * to_vertex, void const * value);
size_t edge_t_size(graph_t const * graph);

graph_t * graph_t_new(size_t vertex_value_size, size_t edge_value_size);

array_t * graph_t_get_vertices(graph_t const * graph);
array_t * graph_t_get_edges_from(graph_t const * graph, vertex_t const * vertex);
array_t * graph_t_get_edges_to(graph_t const * graph, vertex_t const * vertex);

void * vertex_t_value(vertex_t const * vertex);
void * edge_t_value(edge_t const * edge);
vertex_t const * edge_t_to(edge_t const * edge);