G=graph
M=main
T=tree
D=DLL
$(G): $(G).* $(M).cpp $(D)* $(T)*
	g++ -o $(G) -std=c++11 -ansi -Wall $(G).cpp $(M).cpp $(D).cpp $(T).cpp 2> compilation_result.txt
