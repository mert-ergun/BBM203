#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next block to appear in the game
    int position = 0; // Position of the blocks top left corner in the first row of the grid

    bool operator==(const Block& other) const {
        if (this->shape == other.shape) {
            return true;
        }
        return false;
    }

    bool operator!=(const Block& other) const {
        return !(*this == other);
    }
};


#endif //PA2_BLOCK_H
