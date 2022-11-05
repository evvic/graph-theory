# include directory for Indexed Priority Queue data stucture
# INC=-I/../../Data_Structures
INC=-I/../../Utilities

dijkstra: dijkstraEager.o DijkstrasShortestPathDHeap.o graphviz.o
	g++ dijkstraEager.o DijkstrasShortestPathDHeap.o graphviz.o -o dijkstra

dijkstraEager.o: dijkstraEager.cpp
	g++ -c dijkstraEager.cpp

graphviz.o: ../../Utilities/graphviz.cpp
	g++ -c ../../Utilities/graphviz.cpp

DijkstrasShortestPathDHeap.o: DijkstrasShortestPathDHeap.cpp
	g++ -c DijkstrasShortestPathDHeap.cpp



clean:
	rm *.o dijkstra