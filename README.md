# Graph
Bad implementation of Graph data type.

Forgive my bad english.

Since I have difficulties even in cooking scrambled eggs, 
I thought that upload a school project as first repository(just to try to understand how github works) would be a great idea.

# This repository contains 3 libraries:
-graph.h:
    implement graph data type (as list of vertices with adjacency list);
-DLL.h:
    implement doubly linked list data type;
-tree.h:
    implement tree data type (incredible, right?);

# Graph is the main library: 
the reason why I'm actually sleeping really bad lately. It implements, more or less, all basic functions of graph data type:

bool **addVertex**(Label, Graph&);
    add labeled vertex to graph;

bool **addEdge**(Label, Label, Weight, Graph&);
    add edge between two labeled vertices;

bool **isEmpty**(const Graph&);
    return true if the graph is empty;

int **numVertices**(const Graph&);
    return the number of vertices in graph;

int **numEdges**(const Graph&);
    return the number of edges in graph;

bool **nodeDegree**(Label, int&, const Graph&);
    compute the Labeled vertex's degree and return it in the second parameter, unless it return false;

bool **areAdjacent**(Label, Label, const Graph&); 
    return true if two labeled vertices are adjacent;

void **findPath**(Label, Label, list::List &, int &, const Graph& g);
    return in the second parameter a list of the path, label by label, wich leads from first label to second label,
    and in the third one the lenght of the path(each edge has a weight);

# and some extra as:

list::List adjacentList(Label, const Graph&);
    return all vertices adjacent to the vertex labeled "label";

void ***treealize**(graph::Label, graph::Graph&);
    realize a tree (why am I so stupid?) out of the graph starting from the given vertex;

I'm currently working on findpath() function.
