# Arena Generation and Traversal Program

The program consists of a moving robot which is going to explore a randomly generated maze (random size, random distribution of obstacles and markers) using an implementation of the Depth First Search (DFS) exploration algorithm. 

# Structure and Functionality

The code containing the logic and functionality is written in C, and it uses an Object Oriented approach by using structs to represent the objects (Arena, Robot...). To generate the Graphical User Interface, the C program pipes its output to a Java Application, which reads the commands and generates the GUI using JSwing accordingly.

The source code is split into four files by functionality:

- main.c
  - Contains all code required to initialise the system (maze and robot), trigger the execution of the maze exploration and free the program memory
- maze.c
  - Contains all functionality related to the maze:
    - Generation of the Maze
    - Freeing of memory
- robot.c
  - Contains all functionality related to the robot:
    - Generation of the robot
    - Robot control
    - Robot exploration
    - Freeing of memory
- display.c
  - Contains all functionality related to the displaying of the objects using drawapp-4.0.jar:
    - Drawing the maze
    - Drawing the robot (including moving animations)
    - Maze updating (when markers are collected)

### Traversal Algorithm

The Arena Traversal Algorithm used by the robot to explore the arena is Depth-First Search (DFS), which is a traversal algorithm that follows one path as far as possible before backtracking when reaching a dead end. It uses recursion to traverse the maze, ensuring every possible route is explored systematically until the exit is found or all paths are exhausted. 
The robot will also have a memory where it will store the positions of the maze which it has visited, so as not to visit the same position in the DFS more than once, except when backtracking.
Once all the markers are collected, the robot will stop exploring the maze and backtrack its way to the home position, where it will drop all the markers.

### Memory Usage

This program efficiently allocates memory only when needed, ensuring optimal resource usage. Dynamic memory allocation is handled using malloc() and related functions, minimizing unnecessary overhead. Memory is freed explicitly using free() before termination, preventing leaks and maintaining a clean execution.

## Compilation and Execution Instructions

Compile the files on the zsh terminal using the following command:

    gcc -o maze_exploring_robot main.c maze.c robot.c display.c graphics.c

Run the program by piping it to the drawing app using the following command:

    ./maze_exploring_robot | java -jar drawapp-4.0.jar

## Functionality and Usage Example

Below is a video showing the robot collecting all the markers from a randomly generated arena.

https://github.com/user-attachments/assets/67aa26f3-9a48-4284-8bdc-3c6ee9d753c1
