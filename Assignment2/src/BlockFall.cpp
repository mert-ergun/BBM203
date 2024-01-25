#include "BlockFall.h"
#include <fstream>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    ifstream input(input_file);
    if (!input.is_open()) {
        exit(1);
    }
    string line;
    while (getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        if (line[0] == '[') {
            vector<vector<bool>> shape;
            vector<bool> row;
            for (char c : line) {
                if (c == '0') {
                    row.push_back(false);
                } else if (c == '1') {
                    row.push_back(true);
                }
            }
            shape.push_back(row);
            if (line.find(']') == string::npos) {
                while (getline(input, line)) {
                    if (line.empty()) {
                        break;
                    }
                    vector<bool> row;
                    for (char c : line) {
                        if (c == '0') {
                            row.push_back(false);
                        } else if (c == '1') {
                            row.push_back(true);
                        }
                    }
                    shape.push_back(row);
                    if (line.find(']') != string::npos) break;
                }
            }
            Block *block = new Block;
            block->shape = shape;

            if (initial_block == nullptr) {
                initial_block = block;
                active_rotation = block;
            } else {
                Block *temp = initial_block;
                while (temp->next_block != nullptr) {
                    temp = temp->next_block;
                }
                temp->next_block = block;
                block->next_block = nullptr;
            }

            block->left_rotation = nullptr;
            block->right_rotation = nullptr;
        } else {
            continue;
        }
    }

    // Set power_up
    Block *temp = initial_block;
    while (temp->next_block != nullptr) {
        temp = temp->next_block;
    }
    power_up = temp->shape;

    // Delete last block
    temp = initial_block;
    while (temp->next_block->next_block != nullptr) {
        temp = temp->next_block;
    }
    // Free memory
    delete temp->next_block;
    temp->next_block = nullptr;

    // Close input file
    input.close();

    temp = initial_block;
    while (temp != nullptr) {
        Block* right = new Block;
        Block* left = new Block;
        Block* down = new Block;

        right->shape = rotate_right(temp->shape);
        left->shape = rotate_left(temp->shape);
        down->shape = rotate_right(rotate_right(temp->shape));

        // Test the rotations
        if (temp->shape == right->shape || temp->shape == left->shape) {
            temp->right_rotation = temp;
            temp->left_rotation = temp;
            delete right;
            delete left;
            delete down;
            
            temp = temp->next_block;
            continue;
        }

        if (temp->shape == down->shape) {
            temp->right_rotation = right;
            temp->left_rotation = right;
            delete down;
            delete left;
            right->right_rotation = temp;
            right->left_rotation = temp;

            right->next_block = temp->next_block;
            
            temp = temp->next_block;
            continue;
        }

        temp->right_rotation = right;
        temp->left_rotation = left;

        down->right_rotation = left;
        down->left_rotation = right;
        down->next_block = temp->next_block;

        right->right_rotation = down;
        right->left_rotation = temp;
        right->next_block = temp->next_block;

        left->right_rotation = temp;
        left->left_rotation = down;
        left->next_block = temp->next_block;

        // Move to the next block in the linked list
        temp = temp->next_block;
    }
}


void BlockFall::initialize_grid(const string &input_file) {
    // Read input file
    ifstream input(input_file);
    if (!input.is_open()) {
        exit(1);
    }
    string line;
    while (getline(input, line)) {
        vector<int> row;
        for (char c : line) {
            if (c == '0') {
                row.push_back(0);
            } else if (c == '1') {
                row.push_back(1);
            }
        }
        grid.push_back(row);
    }

    // Set rows and cols
    rows = grid.size();
    cols = grid[0].size();

    // Close input file
    input.close();
}


BlockFall::~BlockFall() {
    Block* current = initial_block;
    while (current != nullptr) {
        Block* nextBlock = current->next_block;

        // Delete all rotations starting from current
        Block* rotation = current;
        do {
            Block* nextRotation = rotation->right_rotation;
            delete rotation;
            rotation = nextRotation;
        } while (rotation != current);

        current = nextBlock;
    }
}


vector<vector<bool>> BlockFall::rotate_left(const vector<vector<bool>> &shape) {
    int rows = shape.size();
    int cols = shape[0].size();
    vector<vector<bool>> rotated_shape(cols, vector<bool>(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = cols - 1; j >= 0; j--) {
            rotated_shape[cols - 1 - j][i] = shape[i][j];
        }
    }

    return rotated_shape;
}

vector<vector<bool>> BlockFall::rotate_right(const vector<vector<bool>> &shape) {
    int rows = shape.size();
    int cols = shape[0].size();
    vector<vector<bool>> rotated_shape(cols, vector<bool>(rows, false));

    for (int i = rows - 1; i >= 0; i--) {
        for (int j = 0; j < cols; j++) {
            rotated_shape[j][rows - 1 - i] = shape[i][j];
        }
    }

    return rotated_shape;
}