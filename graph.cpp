#include "graph.h"

using namespace graph;

typedef struct edgeNode{
	//peso della cella;
	unsigned int weight;
	//puntatore al nodo di destinazione;
	vertexNode* dest;
	//puntatore alla prossima cella di adiacenza;
	edgeNode* nextEdge;
}* adjList;

const adjList emptyEdge = NULL;

struct graph::vertexNode{
	//nome del nodo;
	Label payload;
	//puntatore alla lista di adiacenza;
	adjList edges;
	//puntatore al nodo successivo;
	vertexNode* nextVertex;
	//
	bool visited;
};
/*******************************************************************************************************/
// Grafo
/*******************************************************************************************************/

/*Helper functions*/
bool memberNode(Label , const Graph&);
void reinit(Graph&);
Graph getVertex(Label , const Graph&);
int* dijkstra(Graph&, Label);
void initializeTreeDFS(Graph&, tree::Tree&);
void initializeTreeBFS(Graph&, tree::Tree&);
int computeLenght(const list::List&, const Graph&, unsigned int);
void findPath_aux(Graph&, Label, Label, int*, list::List&);
void printAdj(const adjList&);

// Restituisce il grafo vuoto
Graph graph::createEmptyGraph(){
  return emptyGraph;
}

// Aggiunge nuovo vertice con etichetta la stringa. Fallisce se gia' presente
bool graph::addVertex(Label l, Graph& g) {
	if(memberNode(l, g))return false;
	Graph tmp = new vertexNode;
	tmp->payload = l;
	tmp->edges = emptyEdge;
	tmp->nextVertex = g;
	tmp->visited = false;
	g = tmp;
	return true;
}

// Aggiunge un arco di peso "w" tra i nodi con etichetta "f" e "t". Fallisce se esiste gia' l'arco
// se i nodi non esistono nel grafo e se si tenta di inserire un arco tra un nodo ed esso stesso
bool graph::addEdge(Label from, Label to, Weight w, Graph& g) {
	if(from==to) return false;
	Graph vertexfrom = getVertex(from, g);
	Graph vertexto = getVertex(to, g);
	if(vertexfrom==emptyGraph || vertexto == emptyGraph) return false;
	//control of vertexfrom's adjacentment list:
	for(adjList tmp = vertexfrom->edges; tmp!=emptyEdge ; tmp = tmp->nextEdge) if(tmp->dest==vertexto) return false;
	//after all these damn controls, add the fucking edge:
	adjList aux = new edgeNode;
	aux->weight = w;
	aux->dest = vertexto;
	aux->nextEdge = vertexfrom->edges;
	vertexfrom->edges = aux;
	//and then the other one:
	aux = new edgeNode;
	aux->weight = w;
	aux->dest = vertexfrom;
	aux->nextEdge = vertexto->edges;
	vertexto->edges = aux;
	return true;
}

// Restituisce true se il grafo e' vuoto, false altrimenti
bool graph::isEmpty(const Graph& g){
	return g==emptyGraph;
}

// Ritorna il numero di vertici del grafo
int graph::numVertices(const Graph& g){
 	if(g==emptyGraph)return 0;
 	return 1+numVertices(g->nextVertex);
}

// Ritorna il numero di archi del grafo
int graph::numEdges(const Graph& g){
	int numEdges = 0;
	for(Graph i = g; i!=emptyGraph; i=i->nextVertex)
		for(adjList j =i->edges; j!=emptyEdge; j=j->nextEdge) numEdges++;
	return numEdges/2;
}

// Calcola e ritorna (nel secondo parametro) il grado del nodo. Fallisce
// se il nodo non esiste
bool graph::nodeDegree(Label l, int& degree, const Graph& g) {
	degree = 0;
	for(Graph tmp=g; tmp!=emptyGraph; tmp=tmp->nextVertex, degree++) if(tmp->payload==l) return true;
	degree = -1;
	return false;
}

// Verifica se i due vertici v1 e v2 sono adiacenti (ovvero se esiste un arco)
bool graph::areAdjacent(Label v1, Label v2, const Graph& g) {
	Graph vertex1 = getVertex(v1, g);
	Graph vertex2 = getVertex(v2, g);
	if(vertex1==emptyGraph || vertex2==emptyGraph) return false;
	for(adjList tmp = vertex1->edges; tmp!= emptyEdge; tmp = tmp->nextEdge) if(tmp->dest==vertex2) return true;
	return false;
}

// Restituisce la lista di adiacenza di un vertice
list::List graph::adjacentList(Label v1, const Graph& g) {
	adjList tmp;
	Graph vertex1 = getVertex(v1, g);
	list::List lst = list::createEmpty();
	for(tmp=vertex1->edges; tmp!=emptyEdge; tmp=tmp->nextEdge) list::addBack(lst, tmp->dest->payload);
	return lst;
}

// Ritorna un cammino tra una citta' ed un altra
// Il cammino da "v1" a "v2" alla fine sara' in "path"
// e la lunghezza sara' in "len".
// Si assume che il chiamante fornisca inizialmente un cammino vuoto.
//
// La funzione rappresenta una variante della visita DFS

void graph::findPath(Label v1, Label v2, list::List &path, int &len, const Graph& g) {
	int index;
	Graph tmp = g;
	nodeDegree(v2, index, g);
	findPath_aux(tmp, v1, v2, p, path);
	int* p = dijkstra(tmp, v1);
	len = p[index];
	delete[] p;
}
/*******************************************************************************************************/
// Stampa il grafo

void printGraph(const graph::Graph& g) {
	if(g==emptyGraph) return;
	cout<<g->payload<<": ";
	printAdj(g->edges);
	cout<<endl;
	return printGraph(g->nextVertex);
}
/***************************************FUNZIONI AUSILIARIE******************************/
bool memberNode(Label l, const Graph& g){
	if(g==emptyGraph)return false;
	if(g->payload==l) return true;
	return memberNode(l, g->nextVertex);
}

void printAdj(const adjList& e){
	if(e==emptyEdge){
		cout<<"\b\b.";
		return;	
	} 
	cout<<e->dest->payload<<"("<<e->weight<<"), ";
	return printAdj(e->nextEdge);
}


void reinit(Graph& g){
	if(g==emptyGraph) return;
	g->visited = false;
	return reinit(g->nextVertex);
}

Graph getVertex(Label l, const Graph& g){
	if(g==emptyGraph) return emptyGraph;
	if(g->payload == l) return g;
	return getVertex(l, g->nextVertex);
}

int* dijkstra(Graph& g, Label from){
	reini(g);
	//creazione dell'array di dimensione del numero di nodi:
	Label temp_Label;
	int dsize = numVertices(g), index, temp_len;
	int* distance = new int[dsize];
	//creazione della lista per le chiamate in ampiezza:
	list::List queue = list::createEmpty();
	//aggiungo l'etichetta di partenza:
	list::addBack(queue, from);
	//inizializzo tutte le distanze al massimo:
	for(int i = 0; i<dsize; i++) distance[i] = INT_MAX;
	//pongo la distanza del nodo di partenza a zero;
	if(nodeDegree(from, index, g)) distance[index] = 0;
	do{
		//ottengo label di cui fare la visita:
		temp_Label = list::get(0, queue);
		//rimuovo il nodo dalla lista:
		list::popFront(queue);
		//chiamo sul nodo:
		Graph aux = getVertex(temp_Label, g);
		//lo marco come visitato:
		aux->visited = true;
		//aggiorno la lunghezza corrente(quella fino al nodo che sto visitando):
		nodeDegree(temp_Label, index, g);
		temp_len = distance[index];
		//per ogni elemento della lista di adiacenza:
		for(adjList tmp=aux->edges;tmp!=emptyEdge; tmp=tmp->nextEdge){
			//se l'elemento non è stato visitato:
			if(!tmp->dest->visited){
				//lo aggiungo alla lista per la prossima chiamata:
				list::addBack(queue, (tmp->dest)->payload);
				//lo marco come visitato:
				tmp->dest->visited = true;
			}
			nodeDegree(tmp->dest->payload, index, g);
			if(distance[index]>temp_len+tmp->weight) distance[index]=temp_len+tmp->weight;
		}
	}while(!list::isEmpty(queue));
	return distance;
}

void findPath_aux(Graph& g, Label from, Label to, int* distance, list::List& path){
	reinit(g);
	//creazione dell'array di dimensione del numero di nodi:
	Label temp_Label;
	int index, index_visited, temp_len;
	//creazione della lista per le chiamate in ampiezza:
	list::List queue = list::createEmpty();
	//aggiungo l'etichetta di partenza:
	list::addBack(queue, to);
	list::addFront(path, to);
	do{
		//ottengo label di cui fare la visita:
		temp_Label = list::get(0, queue);
		//rimuovo il nodo dalla lista:
		list::popFront(queue);
		//chiamo sul nodo:
		Graph aux = getVertex(temp_Label, g);
		//lo marco come visitato:
		aux->visited = true;
		//ottengo il valore della sua distanza dal nodo di partenza:
		nodeDegree(temp_Label, index, g);
		//per ogni elemento della lista di adiacenza:
		for(adjList tmp=aux->edges;tmp!=emptyEdge; tmp=tmp->nextEdge){
			//se l'elemento non è stato visitato:
			if(tmp->dest->payload==from) return;
			if(!tmp->dest->visited){
				//lo marco come visitato:
				tmp->dest->visited = true;
				//ottengo la distanza del nodo vistato dal nodo di partenza:
				nodeDegree(tmp->dest->payload, index_visited, g);
				if(distance[index]-distance[index_visited]==temp_len+tmp->weight){
					//lo aggiungo alla lista per la prossima chiamata:
					list::addBack(queue, (tmp->dest)->payload);
					list::addFront(path, tmp->dest->payload);
					break;
				}
			}
		}
	}while(!list::isEmpty(queue));	
}