#include "SpaceSectorBST.h"
#include <vector>

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    deleteAllSectors();
}

void SpaceSectorBST::deleteAllSectors() {
    if (root == nullptr) {
        return;
    }

    if (root->left != nullptr) {
        root = root->left;
        Sector* temp = root->parent;
        deleteAllSectors();
        root = temp;
    }

    if (root->right != nullptr) {
        root = root->right;
        Sector* temp = root->parent;
        deleteAllSectors();
        root = temp;
    }

    delete root;
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    getline(inputFile, line); // Skip the header line

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string xStr, yStr, zStr;
        getline(ss, xStr, ',');
        getline(ss, yStr, ',');
        getline(ss, zStr, ',');

        int x = stoi(xStr);
        int y = stoi(yStr);
        int z = stoi(zStr);

        insertSectorByCoordinates(x, y, z);
    }

    inputFile.close();
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.

    if (root == nullptr) {
        root = new Sector(x, y, z);
        sectors.push_back(root);
        return;
    }

    if (x < root->x) {
        if (root->left == nullptr) {
            root->left = new Sector(x, y, z);
            sectors.push_back(root->left);
            root->left->parent = root;
        } else {
            root = root->left;
            insertSectorByCoordinates(x, y, z);
            root = root->parent;
        }
    } else if (x > root->x) {
        if (root->right == nullptr) {
            root->right = new Sector(x, y, z);
            sectors.push_back(root->right);
            root->right->parent = root;
        } else {
            root = root->right;
            insertSectorByCoordinates(x, y, z);
            root = root->parent;
        }
    } else {
        if (y < root->y) {
            if (root->left == nullptr) {
                root->left = new Sector(x, y, z);
                sectors.push_back(root->left);
                root->left->parent = root;
            } else {
                root = root->left;
                insertSectorByCoordinates(x, y, z);
                root = root->parent;
            }
        } else if (y > root->y) {
            if (root->right == nullptr) {
                root->right = new Sector(x, y, z);
                sectors.push_back(root->right);
                root->right->parent = root;
            } else {
                root = root->right;
                insertSectorByCoordinates(x, y, z);
                root = root->parent;
            }
        } else {
            if (z < root->z) {
                if (root->left == nullptr) {
                    root->left = new Sector(x, y, z);
                    sectors.push_back(root->left);
                    root->left->parent = root;
                } else {
                    root = root->left;
                    insertSectorByCoordinates(x, y, z);
                    root = root->parent;
                }
            } else if (z > root->z) {
                if (root->right == nullptr) {
                    root->right = new Sector(x, y, z);
                    sectors.push_back(root->right);
                    root->right->parent = root;
                } else {
                    root = root->right;
                    insertSectorByCoordinates(x, y, z);
                    root = root->parent;
                }
            }
        }
    }
}


void SpaceSectorBST::deleteNode(Sector* node) {
    if(node == nullptr) return;

    if(node->left == nullptr && node->right == nullptr){
        if(node == root) {
            root = nullptr;
        } else if(node->parent->left == node) {
            node->parent->left = nullptr;
        } else {
            node->parent->right = nullptr;
        }

        delete node;
    } else if(node->left == nullptr){
        if(node == root) {
            root = node->right;
        } else if(node->parent->left == node) {
            node->parent->left = node->right;
        } else {
            node->parent->right = node->right;
        }

        node->right->parent = node->parent;
        delete node;
    } else if(node->right == nullptr){
        if(node == root) {
             root = node->left;
        } else if(node->parent->left == node) {
            node->parent->left = node->left;
        } else {
            node->parent->right = node->left;
        }

        node->left->parent = node->parent;
        delete node;
    } else {

        Sector *successor = node->right;
        while(successor->left != nullptr) {
            successor = successor->left;
        }

        node->x = successor->x;
        node->y = successor->y;
        node->z = successor->z;
        node->distance_from_earth = successor->distance_from_earth;
        node->sector_code = successor->sector_code;

        if(successor->parent->left == successor) {
            successor->parent->left = successor->right;
        } else {
            successor->parent->right = successor->right;

        }

        if(successor->right != nullptr) {
            successor->right->parent = successor->parent;
        }

        delete successor;
    }
}


void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.

    Sector* current = findSector(sector_code);

    deleteNode(current);
}

void SpaceSectorBST::inOrderTraversal() {
    Sector* temp = root;

    if (root == nullptr) {
        return;
    }

    if (root->left != nullptr) {
        root = root->left;
        inOrderTraversal();
        root = temp;
    }

    cout << root->sector_code << endl;

    if (root->right != nullptr) {
        root = root->right;
        inOrderTraversal();
        root = temp;
    }
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.

    cout << "Space sectors inorder traversal:" << endl;
    inOrderTraversal();
    cout << endl;

}

void SpaceSectorBST::preOrderTraversal() {
    Sector* temp = root;

    if (root == nullptr) {
        return;
    }

    cout << root->sector_code << endl;

    if (root->left != nullptr) {
        root = root->left;
        preOrderTraversal();
        root = temp;
    }

    if (root->right != nullptr) {
        root = root->right;
        preOrderTraversal();
        root = temp;
    }
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.

    cout << "Space sectors preorder traversal:" << endl;
    preOrderTraversal();
    cout << endl;

}

void SpaceSectorBST::postOrderTraversal() {
    Sector* temp = root;

    if (root == nullptr) {
        return;
    }

    if (root->left != nullptr) {
        root = root->left;
        postOrderTraversal();
        root = temp;
    }

    if (root->right != nullptr) {
        root = root->right;
        postOrderTraversal();
        root = temp;
    }

    cout << root->sector_code << endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.

    cout << "Space sectors postorder traversal:" << endl;
    postOrderTraversal();
    cout << endl;
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    // First find the Earth sector
    Sector* earth = findSector("0SSS");

    // Then find the destination sector
    Sector* destination = findSector(sector_code);

    // Then find the path from the Earth to the destination sector
    // and return a vector of pointers to the Sector nodes that are on the path.
    // Make sure that there are no duplicate Sector nodes in the path!

    if (earth == nullptr || destination == nullptr) {
        return path;
    }

    Sector* current = earth;

    while (current != destination) {
        path.push_back(current);
        if (current->x < destination->x) {
            current = current->right;
        } else if (current->x > destination->x) {
            current = current->left;
        } else {
            if (current->y < destination->y) {
                current = current->right;
            } else if (current->y > destination->y) {
                current = current->left;
            } else {
                if (current->z < destination->z) {
                    current = current->right;
                } else if (current->z > destination->z) {
                    current = current->left;
                }
            }
        }
    }

    path.push_back(current);

    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.

    if (path.size() == 0) {
        cout << "A path to Dr. Elara could not be found." << endl;
        return;
    }

    cout << "The stellar path to Dr. Elara: ";
    for (int i = 0; i < path.size(); i++) {
        cout << path[i]->sector_code;
        if (i != path.size() - 1) {
            cout << "->";
        }
    }

    cout << endl;
}

Sector* SpaceSectorBST::minValueNode(Sector* node) {
    Sector* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// Write a function that maps the given sector code to the corresponding sector node in the BST.
// If the sector code is not found in the BST, return nullptr.
Sector* SpaceSectorBST::findSector(const std::string& sector_code) {
    for (int i = 0; i < sectors.size(); i++) {
        if (sectors[i]->sector_code == sector_code) {
            return sectors[i];
        }
    }

    return nullptr;
}