#ifndef FUNCTIONALITY_H
#define FUNCTIONALITY_H

#define UNKNOWN -1
#define EMPTY 0
#define WALL 1
#define OBSTACLE 2
#define MARKER 3
#define HOME 4

#define TRUE 1
#define FALSE 0


typedef enum { 
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
} Orientation;


typedef struct {
    int x;
    int y;
} Point;


typedef struct {
    char **grid;
    int size;
    int width, height;
    int numberOfMarkers;
} Maze;


typedef struct {
    Point *position;
    Point *home;
    Orientation orientation;
    int numberOfCurrentlyHeldMarkers;
    char **knownGrid;
} Robot;


Maze* generateMaze(void);
Robot* generateRobot(Maze *maze);

void generateObjectPosition(Maze *maze, Point *objectPosition, int typeOfObject);

void moveRobotToExploreMap(Robot *robot, Maze *maze);

void freeMazeAndRobotGrids(Maze *maze, Robot *robot);
void freeMazeMemory(Maze *maze);
void freeRobotMemory(Robot *robot);

#endif
