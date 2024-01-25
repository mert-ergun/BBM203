#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool gameOver = false; // Boolean variable that indicates if the game is over

    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    void printGrid(BlockFall &game); // Function that prints the grid
    void rotateRight(BlockFall &game); // Function that rotates the block to the right
    void rotateLeft(BlockFall &game); // Function that rotates the block to the left
    void moveLeft(BlockFall &game); // Function that moves the block to the left
    void moveRight(BlockFall &game); // Function that moves the block to the right
    bool drop(BlockFall &game); // Function that drops the block
    void gravitySwitch(BlockFall &game); // Function that switches the gravity
    bool getBlock(BlockFall &game); // Function that gets the next block
    bool checkRows(BlockFall &game); // Function that checks if there are any full rows
    void clearRows(BlockFall &game); // Function that clears the rows
    void checkPowerUp(BlockFall &game); // Function that checks if the power-up detected
    void clearGrid(BlockFall &game); // Function that clears the grid
    void printGameOver(BlockFall &game, string cause); // Function that prints the game over message
    void printGridOver(BlockFall &game); // Function that prints the grid after the game is over
    void printGridNoCommand(BlockFall &game); // Function that prints the grid when there is no command
};


#endif //PA2_GAMECONTROLLER_H
