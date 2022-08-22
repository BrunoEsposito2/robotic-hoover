/****************************************************************************
 *
 * graph.c -- Implementazione grafo mediante liste di adiacenza
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

 /***
 % LabASD - Implementazione del tipo "Grafo"
 % Moreno Marzolla <moreno.marzolla@unibo.it>
 % Ultimo aggiornamento: 2022-04-08

 ![Il problema dei [sette ponti di Königsberg](https://en.wikipedia.org/wiki/Seven_Bridges_of_K%C3%B6nigsberg) risolto da [Leonhard Euler](https://en.wikipedia.org/wiki/Leonhard_Euler) nel 1736 ha segnato la nascita della teoria dei grafi](konigsberg.jpg "Il problema dei sette ponti di Königsberg")

 Questo file contiene alcune funzioni per gestire la rappresentazione
 di grafi mediante liste di adiacenza. È possibile rappresentare sia
 grafi orientati che non orientati, usando le strutture dati seguenti:

 ```C
 typedef struct Edge {
     int src;
     int dst;
     double weight;
     struct Edge *next;
 } Edge;

 typedef enum { GRAPH_UNDIRECTED, GRAPH_DIRECTED } Graph_type;

 typedef struct {
     int n;
     int m;
     Graph_type t;
     Edge **edges;
     int *in_deg;
     int *out_deg;
 } Graph;
 ```

 `Edge` rappresenta un arco del grafo. Per grafi non orientati, ogni
 arco $(u,v)$ deve essere presente due volte: una come $(u,v)$ nella
 lista di adiacenza di $u$, e una come $(v,u)$ nella lista di adiacenza
 di $v$. Le liste sono concatenate semplici; il campo `next` indica
 l'arco successivo della lista di adiacenza, oppure `NULL` se è
 l'ultimo nodo.

 La struttura `Graph` rappresenta l'intero grafo; la spiegazione dei
 vari campi è nel file [graph.h](graph.h).  Il campo `edges` è un array
 di lunghezza $n$ di puntatori: `edges[v]` punta all'inizio della lista
 di adiacenza del nodo $v$, oppure `NULL` se $v$ non ha archi
 uscenti. `in_deg` e `out_deg` sono array di lunghezza $n$; `in_deg[v]`
 e `out_deg[v]` sono, rispettivamente, il grado entrante e il grado
 uscente di $v$. Nel caso di grafi non orientati, si deve avere
 `out_deg[v] == in_deg[v]` per ogni $v$, dato che ogni arco viene
 considerato sia entrante che uscente su entrambi gli estremi. È
 necessario mantenere l'informazione sui gradi entranti/uscenti durante
 la costruzione del grafo.

 La Figura 1 mostra un esempio di rappresentazione di un grafo
 orientato, mentre la Figura 2 mostra un esempio di grafo non
 orientato; si noti che l'ordine con cui gli archi compaiono nelle
 liste di adiacenza non è rilevante, e dipende dall'ordine con cui sono
 stati inseriti nel grafo.

 ![Figura 1: Rappresentazione di un grafo orientato](graph-directed.png)

 ![Figura 2: Rappresentazione di un grafo non orientato](graph-undirected.png)

 L'elenco delle funzioni con la descrizione dei parametri e del
 comportamento atteso è presente nel file [graph.h](graph.h).

 Il file [graph-main.c](graph-main.c) contiene una funzione `main()`
 che legge un grafo da file, il cui nome va specificato sulla riga di
 comando, e ne stampa il contenuto a video. Per rispondere ai quiz di
 autovalutazione sulla piattaforma "Virtuale", è possibile modificare
 il programma per stampare il grado entrante/uscente dei nodi con le
 funzioni `graph_in_degree()` oppure `graph_out_degree()`.

 Per compilare:

         gcc -std=c99 -Wall -Wpedantic graph.c graph-main.c -o graph-main

 Per eseguire:

         ./graph-main graph10.in

 ## Formato di input/output

 Le funzioni `graph_read_from_file()` e `graph_write_to_file()` utilizzano
 un semplice formato testuale per rappresentare un grafo:

 ```
 n m type
 s[0]    d[0]    w[0]
 s[1]    d[1]    w[1]
 ...
 s[m-1]  d[m-1]  w[m-1]
 ```

 dove:

 - _n_ è il numero di nodi; i nodi sono rappresentati dagli interi $0,
   \ldots, (n-1)$;

 - _m_ è il numero di archi;

 - _type_ può valere 0 (grafo non orientato), oppure 1 (grafo orientato)

 - $s[i], d[i], w[i]$ indicano, rispettivamente, il nodo sorgente, il nodo
   destinazione e il peso dell'arco $i$-esimo. I pesi sono valori
   reali, anche negativi.

 ## File

 - [graph.c](graph.c)
 - [graph.h](graph.h)
 - [graph-main.c](graph-main.c)
 - [graph10.in](graph10.in) ([output atteso](graph10.out); l'ordine con cui compaiono gli archi nelle liste di adiacenza è irrilevante)
 - [graph100.in](graph100.in)
 - [graph1000.in](graph1000.in)

 ***/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

Graph* graph_create(int n, Graph_type t)
{
    int i;
    Graph* g = (Graph*)malloc(sizeof(*g));
    assert(g != NULL);
    assert(n > 0);

    g->n = n;
    g->m = 0;
    g->t = t;
    g->edges = (Edge**)malloc(n * sizeof(Edge*));
    assert(g->edges != NULL);
    g->in_deg = (int*)malloc(n * sizeof(*(g->in_deg)));
    assert(g->in_deg != NULL);
    g->out_deg = (int*)malloc(n * sizeof(*(g->out_deg)));
    assert(g->out_deg != NULL);
    for (i = 0; i < n; i++) {
        g->edges[i] = NULL;
        g->in_deg[i] = g->out_deg[i] = 0;
    }
    return g;
}

void graph_destroy(Graph* g)
{
    int i;

    assert(g != NULL);

    for (i = 0; i < g->n; i++) {
        Edge* edge = g->edges[i];
        while (edge != NULL) {
            Edge* next = edge->next;
            free(edge);
            edge = next;
        }
        g->edges[i] = NULL; /* e' superfluo */
    }
    free(g->edges);
    free(g->in_deg);
    free(g->out_deg);
    g->n = 0;
    g->edges = NULL;
    free(g);
}

Graph_type graph_type(const Graph* g)
{
    return g->t;
}

static Edge* new_edge(int src, int dst, int srcX, int srcY, int dstX, int dstY, double weight, Edge* next)
{
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    assert(edge != NULL);

    edge->s = src;
    edge->d = dst;
    edge->src[0] = srcX;
    edge->src[1] = srcY;
    edge->dst[0] = dstX;
    edge->dst[1] = dstY;
    edge->weight = weight;
    edge->next = next;
    return edge;
}

/* Inserisce l'arco (src, dst, weight) nel grafo; NON viene inserito
   l'arco opposto nel caso di grafo non orientato (occorre invocare
   due volte questa funzione). Questa funzione ritorna sempre 0; se si
   effettua il controllo sulla presenza di archi duplicati (controllo
   che aumenta il costo asintotico di questa operazione), la funzione
   restituisce true (nonzero) se e solo se l'arco (src, dst) esiste
   già e quindi non viene inserito. */
static int graph_adj_insert(Graph* g, int src, int dst, int srcX, int srcY, int dstX, int dstY, double weight)
{
#if 0
    const Edge* e;

    /* Se si vuole rendere l'operazione più robusta, controlliamo che
       l'arco non esista già. */
    e = graph_adj(g, src);
    while ((e != NULL) && (e->dst != dst)) {
        e = e->next;
    }
    /* Arrivati qui abbiamo che e == NULL se e solo se l'arco
       non è stato trovato. */
    if (e != NULL) {
        return 1;
    }
#endif
    /* Inseriamo l'arco all'inizio della lista di adiacenza.  Se non
       ci fosse il controllo precedente, l'inserimento di un arco
       richiederebbe tempo O(1) */
    g->edges[src] = new_edge(src, dst, srcX, srcY, dstX, dstY, weight, g->edges[src]);
    g->in_deg[dst]++;
    g->out_deg[src]++;
    return 0;
}

void graph_add_edge(Graph* g, int src, int dst, int srcX, int srcY, int dstX, int dstY, double weight)
{
    int status = 0;

    assert(g != NULL);

    assert((srcX >= 0) && (srcX < graph_n_nodes(g)));
    assert((srcY >= 0) && (srcY < graph_n_nodes(g)));

    assert((dstX >= 0) && (dstX < graph_n_nodes(g)));
    assert((dstY >= 0) && (dstY < graph_n_nodes(g)));

    status = graph_adj_insert(g, src, dst, srcX, srcY, dstX, dstY, weight);
    if (graph_type(g) == GRAPH_UNDIRECTED) {
        status |= graph_adj_insert(g, src, dst, dstX, dstY, srcX, srcY, weight);
    }
    if (status == 0)
        g->m++;
    else
        fprintf(stderr, "Ho ignorato l'arco duplicato ((%d, %d), (%d, %d))\n", srcX, srcY, dstX, dstY);
}

int graph_n_nodes(const Graph* g)
{
    assert(g != NULL);

    return g->n;
}

int graph_n_edges(const Graph* g)
{
    assert(g != NULL);

    return g->m;
}

int graph_out_degree(const Graph* g, int v)
{
    assert(g != NULL);
    assert((v >= 0) && (v < graph_n_nodes(g)));
    return g->out_deg[v];
}

int graph_in_degree(const Graph* g, int v)
{
    assert(g != NULL);
    assert((v >= 0) && (v < graph_n_nodes(g)));
    return g->in_deg[v];
}

Edge* graph_adj(const Graph* g, int v)
{
    assert(g != NULL);
    assert((v >= 0) && (v < graph_n_nodes(g)));

    return g->edges[v];
}

void graph_print(const Graph* g)
{
    int i;

    assert(g != NULL);

    if (graph_type(g) == GRAPH_UNDIRECTED) {
        printf("UNDIRECTED\n");
    }
    else {
        printf("DIRECTED\n");
    }

    for (i = 0; i < g->n; i++) {
        const Edge* e;
        int out_deg = 0; /* ne approfittiamo per controllare la
                            correttezza dei gradi uscenti */
        printf("[%2d] -> ", i);
        for (e = graph_adj(g, i); e != NULL; e = e->next) {
            printf("src(%d, %d) dst(%d, %d) %f) -> ", e->src[0], e->src[1], e->dst[0], e->dst[1], e->weight);
            out_deg++;
        }
        /* assert(out_deg == graph_out_degree(g, i)); */
        printf("NULL\n");
    }
}

double setWeight(int** matrix, int indX, int indY) {
    int i, j;
    for (i = indX - 1; i <= indX + 1; i++) {
        for (j = indY - 1; j <= indY + 1; j++) {
            if (matrix[i][j] == 42) {
                return -1; /* ritorno un valore non ammissibile di peso */ 
            }
        }
    }
    return 1; /* ritorno un valore minimo di peso */ 
}

Graph* graph_read_from_map(char* f, int** matrix, const int direction)
{
    int n, m, nNodes;
    int i, j, k; /* numero archi letti dal file */
    double weight;
    Graph* g;
    FILE* file = stdin;
    file = fopen(f, "r");

    assert(file != NULL);

    if (2 != fscanf(file, "%d %d ", &n, &m)) {
        fprintf(stderr, "ERRORE durante la lettura dell'intestazione del grafo\n");
        abort();
    }

    assert(n > 0);
    assert(m >= 0);

    nNodes = (n - 2) * (m - 2); /* calcolo il numero totale dei nodi del grado 
                            considerando ognuno di dimensione 3x3 */

    assert(nNodes > 0);
    assert((direction == GRAPH_UNDIRECTED) || (direction == GRAPH_DIRECTED));

    g = graph_create(nNodes, direction);
    /* Ciclo di lettura degli archi. Per rendere il programma più
       robusto, meglio non fidarsi del valore `m` nell'intestazione
       dell'input. Leggiamo informazioni sugli archi fino a quando ne
       troviamo, e poi controlliamo che il numero di archi letti (i)
       sia uguale a quello dichiarato (m) */

    i = 1;
    j = 1;
    k = 0;
    int s;
    while (i < n - 1 && j < m) {
        if (j == m - 1) {
            j = 1;
            i++;
        }
        s = k;
        if (i + 2 <= n - 1) { /* guardo a SUD */
            weight = setWeight(matrix, i + 1, j);
            graph_add_edge(g, k, k + 1, i, j, i + 1, j, weight);
        }
        if (i - 2 >= 0) { /* guardo a OVEST */
            weight = setWeight(matrix, i - 1, j);
            graph_add_edge(g, k, k + 2, i, j, i - 1, j, weight);
        }
        if (j + 2 <= m - 1) { /* guardo a EST */
            weight = setWeight(matrix, i, j + 1);
            graph_add_edge(g, k, k + 3, i, j, i, j + 1, weight);
        }
        if (j - 2 >= 0) { /* guardo a NORD */
            weight = setWeight(matrix, i, j - 1);
            graph_add_edge(g, k, k + 4, i, j, i, j - 1, weight);
        }
        k = s + 1;
        printf("K: %d \n", k);
        j++;
    }

    /*if (i != nNodes - 1) {
        fprintf(stderr, "WARNING: ho letto %d archi, ma l'intestazione ne dichiara %d\n", i, m);
    }*/

    /*
    fprintf(stderr, "INFO: Letto grafo %s con %d nodi e %d archi\n",
            (t == GRAPH_UNDIRECTED) ? "non orientato" : "orientato",
            n,
            m);
    */
    return g;
}


Graph* graph_read_from_file(FILE* f)
{
    int n, m, t;
    int src, dst;
    int i; /* numero archi letti dal file */
    double weight;
    Graph* g;

    assert(f != NULL);

    if (3 != fscanf(f, "%d %d %d", &n, &m, &t)) {
        fprintf(stderr, "ERRORE durante la lettura dell'intestazione del grafo\n");
        abort();
    };
    assert(n > 0);
    assert(m >= 0);
    assert((t == GRAPH_UNDIRECTED) || (t == GRAPH_DIRECTED));

    g = graph_create(n, t);
    /* Ciclo di lettura degli archi. Per rendere il programma più
       robusto, meglio non fidarsi del valore `m` nell'intestazione
       dell'input. Leggiamo informazioni sugli archi fino a quando ne
       troviamo, e poi controlliamo che il numero di archi letti (i)
       sia uguale a quello dichiarato (m) */
    i = 0;
    while (3 == fscanf(f, "%d %d %lf", &src, &dst, &weight)) {
        /* graph_add_edge(g, src, dst, weight); */
        i++;
    }
    if (i != m) {
        fprintf(stderr, "WARNING: ho letto %d archi, ma l'intestazione ne dichiara %d\n", i, m);
    }
    /*
    fprintf(stderr, "INFO: Letto grafo %s con %d nodi e %d archi\n",
            (t == GRAPH_UNDIRECTED) ? "non orientato" : "orientato",
            n,
            m);
    */
    return g;
}

void graph_write_to_file(FILE* f, const Graph* g)
{
    int v;
    int n, m, t;

    assert(g != NULL);
    assert(f != NULL);

    n = graph_n_nodes(g);
    m = graph_n_edges(g);
    t = graph_type(g);

    fprintf(f, "%d %d %d\n", n, m, t);
    for (v = 0; v < n; v++) {
        const Edge* e;
        for (e = graph_adj(g, v); e != NULL; e = e->next) {
            assert(e->s == v);
            /* Se il grafo è non orientato, dobbiamo ricordarci che
               gli archi compaiono due volte nelle liste di
               adiacenza. Nel file pero' dobbiamo riportare ogni arco
               una sola volta, dato che sarà la procedura di lettura a
               creare le liste di adiacenza in modo corretto. Quindi,
               ogni coppia di archi (u,v), (v,u) deve comparire una
               sola volta nel file. Per comodità, salviamo nel file la
               versione di ciascun arco in cui il nodo sorgente è
               minore del nodo destinazione. */
            if ((graph_type(g) == GRAPH_DIRECTED) || (e->src < e->dst)) {
                fprintf(f, "src(%d, %d) dst(%d, %d) %f\n", e->src[0], e->src[1], e->dst[0], e->dst[1], e->weight);
            }
        }
    }
}
