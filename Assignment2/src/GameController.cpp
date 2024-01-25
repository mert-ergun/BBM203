#include "GameController.h"
#include <iostream>
#include <fstream>

bool GameController::play(BlockFall& game, const string& commands_file){
    // Read the commands from the input file
    ifstream commands(commands_file);
    if (!commands.is_open()) {
        cerr << "Error: Cannot open file " << commands_file << endl;
        return false;
    }

    getBlock(game);

    // Iterate through the commands vector and execute the commands
    string command;
    while (commands >> command) {
        if (command == "PRINT_GRID") {
            printGrid(game);
        } else if (command == "ROTATE_RIGHT") {
            rotateRight(game);
        } else if (command == "ROTATE_LEFT") {
            rotateLeft(game);
        } else if (command == "MOVE_LEFT") {
            moveLeft(game);
        } else if (command == "MOVE_RIGHT") {
            moveRight(game);
        } else if (command == "DROP") {
            if (!drop(game)) {
                printGameOver(game, "game_over");
                game.leaderboard.insert_new_entry(new LeaderboardEntry(game.current_score, time(nullptr), game.player_name));
                game.leaderboard.print_leaderboard();
                game.leaderboard.write_to_file(game.leaderboard_file_name);
                return false;
            }
        } else if (command == "GRAVITY_SWITCH") {
            gravitySwitch(game);
        } else {
            cerr << "Unknown command: " << command << endl;
        }
        if (gameOver) {
                game.leaderboard.insert_new_entry(new LeaderboardEntry(game.current_score, time(nullptr), game.player_name));
                game.leaderboard.print_leaderboard();
                game.leaderboard.write_to_file(game.leaderboard_file_name);
            return false;
        }
    }

    printGameOver(game, "game_finished");
    game.leaderboard.insert_new_entry(new LeaderboardEntry(game.current_score, time(nullptr), game.player_name));
    game.leaderboard.print_leaderboard();
    game.leaderboard.write_to_file(game.leaderboard_file_name);

    return false;
}

void GameController::printGameOver(BlockFall& game, string cause){
    if (gameOver) {
        return;
    }
    if (cause == "game_over") {
        cout << "GAME OVER!" << endl;
        cout << "Next block that couldn't fit:" << endl;
        vector<vector<bool>> shape = game.active_rotation->shape;
        int rows = shape.size();
        int cols = shape[0].size();
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; ++j) {
                if (shape[i][j] == 1) {
                    cout << occupiedCellChar;
                } else {
                    cout << unoccupiedCellChar;
                }
            }
            cout << endl;
        }
        cout << endl;
        cout << "Final grid and score:" << endl;
        cout << endl;
        printGridOver(game);
        cout << "Leaderboard" << endl;
        cout << "-----------" << endl;
    } else if (cause == "game_finished") {
        checkPowerUp(game);
        cout << "GAME FINISHED!" << endl;
        cout << "No more commands." << endl;
        cout << "Final grid and score:" << endl;
        cout << endl;
        printGridNoCommand(game);
        cout << "Leaderboard" << endl;
        cout << "-----------" << endl;
    } else if (cause == "you_win") {
        checkPowerUp(game);
        cout << "YOU WIN!" << endl;
        cout << "No more blocks." << endl;
        cout << "Final grid and score:" << endl;
        cout << endl;
        printGridOver(game);
        cout << "Leaderboard" << endl;
        cout << "-----------" << endl;
    }
    gameOver = true;
}

bool GameController::checkRows(BlockFall& game){
    for (int i = 0; i < game.rows; i++) {
        bool rowIsFull = true;
        for (int j = 0; j < game.cols; j++) {
            if (game.grid[i][j] == 0) {
                rowIsFull = false;
                break;
            }
        }
        if (rowIsFull) {
            return true;
        }
    }
    return false;
}

void GameController::clearRows(BlockFall& game){
    cout << "Before clearing:" << endl;
    for (int i = 0; i < game.rows; i++) {
        for (int j = 0; j < game.cols; j++) {
            if (game.grid[i][j] == 1) {
                cout << occupiedCellChar;
            } else {
                cout << unoccupiedCellChar;
            }
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
    
    int rowsCleared = 0;

    for (int i = 0; i < game.rows; i++) {
        bool rowIsFull = true;
        for (int j = 0; j < game.cols; j++) {
            if (game.grid[i][j] == 0) {
                rowIsFull = false;
                break;
            }
        }
        if (rowIsFull) {
            // Clear the row
            for (int j = 0; j < game.cols; j++) {
                game.grid[i][j] = 0;
            }
            rowsCleared++;

            // Move the rows above the cleared row down by 1
            for (int k = i - 1; k >= 0; k--) {
                for (int j = 0; j < game.cols; j++) {
                    game.grid[k + 1][j] = game.grid[k][j];
                }
            }
        }
    }

    // Update the score
    if (rowsCleared > 0) {
        game.current_score += rowsCleared * game.cols;
        if (game.current_score > game.leaderboard.head_leaderboard_entry->score) {
            LeaderboardEntry* new_entry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
            game.leaderboard.insert_new_entry(new_entry);
        }
    }
}

bool GameController::getBlock(BlockFall& game){
    Block* block = game.active_rotation;
    
    // Try to place the block in the top-left corner of the grid
    int blockRows = block->shape.size();
    int blockCols = block->shape[0].size();
    bool blockFits = true;

    // Check if the block fits in the top-left corner of the grid
    for (int i = 0; i < blockRows; i++) {
        for (int j = 0; j < blockCols; j++) {
            if (game.grid[i][j] == 1 && block->shape[i][j] == 1) {
                blockFits = false;
                break;
            }
        }
    }

    // If the block doesn't fit in the top-left corner, game over
    if (!blockFits) {
        return false;
    }

    return true;
}

void GameController::moveLeft(BlockFall &game) {
    Block* block = game.active_rotation;
    int blockRows = block->shape.size();
    int blockCols = block->shape[0].size();
    int blockPosition = block->position;

    // Check if the block can move to the left
    if (blockPosition <= 0) {
        return; // Block is at the leftmost position
    }

    // Check if the block can move to the left
    for (int i = 0; i < blockRows; i++) {
        for (int j = 0; j < blockCols; j++) {
            if (block->shape[i][j] == 1 && game.grid[i][blockPosition + j - 1] == 1) {
                return;
            }
        }
    }

    // Move the block to the left
    block->position--;
}

void GameController::moveRight(BlockFall &game) {
    Block* block = game.active_rotation;
    int blockRows = block->shape.size();
    int blockCols = block->shape[0].size();
    int blockPosition = block->position;

    // Check if the block can move to the right
    if (blockPosition + blockCols >= game.cols) {
        return; // Block is at the rightmost position
    }

    // Check if the block can move to the right
    for (int i = 0; i < blockRows; i++) {
        for (int j = blockCols - 1; j >= 0; j--) {
            if (block->shape[i][j] == 1 && game.grid[i][blockPosition + j + 1] == 1) {
                return;
            }
        }
    }

    // Move the block to the right
    block->position++;
}


void GameController::rotateLeft(BlockFall& game){
    Block* block = game.active_rotation;
    Block* leftRotation = block->left_rotation;
    int blockRows = block->shape.size();
    int blockCols = block->shape[0].size();
    int blockPosition = block->position;

    // Rotate the block to the left
    block = leftRotation;
    block->position = blockPosition;

    // Get new dimensions of the rotated block
    blockRows = block->shape.size();
    blockCols = block->shape[0].size();

    if (blockPosition + blockCols >= game.cols || blockPosition < 0) {
        // Rotate the block back to its original position
        block = block->right_rotation;
        block->position = blockPosition;
        return;
    }

    for (int i = 0; i < game.rows; i++) {
        for (int j = 0; j < game.cols; j++) {
            int diff = j - blockPosition;

            if (diff >= 0 && diff < blockCols && i < blockRows) {
                if (block->shape[i][diff] == 1 && game.grid[i][j] == 1) {
                    // Rotate the block back to its original position
                    block = block->right_rotation;
                    block->position = blockPosition;
                    return;
                }
            }
        }
    }

    game.active_rotation = block;
}

void GameController::rotateRight(BlockFall& game){
    // Same as rotateLeft() but with the right rotation
    Block* block = game.active_rotation;
    Block* rightRotation = block->right_rotation;
    int blockRows = block->shape.size();
    int blockCols = block->shape[0].size();
    int blockPosition = block->position;

    // Rotate the block to the right
    block = rightRotation;
    block->position = blockPosition;

    // Get new dimensions of the rotated block
    blockRows = block->shape.size();
    blockCols = block->shape[0].size();

    if (blockPosition + blockCols >= game.cols || blockPosition < 0) {
        // Rotate the block back to its original position
        block = block->left_rotation;
        block->position = blockPosition;
        return;
    }

    for (int i = 0; i < game.rows; i++) {
        for (int j = 0; j < game.cols; j++) {
            int diff = j - blockPosition;

            if (diff >= 0 && diff < blockCols && i < blockRows) {
                if (block->shape[i][diff] == 1 && game.grid[i][j] == 1) {
                    // Rotate the block back to its original position
                    block = block->left_rotation;
                    block->position = blockPosition;
                    return;
                }
            }
        }
    }

    game.active_rotation = block;
}

bool GameController::drop(BlockFall& game) {
    Block* block = game.active_rotation;
    int blockRows = block->shape.size();
    int blockCols = block->shape[0].size();
    int blockPosition = block->position;

    int blockCells = 0;
    for (int i = 0; i < blockRows; ++i) {
        for (int j = 0; j < blockCols; ++j) {
            if (block->shape[i][j] == 1) {
                ++blockCells;
            }
        }
    }

    // Find how far down the block can move
    int dropDistance = 0;
    bool canDrop = true;
    while (canDrop) {
        for (int i = 0; i < blockRows; ++i) {
            for (int j = 0; j < blockCols; ++j) {
                if (block->shape[i][j] == 1) {
                    // Check if the block has hit the bottom or another block
                    if (i + dropDistance + 1 >= game.rows || game.grid[i + dropDistance + 1][j + blockPosition] == 1) {
                        canDrop = false;
                        break;
                    }
                }
            }
            if (!canDrop) {
                break;
            }
        }
        if (canDrop) {
            ++dropDistance;
        }
    }

    if (game.gravity_mode_on) {
        // Apply gravity to each cell in the block
        for (int j = 0; j < blockCols; ++j) {
            int dropDistance = 0;
            for (int i = blockRows - 1; i >= 0; --i) {
                if (block->shape[i][j] == 1) {
                    // Calculate drop distance for this cell
                    dropDistance = 0;
                    while (i + dropDistance + 1 < game.rows && game.grid[i + dropDistance + 1][j + blockPosition] == 0) {
                        ++dropDistance;
                    }
                    // Move the cell down by 'dropDistance'
                    game.grid[i][j + blockPosition] = 0;
                    game.grid[i + dropDistance][j + blockPosition] = 1;
                }
            }
        }
    } else {
        // Normal drop (without gravity)

        // Redraw the block at the new position
        for (int i = 0; i < blockRows; ++i) {
            for (int j = 0; j < blockCols; ++j) {
                if (block->shape[i][j] == 1) {
                    game.grid[i + dropDistance][j + blockPosition] = 1;
                }
            }
        }
    }

    game.current_score += blockCells * dropDistance;
    if (game.current_score > game.leaderboard.head_leaderboard_entry->score) {
        LeaderboardEntry* new_entry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
        game.leaderboard.insert_new_entry(new_entry);
    }

    if (block->next_block == nullptr) {
        printGameOver(game, "you_win");
        return true;
    }

    // Prepare for the next block
    game.active_rotation = block->next_block;

    checkPowerUp(game);

    if (checkRows(game)) {
        clearRows(game);
    }

    return getBlock(game);
}



void GameController::printGrid(BlockFall& game){
    cout << "Score: " << game.current_score << endl;
    cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << endl;
    for (int i = 0; i < game.rows; i++) {
        for (int j = 0; j < game.cols; j++) {
            int diff = j - game.active_rotation->position;
            int activeBlockCell = 0;

            if (diff >= 0 && diff< game.active_rotation->shape[0].size() && i < game.active_rotation->shape.size()) {
                activeBlockCell = game.active_rotation->shape[i][diff];
            } 
            if (game.grid[i][j] == 1 || activeBlockCell == 1) {
                cout << occupiedCellChar;
            } else {
                cout << unoccupiedCellChar;
            }
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}

void GameController::printGridOver(BlockFall& game) {
    cout << "Score: " << game.current_score << endl;
    cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << endl;
    // Print the grid
    for (int i = 0; i < game.rows; i++) {
        for (int j = 0; j < game.cols; j++) {
            if (game.grid[i][j] == 1) {
                cout << occupiedCellChar;
            } else {
                cout << unoccupiedCellChar;
            }
        }
        cout << endl;
    }
    cout << endl;
}

void GameController::printGridNoCommand(BlockFall& game) {
    // Same as the printGrid() function, but without the active block
    cout << "Score: " << game.current_score << endl;
    cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << endl;
    // Create a temporary grid to mark the cells occupied by the active block
    std::vector<std::vector<bool>> tempGrid(game.rows, std::vector<bool>(game.cols, false));
    Block* activeBlock = game.active_rotation;
    int blockRows = activeBlock->shape.size();
    int blockCols = activeBlock->shape[0].size();
    int blockPosition = activeBlock->position;
    for (int i = 0; i < blockRows; ++i) {
        for (int j = 0; j < blockCols; ++j) {
            if (activeBlock->shape[i][j] == 1) {
                tempGrid[i][j + blockPosition] = true;
            }
        }
    }

    // Print the grid
    for (int i = 0; i < game.rows; i++) {
        for (int j = 0; j < game.cols; j++) {
            if (tempGrid[i][j] == 1) {
                cout << unoccupiedCellChar;
            } else if (game.grid[i][j] == 1) {
                cout << occupiedCellChar;
            } else {
                cout << unoccupiedCellChar;
            }
        }
        cout << endl;
    }
    cout << endl;
}

void GameController::gravitySwitch(BlockFall& game) {
    // Toggle the gravity mode
    game.gravity_mode_on = !game.gravity_mode_on;

    // Apply gravity to the entire grid if gravity mode is turned on
    if (game.gravity_mode_on) {
        Block* activeBlock = game.active_rotation;
        int blockRows = activeBlock->shape.size();
        int blockCols = activeBlock->shape[0].size();
        int blockPosition = activeBlock->position;

        // Create a temporary grid to mark the cells occupied by the active block
        std::vector<std::vector<bool>> tempGrid(game.rows, std::vector<bool>(game.cols, false));
        for (int i = 0; i < blockRows; ++i) {
            for (int j = 0; j < blockCols; ++j) {
                if (activeBlock->shape[i][j] == 1) {
                    tempGrid[i][j + blockPosition] = true;  // Mark this cell as occupied by the active block
                }
            }
        }

        // Iterate over each column
        for (int j = 0; j < game.cols; ++j) {
            // Apply gravity from bottom to top in each column
            int emptySpace = 0;  // Tracks the number of empty cells below the current cell
            for (int i = game.rows - 1; i >= 0; --i) {
                if (tempGrid[i][j] || game.grid[i][j] == 0) {
                    // Increase the count of consecutive empty cells, or skip if it's part of the active block
                    emptySpace++;
                } else if (emptySpace > 0) {
                    // If there is an occupied cell and there are empty cells below it, move the cell down
                    game.grid[i + emptySpace][j] = game.grid[i][j];
                    game.grid[i][j] = 0;
                }
            }
        }
    }
    clearRows(game);
}

void GameController::checkPowerUp(BlockFall& game) {
    vector<vector<bool>> specPowerUp = game.power_up;
    vector<vector<bool>> defaultPowerUp = {
        {0, 1, 0, 1, 0}, 
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0}};

    int specPowerUpRows = specPowerUp.size();
    int specPowerUpCols = specPowerUp[0].size();
    int defaultPowerUpRows = defaultPowerUp.size();
    int defaultPowerUpCols = defaultPowerUp[0].size();

    // Check grid for power-up shapes
    for (int i = 0; i <= game.rows - specPowerUpRows; i++) {
        for (int j = 0; j <= game.cols - specPowerUpCols; j++) {
            bool specPowerUpFound = true;
            bool defaultPowerUpFound = true;

            for (int k = 0; k < specPowerUpRows; k++) {
                for (int l = 0; l < specPowerUpCols; l++) {
                    if (game.grid[i + k][j + l] != specPowerUp[k][l]) {
                        specPowerUpFound = false;
                        break;
                    }
                }
                if (!specPowerUpFound) {
                    break;
                }
            }

            for (int k = 0; k < defaultPowerUpRows; k++) {
                for (int l = 0; l < defaultPowerUpCols; l++) {
                    if (i+k >= game.rows || j+l >= game.cols) {
                        defaultPowerUpFound = false;
                        break;
                    }
                    if (game.grid[i + k][j + l] != defaultPowerUp[k][l]) {
                        defaultPowerUpFound = false;
                        break;
                    }
                }
                if (!defaultPowerUpFound) {
                    break;
                }
            }

            if (specPowerUpFound || defaultPowerUpFound) {
                clearGrid(game);
                return;  // Stop checking once a power-up is found and the grid is cleared
            }
        }
    }
}


void GameController::clearGrid(BlockFall& game){

    cout << "Before clearing:" << endl;
    for (int i = 0; i < game.rows; i++) {
        for (int j = 0; j < game.cols; j++) {
            if (game.grid[i][j] == 1) {
                cout << occupiedCellChar;
            } else {
                cout << unoccupiedCellChar;
            }
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;

    int occupiedCells = 0;

    // Clear the grid
    for (int i = 0; i < game.rows; i++) {
        for (int j = 0; j < game.cols; j++) {
            if (game.grid[i][j] == 1) {
                game.grid[i][j] = 0;
                occupiedCells++;
            }
        }
    }

    // Update the score
    game.current_score += occupiedCells + 1000;
    if (game.current_score > game.leaderboard.head_leaderboard_entry->score) {
        LeaderboardEntry* new_entry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
        game.leaderboard.insert_new_entry(new_entry);
    }
}

