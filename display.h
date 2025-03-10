#ifndef DISPLAY_H
#define DISPLAY_H

#include "functionality.h"

void setWindowSizeForMaze(Maze *maze);

void drawMazeGrid(Maze *maze);
void drawEmptySlot(int x, int y);
void drawHomeSlot(int x, int y);

void drawRobotMovingAnimation(Point startingSlot, Point endingSlot, Orientation robotOrientation);
void drawRobotInGivenSlot(int x, int y, Orientation robotOrientation);

#endif