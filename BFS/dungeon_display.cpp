#include <iostream>
#include "dungeon_display.h"

using namespace std;

void DungeonDisplay::setDungeonSize(int r, int c) {
    // extra: add check to make sure it fits in terminal window
    d_rows = r;
    d_cols = c;
}