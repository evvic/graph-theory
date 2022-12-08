# include directory for Indexed Priority Queue data stucture
INC=-I/../../utilities

bellmanford: bfMain.o bellman-ford.o graphviz.o
	g++ bfMain.o bellman-ford.o graphviz.o -o bellmanford

bfMain.o: bfMain.cpp
	g++ -c bfMain.cpp

graphviz.o: ../../utilities/graphviz.cpp
	g++ -c ../../utilities/graphviz.cpp

bellman-ford.o: bellman-ford.cpp
	g++ -c bellman-ford.cpp

clean:
	rm *.o bellmanford