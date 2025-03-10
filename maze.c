#include "functionality.h"
#include <stdlib.h>
#include <time.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))


const int MAX_MAZE_DIMENSION_SIZE = 19;
const int MIN_MAZE_DIMENSION_SIZE = 15;

const double MAX_WALL_SIZE_FACTOR = 0.2;
const int MIN_WALL_SIZE = 1;

const double OBSTACLE_FREQUENCY = 0.2;
const double MARKER_FREQUENCY = 0.075;


void generateRandomMazeDimensions(Maze *maze);
int generateRandomDimension();
void generateMazeGrid(Maze *maze);
void initializeMazeDimensions(Maze *maze);
void createMazeObjects(Maze *maze);
void createMazeWalls(Maze *maze);
void createHorizontalMazeWall(Maze *maze, int row, int column);
void createVerticalMazeWall(Maze *maze, int row, int column);
void closeGapsBetweenWalls(Maze *maze);
void createMazeObstacles(Maze *maze);
void createMazeMarkers(Maze *maze);
void createObjects(Maze *maze, int typeOfObject, int numberOfObjects);
void generateRandomObjectPosition(Maze *maze, Point *objectPosition);
int isPossibleAreaEnclosure(Maze *maze, Point *objectPosition, int typeOfObject);
int calculateObjectFrequencyBasedOnPercentage(Maze *maze, double percentageOfObjects);
int calculateValueBasedOnPercentage(int number, double percentage);
int isObstacleEnclosingWithOtherObstacles(Maze *maze, Point *objectPosition);
int isObstacleNextToWall(Maze *maze, Point *objectPosition);
int generateRandomInRange(int minimum, int maximum);


Maze* generateMaze() {
    Maze *maze = (Maze *)malloc(sizeof(Maze));
    generateRandomMazeDimensions(maze);
    generateMazeGrid(maze);
    return maze;
}


void generateRandomMazeDimensions(Maze *maze) {
    maze->width = generateRandomDimension();
    maze->height = generateRandomDimension();
    maze->size = maze->width * maze->height;
}


int generateRandomDimension() {
    return generateRandomInRange(MIN_MAZE_DIMENSION_SIZE, MAX_MAZE_DIMENSION_SIZE);
}


void generateMazeGrid(Maze *maze) {
    initializeMazeDimensions(maze);
    createMazeObjects(maze);
}


void initializeMazeDimensions(Maze *maze) {
    maze->grid = (char **)malloc(maze->height * sizeof(char*));
    for(int row = 0; row < maze->height; ++row)
        maze->grid[row] = (char *)malloc(maze->width * sizeof(char*));
}


void createMazeObjects(Maze *maze) {
    createMazeWalls(maze);
    createMazeObstacles(maze);
    createMazeMarkers(maze);
}


void createMazeWalls(Maze *maze) {
    for(int row = 0; row < maze->height; ++row) {
        for(int column = 0; column < maze->width; ++column) {
            if(column == 0 || column == maze->width-1)
                createHorizontalMazeWall(maze, row, column);
            else if(row == 0 || row == maze->height-1)
                createVerticalMazeWall(maze, row, column);
        }
    }
    closeGapsBetweenWalls(maze);
}


void createHorizontalMazeWall(Maze *maze, int row, int column) {
    int wallDepth = (int)generateRandomInRange(MIN_WALL_SIZE, MIN(maze->width, maze->height) * MAX_WALL_SIZE_FACTOR);
    for(int currentColumn = (column ? column - wallDepth + 1 : column); currentColumn <= (column ? column : wallDepth); ++currentColumn) {
        if(maze->grid[row][currentColumn] == EMPTY) {
            maze->grid[row][currentColumn] = WALL;
            --maze->size;
        }
    }
}


void createVerticalMazeWall(Maze *maze, int row, int column) {
    int wallDepth = (int)generateRandomInRange(MIN_WALL_SIZE, MIN(maze->width, maze->height) * MAX_WALL_SIZE_FACTOR);
    for(int currentRow = (row ? row - wallDepth + 1 : row); currentRow <= (row ? row : wallDepth); ++currentRow) {
        if(maze->grid[currentRow][column] == EMPTY) {
            maze->grid[currentRow][column] = WALL;
            --maze->size;
        }
    }
}


void closeGapsBetweenWalls(Maze *maze) {
    for(int row = 1; row < maze->height - 1; ++row) {
        for(int column = 1; column < maze->width - 1; ++column) {
            if(maze->grid[row][column] == EMPTY && maze->grid[row-1][column] == WALL && maze->grid[row][column-1] == WALL && maze->grid[row+1][column] == WALL && maze->grid[row][column+1] == WALL)
                maze->grid[row][column] = WALL;
        }
    }
}


void createMazeObstacles(Maze *maze) {
    createObjects(maze, OBSTACLE, calculateObjectFrequencyBasedOnPercentage(maze, OBSTACLE_FREQUENCY));
}


void createMazeMarkers(Maze *maze) {
    maze->numberOfMarkers = calculateObjectFrequencyBasedOnPercentage(maze, MARKER_FREQUENCY);
    createObjects(maze, MARKER, maze->numberOfMarkers);
}


int calculateObjectFrequencyBasedOnPercentage(Maze *maze, double percentageOfObjects) {
    return calculateValueBasedOnPercentage(maze->size, percentageOfObjects);
}


int calculateValueBasedOnPercentage(int number, double percentage) {
    if(percentage < 0 || percentage > 1)
        return -1;
    return number * percentage;
}


void createObjects(Maze *maze, int typeOfObject, int numberOfObjects) {
    for(int i = 0; i < numberOfObjects; ++i) {
        Point objectPosition;
        generateObjectPosition(maze, &objectPosition, typeOfObject);
        maze->grid[objectPosition.y][objectPosition.x] = typeOfObject;
    }
}


void generateObjectPosition(Maze *maze, Point *objectPosition, int typeOfObject) {
    do {
        generateRandomObjectPosition(maze, objectPosition);
    } while((maze->grid[objectPosition->y][objectPosition->x] != EMPTY) || (typeOfObject == OBSTACLE && isPossibleAreaEnclosure(maze, objectPosition, typeOfObject)));
}


void generateRandomObjectPosition(Maze *maze, Point *objectPosition) {
    objectPosition->x = rand() % maze->width;
    objectPosition->y = rand() % maze->height;
}


int isPossibleAreaEnclosure(Maze *maze, Point *objectPosition, int typeOfObject) {
    return isObstacleEnclosingWithOtherObstacles(maze, objectPosition) || isObstacleNextToWall(maze, objectPosition);
}


int isObstacleEnclosingWithOtherObstacles(Maze *maze, Point *objectPosition) {
    if(objectPosition->x > 1 && maze->grid[objectPosition->y][objectPosition->x - 2] == OBSTACLE)
        return TRUE;
    else if(objectPosition->x < maze->width-2 && maze->grid[objectPosition->y][objectPosition->x + 2] == OBSTACLE)
        return TRUE;
    else if(objectPosition->y > 1 && maze->grid[objectPosition->y - 2][objectPosition->x] == OBSTACLE)
        return TRUE;
    else if(objectPosition->y < maze->height-2 && maze->grid[objectPosition->y + 2][objectPosition->x] == OBSTACLE)
        return TRUE;
    return FALSE;
}


int isObstacleNextToWall(Maze *maze, Point *objectPosition) {
    return objectPosition->y > 0 && maze->grid[objectPosition->y-1][objectPosition->x] == WALL || 
           objectPosition->x > 0 && maze->grid[objectPosition->y][objectPosition->x-1] == WALL || 
           objectPosition->y < maze->height-1 && maze->grid[objectPosition->y+1][objectPosition->x] == WALL || 
           objectPosition->x < maze->width-1 && maze->grid[objectPosition->y][objectPosition->x+1] == WALL ||
           objectPosition->x > 0 && objectPosition->y > 0 && maze->grid[objectPosition->y-1][objectPosition->x-1] == WALL ||
           objectPosition->x > 0 && objectPosition->y < maze->height-1 && maze->grid[objectPosition->y+1][objectPosition->x-1] == WALL ||
           objectPosition->x < maze->width-1 && objectPosition->y > 0 && maze->grid[objectPosition->y-1][objectPosition->x+1] == WALL ||
           objectPosition->x < maze->width-1 && objectPosition->y < maze->height-1 && maze->grid[objectPosition->y+1][objectPosition->x+1] == WALL;
}


int generateRandomInRange(int minimum, int maximum) {
    return minimum + (rand() % (maximum - minimum));
}


void freeMazeAndRobotGrids(Maze *maze, Robot *robot) {
    for(int row = 0; row < maze->height; ++row) {
        free(maze->grid[row]);
        free(robot->knownGrid[row]);
    }
    free(maze->grid);
    free(robot->knownGrid);
}


void freeMazeMemory(Maze *maze) {
    free(maze);
}