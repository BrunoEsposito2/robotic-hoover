/****************************************************************************
 *
 * graph.h -- Interfaccia grafo
 *
 * Copyright (C) 2021, 2022 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

#include "list.h"

/* struttura arco */
typedef struct Edge {
    int s;                  /* nodo sorgente        */
    int d;                  /* nodo destinazione    */
    int src[2];            /* coordinate nodo sorgente        */
    int dst[2];            /* coordinate nodo destinazione    */
    double weight;      /* peso dell'arco       */
    struct Edge *next;
} Edge;

typedef enum { GRAPH_UNDIRECTED, GRAPH_DIRECTED } Graph_type;

/* struttura grafo */
typedef struct {
    int n;              /* numero di nodi               */
    int m;              /* numero di archi              */
    Graph_type t;       /* tipo di grafo (orientato/non orientato) */
    Edge **edges;       /* array di liste di adiacenza  */
    int *in_deg;        /* grado entrante dei nodi      */
    int *out_deg;       /* grado uscente dei nodi       */
} Graph;

/* Crea un nuovo grafo con `n` nodi. Il numero di nodi deve essere
   strettamente positivo. Il tipo di grafo (orientato oppure non
   orientato) è specificato dal parametro `t`. */
Graph *graph_create(int n, Graph_type t);

/* Libera tutta la memoria occupata dal grafo e dalle liste di
   adiacenza */
void graph_destroy(Graph *g);

/* Restituisce il tipo di grafo */
Graph_type graph_type(const Graph *g);

/* Aggiunge un nuovo arco (src, dst) con peso "weight". Si puo'
   assumere che l'arco non esista già (si può omettere il controllo,
   anche se nella soluzione fornita viene fatto). Nel caso di grafo
   non orientato, occorre aggiungere l'arco sia nella lista di
   adiacenza di `src` che in quella di `dst`. */
void graph_add_edge(Graph *g, int src, int dst, int srcX, int srcY, int dstX, int dstY, double weight);

/* Restituisce un puntatore al primo arco della lista di adiacenza
   associata al nodo `v` (`NULL` se la lista è vuota) */
Edge *graph_adj(const Graph *g, int v);

/* Restituisce il numero di nodi del grafo */
int graph_n_nodes(const Graph *g);

/* Restituisce il numero di archi del grafo */
int graph_n_edges(const Graph *g);

/* Restituisce il grado uscente del nodo `v`. */
int graph_out_degree(const Graph *g, int v);

/* Restituisce il grado entrante nel nodo `v`. */
int graph_in_degree(const Graph *g, int v);

/* Stampa il grafo a video */
void graph_print(const Graph *g);

/* Salva il grafo `g` sul file `f`, con lo stesso formato usato dalla
   funzione `graph_read_from_file()`. Il file `f` deve essere già
   stato aperto in scrittura. */
void graph_write_to_file(FILE *f, const Graph *g);

/* crea un grafo a partire dalla matrice ricavata dalla lettura di un file */
Graph* graph_create_from_matrix(char* f, int** matrix, const int direction);

/* stampa il percorso su un file */
void path_write_to_file(FILE* f, Graph* g, const List* path, int src);

#endif
