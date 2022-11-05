# include directory for Indexed Priority Queue data stucture
INC=-I/../../utilities

dijkstra: dijkstraEager.o DijkstrasShortestPathDHeap.o graphviz.o
	g++ dijkstraEager.o DijkstrasShortestPathDHeap.o graphviz.o -o dijkstra

dijkstraEager.o: dijkstraEager.cpp
	g++ -c dijkstraEager.cpp

graphviz.o: ../../utilities/graphviz.cpp
	g++ -c ../../utilities/graphviz.cpp

DijkstrasShortestPathDHeap.o: DijkstrasShortestPathDHeap.cpp
	g++ -c DijkstrasShortestPathDHeap.cpp



clean:
	rm *.o dijkstra