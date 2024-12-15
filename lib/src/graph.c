#include "lib/graph.h"

#include "lib/array.h"

#include <assert.h>
#include <string.h>

struct vertex_t {
    size_t index;
    char * value;
};

struct edge_t {
    vertex_t const * from;
    vertex_t const * to;
    char * value;
};

struct graph_t {
    size_t vertex_value_size;
    size_t edge_value_size;
    array_t * vertices;
    array_t * edges_from;
    array_t * edges_to;
};

void graph_t_add_vertex(graph_t * graph, vertex_t const * vertex) {
    array_t_append(graph->vertices, vertex);

    array_t * new_edges_from = array_t_new(sizeof(edge_t) + graph->edge_value_size);
    array_t_append(graph->edges_from, &new_edges_from);

    array_t * new_edges_to = array_t_new(sizeof(edge_t) + graph->edge_value_size);
    array_t_append(graph->edges_to, &new_edges_to);
}

void graph_t_add_edge(graph_t * graph, edge_t const * edge) {
    assert(array_t_length(graph->vertices) > edge->from->index && array_t_length(graph->vertices) > edge->to->index);

    array_t * edges_from = *(array_t **)array_t_at(graph->edges_from, edge->from->index);
    array_t_append(edges_from, edge);

    array_t * edges_to = *(array_t **)array_t_at(graph->edges_to, edge->to->index);
    array_t_append(edges_to, edge);
}

vertex_t * vertex_t_new(graph_t * graph, void const * value) {
    vertex_t * vertex = (vertex_t *)malloc(sizeof(vertex_t) + graph->vertex_value_size);

    vertex->index = array_t_length(graph->vertices);
    vertex->value = (char *)(vertex + 1);

    memcpy(vertex->value, value, graph->vertex_value_size);

    graph_t_add_vertex(graph, vertex);

    return vertex;
}

size_t vertex_t_size(graph_t const * graph) {
    return sizeof(vertex_t) + graph->vertex_value_size;
}

bool vertex_t_equality(void const *first, void const *second) {
    vertex_t const * first_ = first;
    vertex_t const * second_ = second;

    return first_->index == second_->index;
}

edge_t * edge_t_new(graph_t * graph, vertex_t const * from_vertex, vertex_t const * to_vertex, void const * value) {
    edge_t * edge = (edge_t *)malloc(sizeof(edge_t) + graph->edge_value_size);

    edge->from = from_vertex;
    edge->to = to_vertex;
    edge->value = (char *)(edge + 1);

    memcpy(edge->value, value, graph->edge_value_size);

    graph_t_add_edge(graph, edge);

    return edge;
}

size_t edge_t_size(graph_t const * graph) {
    return sizeof(edge_t) + graph->edge_value_size;
}

int vertex_t_compare(void const * first, void const * second) {
    vertex_t * first_ = (vertex_t *)first;
    vertex_t * second_ = (vertex_t *)second;

    if (first_->index == second_->index) {
        return 0;
    }

    return first_->index < second_->index ? -1 : 1;
}

graph_t * graph_t_new(size_t vertex_value_size, size_t edge_value_size) {
    graph_t * graph = (graph_t *)malloc(sizeof(graph_t));

    graph->vertex_value_size = vertex_value_size;
    graph->edge_value_size = edge_value_size;

    graph->vertices = array_t_new(sizeof(vertex_t) + vertex_value_size);
    array_t_sort(graph->vertices, vertex_t_compare);

    graph->edges_from = array_t_new(sizeof(array_t *));
    graph->edges_to = array_t_new(sizeof(array_t *));

    return graph;
}

array_t * graph_t_get_vertices(graph_t const * graph) {
    return graph->vertices;
}

array_t * graph_t_get_edges_from(graph_t const * graph, vertex_t const * vertex) {
    assert(vertex->index < array_t_length(graph->edges_from));
    return array_t_at(graph->edges_from, vertex->index);
}

array_t * graph_t_get_edges_to(graph_t const * graph, vertex_t const * vertex) {
    assert(vertex->index < array_t_length(graph->edges_to));
    return array_t_at(graph->edges_to, vertex->index);
}

void * vertex_t_value(vertex_t const * vertex) {
    return vertex->value;
}

void * edge_t_value(edge_t const * edge) {
    return edge->value;
}

vertex_t const * edge_t_to(edge_t const * edge) {
    return edge->to;
}
