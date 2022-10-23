# include directory for Indexed Priority Queue data stucture
INC=-I/../../Data_Structures

dijkstra: dijkstraEager.o DijkstrasShortestPathDHeap.o
	g++ dijkstraEager.o DijkstrasShortestPathDHeap.o -o dijkstra

dijkstraEager.o: dijkstraEager.cpp
	g++ -c dijkstraEager.cpp

DijkstrasShortestPathDHeap.o: DijkstrasShortestPathDHeap.cpp
	g++ -c DijkstrasShortestPathDHeap.cpp

clean:
	rm *.o dijkstra