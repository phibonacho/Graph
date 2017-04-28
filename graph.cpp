#include "graph.h"

using namespace graph;

struct adjCell{
	Label payload;
	adjCell* next;
};

typedef adjCell* adjList;

struct graph::vertexNode{
	Label payload;
	adjList edges;
	Graph* next;
};
/*******************************************************************************************************/
// Grafo
/*******************************************************************************************************/

// Restituisce il grafo vuoto
Graph graph::createEmptyGraph()
{
  return emptyGraph;
}

// Aggiunge nuovo vertice con etichetta la stringa. Fallisce se gia' presente
bool graph::addVertex(Label l, Graph& g) {
  return true;
}

// Aggiunge un arco di peso "w" tra i nodi con etichetta "f" e "t". Fallisce se esiste gia' l'arco
// se i nodi non esistono nel grafo e se si tenta di inserire un arco tra un nodo ed esso stesso
bool graph::addEdge(Label from, Label to, Weight w, Graph& g) {
  return true;
}

// Restituisce true se il grafo e' vuoto, false altrimenti
bool graph::isEmpty(const Graph& g)
{ 
 return true;
}

// Ritorna il numero di vertici del grafo
int graph::numVertices(const Graph& g){
 return 0;
}

// Ritorna il numero di archi del grafo
int graph::numEdges(const Graph& g){
 return 0;
}

// Calcola e ritorna (nel secondo parametro) il grado del nodo. Fallisce
// se il nodo non esiste
bool graph::nodeDegree(Label l, int& degree, const Graph& g) {
  return true;
}

// Verifica se i due vertici v1 e v2 sono adiacenti (ovvero se esiste un arco)
bool graph::areAdjacent(Label v1, Label v2, const Graph& g) {
  return true;
}

// Restituisce la lista di adiacenza di un vertice
list::List graph::adjacentList(Label v1, const Graph& g) {
  list::List lst = list::createEmpty();
  return lst;
}

// Ritorna un cammino tra una citta' ed un altra
// Il cammino da "v1" a "v2" alla fine sara' in "path"
// e la lunghezza sara' in "len".
// Si assume che il chiamante fornisca inizialmente un cammino vuoto.
//
// La funzione rappresenta una variante della visita DFS

void graph::findPath(Label v1, Label v2, list::List &path, int &len, const Graph& g) {
    return;    
}
/*******************************************************************************************************/
// Stampa il grafo
void printGraph(const graph::Graph& g) {
  
}
