/****************************************************************************
 *
 * bfs.c -- Visita in ampiezza di grafi orientati
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
 % LabASD - Visita in ampiezza (BFS)
 % Moreno Marzolla <moreno.marzolla@unibo.it>
 % Ultimo aggiornamento: 2022-03-31

 [![BFS Demo](bfs-demo.png "Demo visita BFS")](bfs-demo.html)

 Realizziamo l'algoritmo di visita in ampiezza (_Breadth-First Search_,
 BFS) di un grafo orientato $G=(V,E)$ con $n$ nodi e $m$
 archi. L'algoritmo da implementare accetta un nodo sorgente $s$, e
 deve visitare esclusivamente i nodi raggiungibili da $s$.

 Al termine della visita, l'algoritmo riempie due array `p[]` e `d[]`,
 entrambi di $n$ elementi, con le seguenti informazioni:

 - `p[v]` è il predecessore del nodo $v$ durante la visita (il
    predecessore del nodo sorgente è $-1$; è possibile usare il simbolo
    `NODE_UNDEF` il cui valore è $-1$ per evitare di fare ricorso a
    "costanti magiche");

 - `d[v]` è la distanza del nodo $v$ dalla sorgente, dove per distanza
   si intende il numero di archi attraversati. Per ogni nodo
   raggiungibile dalla sorgente, l'algoritmo BFS determina il minimo
   numero di archi che è necessario attraversare per raggiungerlo. Se
   un nodo $v$ non è raggiungibile dalla sorgente poniamo `d[v] = -1`

 La visita in ampiezza inizia da un nodo sorgente $s$ e procede "per
 livelli": prima si visitano tutti i nodi adiacenti a $s$, poi tutti i
 nodi adiacenti a quelli adiacenti, e così via.  Cliccando
 sull'immagine in cima alla pagina è possibile vedere l'algoritmo BFS
 in azione ([fonte](https://bl.ocks.org/mbostock/11337835)).

 L'algoritmo BFS richiede l'uso di una coda FIFO in cui inserire i nodi
 da visitare. In una coda FIFO il primo elemento inserito è anche il
 primo ad essere estratto. A tale scopo si può usare la struttura dati
 `List` già vista in una [precedente esercitazione](list.html), che
 supporta inserimento e cancellazione da entrambi gli estremi in tempo
 $O(1)$.

 Alternativamente, viene fornita una implementazione di una coda FIFO
 nei file [queue.h](../solutions/queue.h) e
 [queue.c](../solutions/queue.c). La struttura `Queue` è basata su un
 array circolare che viene automaticamente espanso e contratto in base
 al numero di elementi effettivamente presenti. Tale struttura supporta
 inserimento e cancellazione in tempo $O(1)$ _ammortizzato_[^1]; la
 struttura `List` supporta l'inserimento e la rimozione dalle estremità
 in tempo $O(1)$ nel caso pessimo.

 [^1]: "Tempo $O(1)$ ammortizzato" significa che il costo _medio_
 misurato su una qualunque sequenza di operazioni è costante, anche se
 il costo di una singola operazione potrebbe essere maggiore.  In altre
 parole, la struttura `Queue` garantisce che le operazioni costose
 (cioè il ridimensionamento dell'array) siano "sufficientemente
 infrequenti" da non incidere sul costo medio di una qualunque sequenza
 di operazioni. La dimostrazione esula da questo esercizio, ma è
 presente nel libro di testo per chi fosse interessato.

 Se si vuole usare la struttura `Queue`, le funzioni utili per questo
 esercizio sono:

 `Queue *queue_create( void )`

 : Crea una nuova coda vuota

 `queue_destroy(Queue *q)`

 : Libera tutta la memoria occupata dalla coda; dopo questa chiamata il
 puntatore `q` non deve essere dereferenziato, perché punta ad un'area
 di memoria non più riservata.

 `queue_enqueue(Queue *q, int val)`

 : Inserisce `val` in fondo alla coda

 `int queue_dequeue(Queue *q)`

 : Rimuove e restituisce l'elemento in testa alla coda.

 `int queue_is_empty(const Queue *q)`

 : Restituisce true (nonzero) se la coda è vuota

 Per compilare:

         gcc -std=c90 -Wall -Wpedantic queue.c graph.c bfs.c -o bfs

 oppure, se si usa la struttura `List`:

         gcc -std=c90 -Wall -Wpedantic list.c graph.c bfs.c -o bfs

 Per eseguire occorre passare sulla riga di comando l'indice del nodo
 sorgente, e il nome del file da cui leggere il grafo. Ad esempio:

         ./bfs 0 graph100.in

 dove al posto di 0 si può indicare l'indice di qualunque altro nodo da
 usare come sorgente.

 ## Curiosità

 La visita in ampiezza può essere applicata all'analisi del grafo delle
 mosse di un gioco da tavolo, per determinare il numero minimo di mosse
 necessarie a vincere, o se esiste una strategia vincente.  Un
 risultato interessante è stato ottenuto nel 2007 quando un gruppo di
 ricercatori ha [completato l'analisi del gioco della
 dama](https://www.researchgate.net/publication/231216842_Checkers_Is_Solved),
 che ha circa $5 \times 10^{20}$ configurazioni valide (che
 corrispondono a nodi del grafo). È stato così dimostrato che, se
 entrambi i giocatori giocano in modo "perfetto", il risultato è un
 pareggio.  _"The Atlantic"_ ha un [articolo non
 tecnico](https://www.theatlantic.com/technology/archive/2017/07/marion-tinsley-checkers/534111/)
 che discute questo risultato.

 Usando la visita in ampiezza dell'albero delle mosse, è possibile
 rispondere alle questioni sull'esercizio [shooting
 stars](shooting-stars.html) che sono rimaste in sospeso: qual è il
 minimo numero di mosse necessarie per vincere? qual è il minimo numero
 di mosse necessarie per perdere? esistono configurazioni della griglia
 di gioco che non possono essere ottenute da quella iniziale?

 ## File

 - [bfs.c](bfs.c)
 - [graph.c](graph.c)
 - [graph.h](graph.h)
 - [queue.c](../solutions/queue.c)
 - [queue.h](../solutions/queue.h)
 - [list.c](../solutions/list.c) (se si preferisce implementare la coda usando la struttura `List`)
 - [list.h](../solutions/list.h) (se si preferisce implementare la coda usando la struttura `List`)
 - [graph10.in](graph10.in) (risultato della visita in ampiezza usando [il nodo 0 come sorgente](graph10-bfs-0.out) oppure [il nodo 5 come sorgente](graph10-bfs-5.out))
 - [graph100.in](graph100.in)
 - [graph1000.in](graph1000.in)

 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "../source/repos/ProgettoAlgoritmi/ProgettoAlgoritmi/graph.h"
#include "../source/repos/ProgettoAlgoritmi/ProgettoAlgoritmi/list.h"
#include "../source/repos/ProgettoAlgoritmi/ProgettoAlgoritmi/queue.h"

 /* Si può usare il simbolo NODE_UNDEF per indicare che il predecessore
    della lista dei padri non esiste. */
const int NODE_UNDEF = -1;

typedef enum { WHITE, GREY, BLACK } Color; /* colori dei nodi */

/* Visita il grafo g usando l'algoritmo di visita in ampiezza (BFS)
   partendo dal nodo sorgente s. Restituisce il numero di nodi
   visitati (incluso s). */
int bfs(const Graph* g,
    int s,  /* Nodo sorgente */
    int* d, /* d[i] e' la distanza del nodo `i` dalla sorgente
               `s`, dove la distanza e' intesa come minimo numero
               di archi che collegano la sorgente al nodo
               `i`. Questo array deve essere allocato dal
               chiamante e contenere `n` elementi (uno per ogni
               nodo del grafo) */
    int* p  /* array di predecessori; `p[i]` è il predecessore
               del nodo `i` lungo il cammino minimo da `s` a
               `i`. Nel caso in cui `i` non sia raggiungibile
               dalla sorgente, `p[i] = -1`. Anche questo array
               deve essere allocato dal chiamante e avere
               lunghezza `n` */
)
{
    /* È possibile fare a meno dell'array `color`.  Per capire se un
       nodo è stato visitato, basta controllare se la sua distanza è
       maggiore o uguale a zero oppure no. I nodi non visitati hanno
       distanza negativa. */
    const int n = graph_n_nodes(g);
    List* l;
    int nvisited = 0;
    int i;
    Color* color = (Color*)malloc(n * sizeof(*color));

    assert(color != NULL);
    assert((s >= 0) && (s < n));

    for (i = 0; i < n; i++) {
        d[i] = -1;
        p[i] = NODE_UNDEF;
        color[i] = WHITE;
    }

    d[s] = 0;
    color[s] = GREY;
    l = list_create();
    list_add_first(l, s);

    while (!list_is_empty(l)) {
        const int u = list_remove_first(l);
        const Edge* edge;
        nvisited++;
        for (edge = graph_adj(g, u); edge != NULL; edge = edge->next) {
            const int v = edge->dst;
            assert(u == edge->src);
            if (color[v] == WHITE) { /* il nodo v non è ancora stato visitato */
                color[v] = GREY;
                d[v] = d[u] + 1;
                p[v] = u;
                list_add_last(l, v);
            }
        }
        color[u] = BLACK;
    }
    list_destroy(l);
    free(color);
    return nvisited;
}

/* Stampa il cammino che va da `s` a `d` prodotto dalla visita in
   ampiezza; se `d` non è raggiungibile da `s`, stampa "Non
   raggiungibile". La stampa del cammino deve avere la forma:

   s->n1->n2->...->d

   dove n1, n2... sono gli interi che individuano i nodi
   attraversati. */
void print_path(int s, int d, const int* p)
{
    if (s == d)
        printf("%d", s);
    else if (p[d] < 0)
        printf("Non raggiungibile");
    else {
        print_path(s, p[d], p);
        printf("->%d", d);
    }
}

/* Stampa le distanze di tutti i nodi dal nodo `src`, e i cammini per
   raggiungere tutti i nodi del grafo da `src`. I cammini sono quelli
   prodotti dalla visita in ampiezza. L'array `p[]` indica l'array dei
   predecessori, cioè `p[i]` è il predecessore del nodo `i`
   nell'albero corrispondente alla visita BFS. */
void print_bfs(const Graph* g, int src, const int* d, const int* p)
{
    const int n = graph_n_nodes(g);
    int v;

    assert(p != NULL);
    assert(d != NULL);

    printf("  src | dest | distanza | path\n");
    printf("------+------+----------+-------------------------\n");
    for (v = 0; v < n; v++) {
        printf(" %4d | %4d | %8d | ", src, v, d[v]);
        print_path(src, v, p);
        printf("\n");
    }
}

int** set_dimensions(FILE* f) 
{
    int n = 0, m = 0, **matrix, retValue;
    char c;

    assert(f != NULL);

    if (2 != fscanf(f, "%d %d", &n, &m)) {
        fprintf(stderr, "ERRORE durante la lettura dell'intestazione del file\n");
        abort();
    };
    assert(n >= 10);
    assert(m <= 1000);
    matrix[n][m];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            retValue = fscanf(f, "%c", &c);
            if (retValue == 1 && c != '\n') {
                matrix[n][m] = atoi(c);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("x: %d, y: %d => %d", n, m, matrix[n][m]);
        }
    }

    return matrix;
}


int main(char* argv[])
{
    Graph* G;
    int** matrix;
    int nvisited; /* n. di nodi raggiungibili dalla sorgente */
    int* p, * d;
    FILE* filein = stdin;
    int src = 0, n;

    if (argv[1] != NULL) {
        fprintf(stderr, "Invocare il programma con: %s file_grafo\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[0], "-") != 0) {
        filein = fopen(argv[0], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[2]);
            return EXIT_FAILURE;
        }
    }
    
    matrix = set_dimensions(filein);
    src = matrix[1][1];

    /*
    G = graph_read_from_file(filein);
    n = graph_n_nodes(G);
    assert((src >= 0) && (src < n));
    p = (int*)malloc(n * sizeof(*p)); assert(p != NULL);
    d = (int*)malloc(n * sizeof(*d)); assert(d != NULL);
    nvisited = bfs(G, src, d, p);
    print_bfs(G, src, d, p);
    printf("# %d nodi su %d raggiungibili dalla sorgente %d\n", nvisited, n, src);
    graph_destroy(G);
    free(p);
    free(d);
    if (filein != stdin) fclose(filein);
    */

    return EXIT_SUCCESS;
}
