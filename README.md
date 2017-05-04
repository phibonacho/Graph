# Graph
Bad implementation of Graph data type.

Forgive my bad english.

This repository contains **2 libraries**:
- **graph.h**;
- **DLL.h**:
    (auxiliary library)implement doubly linked list data type;

##### Graph is the main library: 
~~the reason why I'm actually sleeping really, really bad lately.~~
It implements, more or less, graph data type and some basic functions:

- bool **addVertex**(Label, Graph&);
    * add labeled vertex to graph;

- bool **addEdge**(Label, Label, Weight, Graph&);
    * add edge between two labeled vertices;

- bool **isEmpty**(const Graph&);
    * return true if the graph is empty;

- int **numVertices**(const Graph&);
    * return the number of vertices in graph;

- int **numEdges**(const Graph&);
    * return the number of edges in graph;

- bool **nodeDegree**(Label, int&, const Graph&);
    * compute the Labeled vertex's degree and return it in the second parameter, unless it return false;

- bool **areAdjacent**(Label, Label, const Graph&); 
    * return true if two labeled vertices are adjacent;

- void **findPath**(Label, Label, list::List &, int &, const Graph& g);
    * return in the second parameter a list of the path, label by label, wich leads from first label to second label, and in the third one the lenght of the path(each edge has a weight);

##### and some extra functions as:

- list::List adjacentList(Label, const Graph&);
    * return all vertices adjacent to the vertex labeled "label";



### UPDATE:
- modified findPath's auxiliar function: ```int* dijkstra(Graph&, Label)``` to ```Label* dijkstra(Graph&, Label, Label, int&)```:
    * implement [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) and return an array in wich i-th element is the nearest vertex of the i-th vertex in graph. Return in 4th parameter the shortest distance between first and second Label;
- Modified findPath's auxiliary function ```void findPath_aux(Graph&, Label, Label, int*, list::List&)``` to ```void findPath_aux(Graph&, Label, Label, Label*, list::List&)```:
    * returns in 4-th parameter the path, label by label, wich leads from first label to second label;

