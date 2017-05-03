#include "graph.h"

using namespace graph;

typedef struct edgeNode{
	unsigned int weight;
	vertexNode* dest;
	edgeNode* nextEdge;
}* adjList;

const adjList emptyEdge = NULL;

struct graph::vertexNode{
	Label payload;
	adjList edges;
	vertexNode* nextVertex;
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
int computeLenght(const list::List&, const Graph&, unsigned int);
void findPath_aux(Graph&, Label, Label, int*, list::List&);
void printAdj(const adjList&);

// Return the empty Graph:
Graph graph::createEmptyGraph(){
  return emptyGraph;
}

// Add new labeled vertex. Fails if vertex already exists:
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

// Add new edge between vertices labeled "from" and "to". Fails if vertices do not exist or edge already exists 
bool graph::addEdge(Label from, Label to, Weight w, Graph& g) {
	if(from==to) return false;
	Graph vertexfrom = getVertex(from, g);
	Graph vertexto = getVertex(to, g);
	if(vertexfrom==emptyGraph || vertexto == emptyGraph) return false;
	//control of vertexfrom's adjacentment list:
	for(adjList tmp = vertexfrom->edges; tmp!=emptyEdge ; tmp = tmp->nextEdge) if(tmp->dest==vertexto) return false;
	//adding edge in vertexfrom's adjList:
	adjList aux = new edgeNode;
	aux->weight = w;
	aux->dest = vertexto;
	aux->nextEdge = vertexfrom->edges;
	vertexfrom->edges = aux;
	//adding edge in vertexto's adjList
	aux = new edgeNode;
	aux->weight = w;
	aux->dest = vertexfrom;
	aux->nextEdge = vertexto->edges;
	vertexto->edges = aux;
	return true;
}

bool graph::isEmpty(const Graph& g){
	return g==emptyGraph;
}

int graph::numVertices(const Graph& g){
 	if(g==emptyGraph)return 0;
 	return 1+numVertices(g->nextVertex);
}

int graph::numEdges(const Graph& g){
	int numEdges = 0;
	for(Graph i = g; i!=emptyGraph; i=i->nextVertex)
		for(adjList j =i->edges; j!=emptyEdge; j=j->nextEdge) numEdges++;
	return numEdges/2;
}

// Compute and return (in second parameter) vertex's degree. Fail if vertex does not exist:
bool graph::nodeDegree(Label l, int& degree, const Graph& g) {
	degree = 0;
	for(Graph tmp=g; tmp!=emptyGraph; tmp=tmp->nextVertex, degree++) if(tmp->payload==l) return true;
	degree = -1;
	return false;
}

// Verify if an edge exists between vertex labeled v1 and vertex labeled v2:
bool graph::areAdjacent(Label v1, Label v2, const Graph& g) {
	Graph vertex1 = getVertex(v1, g);
	Graph vertex2 = getVertex(v2, g);
	if(vertex1==emptyGraph || vertex2==emptyGraph) return false;
	for(adjList tmp = vertex1->edges; tmp!= emptyEdge; tmp = tmp->nextEdge) if(tmp->dest==vertex2) return true;
	return false;
}

// Return v1's adjoining list:
list::List graph::adjacentList(Label v1, const Graph& g) {
	adjList tmp;
	Graph vertex1 = getVertex(v1, g);
	list::List lst = list::createEmpty();
	for(tmp=vertex1->edges; tmp!=emptyEdge; tmp=tmp->nextEdge) list::addBack(lst, tmp->dest->payload);
	return lst;
}

//Return in third parameter tha path (label by label) wich lead from label v1 to label v2 and it lenght in 4-th parameter:
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
// Print Graph:

void printGraph(const graph::Graph& g) {
	if(g==emptyGraph) return;
	cout<<g->payload<<": ";
	printAdj(g->edges);
	cout<<endl;
	return printGraph(g->nextVertex);
}
/******************************************************************************************************/

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
	Label temp_Label;
	int dsize = numVertices(g), index, temp_len;
	reini(g);
	int* distance = new int[dsize];
	list::List queue = list::createEmpty();
	list::addBack(queue, from);
	for(int i = 0; i<dsize; i++) distance[i] = INT_MAX;
	if(nodeDegree(from, index, g)) distance[index] = 0;
	do{
		temp_Label = list::get(0, queue);
		list::popFront(queue);
		Graph aux = getVertex(temp_Label, g);
		aux->visited = true;
		nodeDegree(temp_Label, index, g);
		temp_len = distance[index];
		for(adjList tmp=aux->edges;tmp!=emptyEdge; tmp=tmp->nextEdge){
			if(!tmp->dest->visited){
				list::addBack(queue, (tmp->dest)->payload);
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
	Label temp_Label;
	int index, index_visited, temp_len;
	list::List queue = list::createEmpty();
	list::addBack(queue, to);
	list::addFront(path, to);
	do{
		temp_Label = list::get(0, queue);
		list::popFront(queue);
		Graph aux = getVertex(temp_Label, g);
		aux->visited = true;
		nodeDegree(temp_Label, index, g);
		for(adjList tmp=aux->edges;tmp!=emptyEdge; tmp=tmp->nextEdge){
			if(tmp->dest->payload==from) return;
			if(!tmp->dest->visited){
				tmp->dest->visited = true;
				nodeDegree(tmp->dest->payload, index_visited, g);
				if(distance[index]-distance[index_visited]==temp_len+tmp->weight){
					list::addBack(queue, (tmp->dest)->payload);
					list::addFront(path, tmp->dest->payload);
					break;
				}
			}
		}
	}while(!list::isEmpty(queue));
}
