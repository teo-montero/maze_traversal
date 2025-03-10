#include "functionality.h"
#include "display.h"
#include <stdlib.h>
#include <time.h>


void initializeRandomSeed();
void generateMazeDrawing(Maze *maze, Robot *robot);
void freeProgramMemory(Maze *maze, Robot *robot);


int main() {
    initializeRandomSeed();
    Maze *maze = generateMaze();
    Robot *robot = generateRobot(maze);
    generateMazeDrawing(maze, robot);
    moveRobotToExploreMap(robot, maze);
    freeProgramMemory(maze, robot);
    return 0;
}


void initializeRandomSeed() {
    srand(time(0));
}


void generateMazeDrawing(Maze *maze, Robot *robot) {
    setWindowSizeForMaze(maze);
    drawMazeGrid(maze);
    drawHomeSlot(robot->home->x, robot->home->y);
}


void freeProgramMemory(Maze *maze, Robot *robot) {
    freeMazeAndRobotGrids(maze, robot);
    freeMazeMemory(maze);
    freeRobotMemory(robot);
}