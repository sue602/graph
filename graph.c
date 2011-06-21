//
//  graph.c
//  adjacencylist
//
//  Created by Sam Goldman on 6/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "graph.h"
#include <stdlib.h>

int compare_edges(const void *aa, const void *bb);
void vertex_free(void *data);

int compare_edges(const void *aa, const void *bb) {
    const Edge *a = aa;
    const Edge *b = bb;
    return b->weight - a->weight;
}

void vertex_free(void *data) {
    Vertex *vertex = data;
    list_free(vertex->edges);
    free(vertex);
}

Graph *graph_create(void) {
	Graph *graph = malloc(sizeof(Graph));
	graph->vertices = list_create(vertex_free);
	graph->count = 0;
	return graph;
}

Vertex *vertex_create(void *data) {
	Vertex *vertex = malloc(sizeof(Vertex));
	vertex->data = data;
	vertex->edges = list_create(free);
	return vertex;
}

Edge *edge_create(Vertex *vertex, double weight) {
	Edge *edge = malloc(sizeof(Edge));
	edge->vertex = vertex;
	edge->weight = weight;
	return edge;
}

void graph_add_vertex(Graph *graph, Vertex *vertex) {
    list_add_data(graph->vertices, vertex);
	graph->count++;
}

void graph_add_vertex_sorted(Graph *graph, Vertex *vertex, int(*cmp)(const void *a, const void *b)) {
    list_add_data_sorted(graph->vertices, vertex, cmp);
	graph->count++;
}

void graph_remove_vertex(Graph *graph, Vertex *vertex) {
	Node *n = graph->vertices->head;
	Node *prev_n = NULL;
	while (n) {
		if (n->data == vertex) {
			if (!prev_n) {
				graph->vertices->head = n->next;
			}
			else {
				prev_n->next = n->next;
			}
            vertex_free(vertex);
			free(n);
		}
		else {
			vertex_remove_edge_to_vertex(n->data, vertex);
		}
		prev_n = n;
		n = n->next;
	}
	graph->count--;
}

void vertex_add_edge(Vertex *vertex, Edge *edge) {
    list_add_data(vertex->edges, edge);
}

void vertex_add_edge_sorted(Vertex *vertex, Edge *edge) {
    list_add_data_sorted(vertex->edges, edge, compare_edges);
}

void vertex_remove_edge(Vertex *vertex, Edge *edge) {
    list_remove_data(vertex->edges, edge);
}

void vertex_add_edge_to_vertex(Vertex *from, Vertex *to, double weight) {
	Edge *edge = edge_create(to, weight);
    list_add_data(from->edges, edge);
}

void vertex_add_edge_to_vertex_sorted(Vertex *from, Vertex *to, double weight) {
	Edge *edge = edge_create(to, weight);
    list_add_data_sorted(from->edges, edge, compare_edges);
}

void vertex_remove_edge_to_vertex(Vertex *from, Vertex *to) {
	Node *e = from->edges->head;
	Node *prev_e = NULL;
	while (e) {
		if (((Edge *)e->data)->vertex == to) {
			if (!prev_e) {
				from->edges->head = e->next;
			}
			else {
				prev_e->next = e->next;
			}
            free(e->data);
			free(e);
			break;
		}
		prev_e = e;
		e = e->next;
	}
}

void graph_sort_vertices(Graph *graph, int(*cmp)(const void *a, const void *b)) {
    list_sort(graph->vertices, cmp);
}

void vertex_sort_edges(Vertex *vertex) {
	list_sort(vertex->edges, compare_edges);
}

void graph_free(Graph *graph) {
    list_free(graph->vertices);
	free(graph);
}