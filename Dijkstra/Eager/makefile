# include directory for Indexed Priority Queue data stucture
INC=-I/../../Data_Structures

dijkstra: DijkstraMain.o DijkstrasShortestPathDHeap.o
	g++ DijkstraMain.o DijkstrasShortestPathDHeap.o -o dijkstra

DijkstraMain.o: DijkstraMain.cpp
	g++ -c DijkstraMain.cpp

DijkstrasShortestPathDHeap.o: DijkstrasShortestPathDHeap.cpp
	g++ -c DijkstrasShortestPathDHeap.cpp

clean:
	rm *.o dijkstra