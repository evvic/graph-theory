dungeon: dungeon.o 2d_BFS.o dungeon_display.o
	g++ dungeon.o 2d_BFS.o dungeon_display.o -o dungeon

dungeon.o: dungeon.cpp
	g++ -c dungeon.cpp

2d_BFS.o: 2d_BFS.cpp
	g++ -c 2d_BFS.cpp

dungeon_display.o: dungeon_display.cpp
	g++ -c dungeon_display.cpp

clean:
	rm *.o dungeon