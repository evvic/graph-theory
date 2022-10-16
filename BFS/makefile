dungeon: dungeon.o dungeon_display.o
	g++ dungeon.o dungeon_display.o -o dungeon

dungeon.o: dungeon.cpp
	g++ -c dungeon.cpp

dungeon_display.o: dungeon_display.cpp
	g++ -c dungeon_display.cpp

clean:
	rm *.o dungeon