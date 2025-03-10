#include "functionality.h"
#include "graphics.h"
#include "display.h"

#define OUTLINE 1
#define WITHOUT_OUTLINE 0

const int SLOT_SIZE = 50;
const int WAIT_TIME = 3; 


void drawGridSlot(int x, int y, int slotContent);
void drawWallSlot(int x, int y);
void drawObstacleSlot(int x, int y);
void drawMarkerSlot(int x, int y);
void drawSlot(int x, int y, colour slotColour, int drawOutline);
void drawSlotOutline(int x, int y);
void drawMarkerInSlot(int x, int y);
void drawHomeIcon(int x, int y);

void drawUpMovingAnimation(Point startingSlot, Point endingSlot);
void drawDownMovingAnimation(Point startingSlot, Point endingSlot);
void drawLeftMovingAnimation(Point startingSlot, Point endingSlot);
void drawRightMovingAnimation(Point startingSlot, Point endingSlot);
void drawRobotFacingUp(int x, int y);
void drawRobotFacingDown(int x, int y);
void drawRobotFacingLeft(int x, int y);
void drawRobotFacingRight(int x, int y);
void drawRobot(int xRobotCoordinates[], int yRobotCoordinates[]);

int calculateCoordinateInPixels(int coordinate);


void setWindowSizeForMaze(Maze *maze) {
    setWindowSize(maze->width * SLOT_SIZE, maze->height * SLOT_SIZE);
}


void drawMazeGrid(Maze *maze) {
    for(int row = 0; row < maze->height; ++row) {
        for(int column = 0; column < maze->width; ++column) {
            drawGridSlot(column, row, maze->grid[row][column]);
        }
    }
}


void drawGridSlot(int x, int y, int slotContent) {
    switch(slotContent) {
        case EMPTY:
            drawEmptySlot(x, y);
            break;
        case WALL:
            drawWallSlot(x, y);
            break;
        case OBSTACLE:
            drawObstacleSlot(x, y);
            break;
        case MARKER:
            drawMarkerSlot(x, y);
            break;
    }
}


void drawEmptySlot(int x, int y) {
    drawSlot(x, y, white, OUTLINE);
}


void drawWallSlot(int x, int y) {
    drawSlot(x, y, red, WITHOUT_OUTLINE);
}


void drawObstacleSlot(int x, int y) {
    drawSlot(x, y, black, OUTLINE);
}


void drawMarkerSlot(int x, int y) {
    drawSlot(x, y, white, OUTLINE);
    drawMarkerInSlot(x, y);
}


void drawSlot(int x, int y, colour slotColour, int drawOutline) {
    x = calculateCoordinateInPixels(x);
    y = calculateCoordinateInPixels(y);
    background();
    setColour(slotColour);
    fillRect(x, y, SLOT_SIZE, SLOT_SIZE);
    if(drawOutline == OUTLINE)
        drawSlotOutline(x, y);
}


void drawSlotOutline(int x, int y) {
    setColour(black);
    drawRect(x, y, SLOT_SIZE, SLOT_SIZE);
}


void drawMarkerInSlot(int x, int y) {
    x = calculateCoordinateInPixels(x);
    y = calculateCoordinateInPixels(y);
    setRGBColour(245, 209, 93);
    fillOval(x + 5, y + 5, 40, 40);
    setRGBColour(237, 187, 38);
    fillOval(x + 10, y + 10, 30, 30);
}




void drawHomeSlot(int x, int y) {
    background();
    x = calculateCoordinateInPixels(x);
    y = calculateCoordinateInPixels(y);
    drawHomeIcon(x, y);
}


void drawHomeIcon(int x, int y) {
    setRGBColour(255, 233, 163);
    fillRect(x+10, y+20, 30, 20);
    setRGBColour(125, 47, 47);
    int xRoofCoordinates[] = {x+10, x+40, x+25};
    int yRoofCoordinates[] = {y+20, y+20, y+10};
    fillPolygon(3, xRoofCoordinates, yRoofCoordinates);
}


void drawRobotMovingAnimation(Point startingSlot, Point endingSlot, Orientation robotOrientation) {
    foreground();
    switch(robotOrientation) {
        case UP:
            drawUpMovingAnimation(startingSlot, endingSlot);
            break;
        case DOWN:
            drawDownMovingAnimation(startingSlot, endingSlot);
            break;
        case LEFT:
            drawLeftMovingAnimation(startingSlot, endingSlot);
            break;
        case RIGHT:
            drawRightMovingAnimation(startingSlot, endingSlot);
            break;
    }
}


void drawUpMovingAnimation(Point startingSlot, Point endingSlot) {
    int xConstantPosition = calculateCoordinateInPixels(startingSlot.x);
    int yStartingPosition = calculateCoordinateInPixels(startingSlot.y);
    int yEndingPosition = calculateCoordinateInPixels(endingSlot.y);
    for(int pixel = yStartingPosition; pixel >= yEndingPosition; --pixel) {
        drawRobotFacingUp(xConstantPosition, pixel);
    }
}


void drawDownMovingAnimation(Point startingSlot, Point endingSlot) {
    int xConstantPosition = calculateCoordinateInPixels(startingSlot.x);
    int yStartingPosition = calculateCoordinateInPixels(startingSlot.y);
    int yEndingPosition = calculateCoordinateInPixels(endingSlot.y);
    for(int pixel = yStartingPosition; pixel <= yEndingPosition; ++pixel) {
        drawRobotFacingDown(xConstantPosition, pixel);
    }
}


void drawLeftMovingAnimation(Point startingSlot, Point endingSlot) {
    int yConstantPosition = calculateCoordinateInPixels(startingSlot.y);
    int xStartingPosition = calculateCoordinateInPixels(startingSlot.x);
    int xEndingPosition = calculateCoordinateInPixels(endingSlot.x);
    for(int pixel = xStartingPosition; pixel >= xEndingPosition; --pixel) {
        drawRobotFacingLeft(pixel, yConstantPosition);
    }
}


void drawRightMovingAnimation(Point startingSlot, Point endingSlot) {
    int yConstantPosition = calculateCoordinateInPixels(startingSlot.y);
    int xStartingPosition = calculateCoordinateInPixels(startingSlot.x);
    int xEndingPosition = calculateCoordinateInPixels(endingSlot.x);
    for(int pixel = xStartingPosition; pixel <= xEndingPosition; ++pixel) {
        drawRobotFacingRight(pixel, yConstantPosition);
    }
}


void drawRobotInGivenSlot(int x, int y, Orientation robotOrientation) {
    x = calculateCoordinateInPixels(x);
    y = calculateCoordinateInPixels(y);
    switch(robotOrientation) {
        case UP:
            drawRobotFacingUp(x, y);
            break;
        case DOWN:
            drawRobotFacingDown(x, y);
            break;
        case LEFT:
            drawRobotFacingLeft(x, y);
            break;
        case RIGHT:
            drawRobotFacingRight(x, y);
            break;
    }
    sleep(WAIT_TIME * 25);
}


void drawRobotFacingUp(int x, int y) {
    int xRobotCoordinates[3] = {x, x + SLOT_SIZE/2, x + SLOT_SIZE};
    int yRobotCoordinates[3] = {y + SLOT_SIZE, y, y + SLOT_SIZE};
    drawRobot(xRobotCoordinates, yRobotCoordinates);
}


void drawRobotFacingDown(int x, int y) {
    int xRobotCoordinates[3] = {x, x + SLOT_SIZE/2, x + SLOT_SIZE};
    int yRobotCoordinates[3] = {y, y + SLOT_SIZE, y};
    drawRobot(xRobotCoordinates, yRobotCoordinates);
}


void drawRobotFacingLeft(int x, int y) {
    int xRobotCoordinates[3] = {x + SLOT_SIZE, x, x + SLOT_SIZE};
    int yRobotCoordinates[3] = {y, y + SLOT_SIZE/2, y + SLOT_SIZE};
    drawRobot(xRobotCoordinates, yRobotCoordinates);
}


void drawRobotFacingRight(int x, int y) {
    int xRobotCoordinates[3] = {x, x + SLOT_SIZE, x};
    int yRobotCoordinates[3] = {y, y + SLOT_SIZE/2, y + SLOT_SIZE};
    drawRobot(xRobotCoordinates, yRobotCoordinates);
}


void drawRobot(int xRobotCoordinates[], int yRobotCoordinates[]) {
    foreground();
    clear();
    setColour(green);
    fillPolygon(3, xRobotCoordinates, yRobotCoordinates);
    sleep(WAIT_TIME);
}


int calculateCoordinateInPixels(int slotNumber) {
    return slotNumber * SLOT_SIZE;
}