#include "functionality.h"
#include "display.h"
#include <stdlib.h>
#include <time.h>


void generateRandomRobotStartingPosition(Maze *maze, Robot *robot);
void allocateMemoryToRobotPositions(Robot *robot);
void initialiseRobotHomePosition(Robot *robot);
void generateRandomRobotOrientation(Robot *robot);
void initialiseRobotMarkerScore(Robot *robot);
void generateRobotMemoryGrid(Maze *maze, Robot *robot);
void allocateMemoryToRobotMemoryGrid(Maze *maze, Robot *robot);
void initialiseRobotMemoryGrid(Maze *maze, Robot *robot);

void forward(Robot *robot, Maze *maze);
int canMoveForward(Point positionAfterMove, Maze *maze);
int isMovablePosition(int positionContent);
void updateRobotPosition(Robot *robot, Point newPosition);
void updateRobotMemoryWithNewPosition(Robot *robot, Maze *maze, Point positionAfterMove);
void left(Robot *robot);
void right(Robot *robot);
int atMarker(Robot *robot, Maze *maze);
void pickUpMarker(Robot *robot, Maze *maze);
void dropMarker(Robot *robot);
int markerCount(Robot *robot);
int isRobotHome(Robot *robot);

void moveRobotToExploreDirection(Robot *robot, Maze *maze, Orientation direction);
int shouldRobotExploreDirection(Robot *robot, Maze *maze);
int isDirectionAlreadyExplored(Robot *robot, Point nextPosition);
void returnToPreviousPosition(Robot *robot, Maze *maze, Orientation direction);

void turnRobotToFaceDirection(Robot *robot, Orientation direction);
void turnRobotLeftToFaceDirection(Robot *robot, Orientation direction);
void turnRobotRightToFaceDirection(Robot *robot, Orientation direction);

Point getRobotPositionAfterMove(Robot *robot);
int getXCoordinateAfterMove(Robot *robot);
int getYCoordinateAfterMove(Robot *robot);

void freeRobotPositionMemory(Robot *robot);


Robot* generateRobot(Maze *maze) {
    Robot *robot = (Robot *)malloc(sizeof(Robot));
    generateRandomRobotStartingPosition(maze, robot);
    generateRandomRobotOrientation(robot);
    initialiseRobotMarkerScore(robot);
    generateRobotMemoryGrid(maze, robot);
    return robot;
}


void generateRandomRobotStartingPosition(Maze *maze, Robot *robot) {
    allocateMemoryToRobotPositions(robot);
    generateObjectPosition(maze, robot->position, HOME);
    initialiseRobotHomePosition(robot);
}


void allocateMemoryToRobotPositions(Robot *robot) {
    robot->position = (Point *)malloc(sizeof(Point));
    robot->home = (Point *)malloc(sizeof(Point));
}


void initialiseRobotHomePosition(Robot *robot) {
    robot->home->x = robot->position->x;
    robot->home->y = robot->position->y;
}


void generateRandomRobotOrientation(Robot *robot) {
    robot->orientation = (Orientation)(rand() % 4);
}


void initialiseRobotMarkerScore(Robot *robot) {
    robot->numberOfCurrentlyHeldMarkers = 0;
}


void generateRobotMemoryGrid(Maze *maze, Robot *robot) {
    allocateMemoryToRobotMemoryGrid(maze, robot);
    initialiseRobotMemoryGrid(maze, robot);
    updateRobotMemoryWithNewPosition(robot, maze, *(robot->home));
}


void allocateMemoryToRobotMemoryGrid(Maze *maze, Robot *robot) {
    robot->knownGrid = (char **)malloc(maze->height * sizeof(char*));
    for(int row = 0; row < maze->height; ++row)
        robot->knownGrid[row] = (char *)malloc(maze->width * sizeof(char*));
}


void initialiseRobotMemoryGrid(Maze *maze, Robot *robot) {
    for(int row = 0; row < maze->height; ++row) {
        for(int column = 0; column < maze->width; ++column) {
            if(row == 0 || row == maze->height-1 || column == 0 || column == maze->width-1) {
                robot->knownGrid[row][column] = WALL;
            } else {
                robot->knownGrid[row][column] = UNKNOWN;
            }
        }
    }
}


void forward(Robot *robot, Maze *maze) {
    Point positionAfterMove = getRobotPositionAfterMove(robot);
    if(canMoveForward(positionAfterMove, maze)) {
        drawRobotMovingAnimation(*(robot->position), positionAfterMove, robot->orientation);
        updateRobotPosition(robot, positionAfterMove);
        if(atMarker(robot, maze))
            pickUpMarker(robot, maze);
    }
    updateRobotMemoryWithNewPosition(robot, maze, positionAfterMove);
}


int canMoveForward(Point positionAfterMove, Maze *maze) {
    return isMovablePosition(maze->grid[positionAfterMove.y][positionAfterMove.x]);
}


int isMovablePosition(int positionContent) {
    return positionContent != WALL && positionContent != OBSTACLE;
}


void updateRobotPosition(Robot *robot, Point newPosition) {
    robot->position->x = newPosition.x;
    robot->position->y = newPosition.y;
}


void updateRobotMemoryWithNewPosition(Robot *robot, Maze *maze, Point positionAfterMove) {
    robot->knownGrid[positionAfterMove.y][positionAfterMove.x] = maze->grid[positionAfterMove.y][positionAfterMove.x];
}


void left(Robot *robot) {
    robot->orientation = robot->orientation == 3 ? 0 : robot->orientation + 1;
}


void right(Robot *robot) {
    robot->orientation = robot->orientation == 0 ? 3 : robot->orientation - 1;
}


int atMarker(Robot *robot, Maze *maze) {
    return maze->grid[robot->position->y][robot->position->x] == MARKER;
}


void pickUpMarker(Robot *robot, Maze *maze) {
    robot->numberOfCurrentlyHeldMarkers += 1; 
    maze->grid[robot->position->y][robot->position->x] = EMPTY;
    drawEmptySlot(robot->position->x, robot->position->y);
}


void dropMarker(Robot *robot) {
    robot->numberOfCurrentlyHeldMarkers -= 1;
}


int markerCount(Robot *robot) {
    return robot->numberOfCurrentlyHeldMarkers;
}


void moveRobotToExploreMap(Robot *robot, Maze *maze) {
    for(Orientation direction = 0; direction < 4; ++direction)
        moveRobotToExploreDirection(robot, maze, direction);
    while(markerCount(robot))
        dropMarker(robot);
}


void moveRobotToExploreDirection(Robot *robot, Maze *maze, Orientation direction) {
    turnRobotToFaceDirection(robot, direction);
    if(!shouldRobotExploreDirection(robot, maze) || markerCount(robot) == maze->numberOfMarkers)
        return;
    forward(robot, maze);
    for(Orientation nextDirection = 0; nextDirection < 4; ++nextDirection)
        moveRobotToExploreDirection(robot, maze, nextDirection);
    returnToPreviousPosition(robot, maze, direction);
}


int shouldRobotExploreDirection(Robot *robot, Maze *maze) {
    Point nextPosition = getRobotPositionAfterMove(robot);
    return !isDirectionAlreadyExplored(robot, nextPosition) && canMoveForward(nextPosition, maze);
}


int isDirectionAlreadyExplored(Robot *robot, Point nextPosition) {
    return robot->knownGrid[nextPosition.y][nextPosition.x] != UNKNOWN;
}


void returnToPreviousPosition(Robot *robot, Maze *maze, Orientation direction) {
    turnRobotToFaceDirection(robot, (direction + 2) % 4);
    forward(robot, maze);
}


void turnRobotToFaceDirection(Robot *robot, Orientation direction) {
    if(robot->orientation == direction)
        return;
    else if((robot->orientation + 1) % 4 == direction)
        turnRobotLeftToFaceDirection(robot, direction);
    else
        turnRobotRightToFaceDirection(robot, direction);
}


void turnRobotLeftToFaceDirection(Robot *robot, Orientation direction) {
    while(robot->orientation != direction) 
        left(robot);
}


void turnRobotRightToFaceDirection(Robot *robot, Orientation direction) {
    while(robot->orientation != direction)
        right(robot);
}


Point getRobotPositionAfterMove(Robot *robot) {
    Point positionAfterMove = {getXCoordinateAfterMove(robot), getYCoordinateAfterMove(robot)};
    return positionAfterMove;
}


int getXCoordinateAfterMove(Robot *robot) {
    int xMoveForDirection[4] = {0, -1, 0, 1};
    return robot->position->x + xMoveForDirection[robot->orientation];
}


int getYCoordinateAfterMove(Robot *robot) {
    int yMoveForDirection[4] = {-1, 0, 1, 0};
    return robot->position->y + yMoveForDirection[robot->orientation];
}


void freeRobotMemory(Robot *robot) {
    freeRobotPositionMemory(robot);
    free(robot);
}


void freeRobotPositionMemory(Robot *robot) {
    free(robot->position);
    free(robot->home);
}