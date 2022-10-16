dungeon: dungeon.o dungeon_display.o
	g++ dungeon.o dungeon_display.o 2d_BFS.o -o dungeon

dungeon.o: dungeon.cpp
	g++ -c dungeon.cpp

dungeon_display.o: dungeon_display.cpp
	g++ -c dungeon_display.cpp

2d_BFS.o: 2d_BFS.cpp
	g++ -c 2d_BFS.cpp

clean:
	rm *.o dungeon