# include directory for Indexed Priority Queue data stucture
INC=-I/../../Utilities

bellmanford: bfMain.o bellman-ford.o
	g++ bfMain.o bellman-ford.o -o bellmanford

bfMain.o: bfMain.cpp
	g++ -c bfMain.cpp

# graphviz.o: ../../Utilities/graphviz.cpp
# 	g++ -c ../../Utilities/graphviz.cpp

bellman-ford.o: bellman-ford.cpp
	g++ -c bellman-ford.cpp

clean:
	rm *.o bellmanford