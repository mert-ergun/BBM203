#include "SpaceSectorLLRBT.h"
#include <algorithm>

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
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


void deleteAll(Sector* root) {
    if (root == nullptr) {
        return;
    }

    deleteAll(root->left);
    deleteAll(root->right);
    delete root;
}


// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    deleteAll(root);
}


Sector* rotateLeft(Sector* root) {
    Sector* child = root->right;
    Sector* childleft = child->left;
    child->left = root;
    root->right = childleft;
    child->color = root->color;
    root->color = RED;

    // Update parent pointers
    child->parent = root->parent;
    root->parent = child;
    if (childleft != nullptr) {
        childleft->parent = root;
    }

    return child;
}

Sector* rotateRight(Sector* root) {
    Sector* child = root->left;
    Sector* childright = child->right;
    child->right = root;
    root->left = childright;
    child->color = root->color;
    root->color = RED;

    // Update parent pointers
    child->parent = root->parent;
    root->parent = child;
    if (childright != nullptr) {
        childright->parent = root;
    }

    return child;
}

void flipColors(Sector* root) {
    root->color = RED;
    root->left->color = BLACK;
    root->right->color = BLACK;
}


Sector* insertSector(Sector* root, Sector* sector, Sector* parent) {
    if (root == nullptr) {
        sector->parent = parent;
        return sector;
    }

    if (sector->x < root->x) {
        root->left = insertSector(root->left, sector, root);
    }
    else if (sector->x > root->x) {
        root->right = insertSector(root->right, sector, root);
    }
    else {
        if (sector->y < root->y) {
            root->left = insertSector(root->left, sector, root);
        }
        else if (sector->y > root->y) {
            root->right = insertSector(root->right, sector, root);
        }
        else {
            if (sector->z < root->z) {
                root->left = insertSector(root->left, sector, root);
            }
            else if (sector->z > root->z) {
                root->right = insertSector(root->right, sector, root);
            } else {
                delete sector;
                return root;
            }
        }
    }


    if (root->right != nullptr && root->right->color == RED && (root->left == nullptr || root->left->color == BLACK)) {
        root = rotateLeft(root);
    }

    if (root->left != nullptr && root->left->color == RED && root->left->left != nullptr && root->left->left->color == RED) {
        root = rotateRight(root);
    }

    if (root->left != nullptr && root->left->color == RED && root->right != nullptr && root->right->color == RED) {
        flipColors(root);
    }

    return root;
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.

    Sector* sector = new Sector(x, y, z);
    root = insertSector(root, sector, nullptr);

    root->color = BLACK;
    sectors.push_back(sector);
}

void SpaceSectorLLRBT::inOrderTraversal() {
    Sector* temp = root;

    if (root == nullptr) {
        return;
    }

    if (root->left != nullptr) {
        root = root->left;
        inOrderTraversal();
        root = temp;
    }

    cout << (root->color ? "RED" : "BLACK") << " sector: " << root->sector_code << endl;

    if (root->right != nullptr) {
        root = root->right;
        inOrderTraversal();
        root = temp;
    }
}


void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.

    cout << "Space sectors inorder traversal:" << endl;
    inOrderTraversal();
    cout << endl;
}

void SpaceSectorLLRBT::preOrderTraversal() {
    Sector* temp = root;

    if (root == nullptr) {
        return;
    }

    cout << (root->color ? "RED" : "BLACK") << " sector: " << root->sector_code << endl;

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

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.

    cout << "Space sectors preorder traversal:" << endl;
    preOrderTraversal();
    cout << endl;
}

void SpaceSectorLLRBT::postOrderTraversal() {
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

    cout << (root->color ? "RED" : "BLACK") << " sector: " << root->sector_code << endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.

    cout << "Space sectors postorder traversal:" << endl;
    postOrderTraversal();
    cout << endl;
}

Sector* SpaceSectorLLRBT::findSector(const std::string& sector_code) {
    for (int i = 0; i < sectors.size(); i++) {
        if (sectors[i]->sector_code == sector_code) {
            return sectors[i];
        }
    }

    return nullptr;
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    // First find the earth sector
    Sector* earth = findSector("0SSS");
    
    // Then find the destination sector
    Sector* destination = findSector(sector_code);

    // Then find all the sectors on the path from the earth to root
    Sector* temp = earth;
    std::vector<Sector*> path1;
    while (temp != nullptr) {
        path1.push_back(temp);
        temp = temp->parent;
    }

    // Then find all the sectors on the path from the destination to root
    temp = destination;
    std::vector<Sector*> path2;
    while (temp != nullptr) {
        path2.push_back(temp);
        temp = temp->parent;
    }

    if (path1.size() == 0 || path2.size() == 0) {
        return path;
    }

    // Then find the common sector on the path
    Sector* commonSector = nullptr;
    for (int i = 0; i < path1.size(); i++) {
        for (int j = 0; j < path2.size(); j++) {
            if (path1[i] == path2[j]) {
                commonSector = path1[i];
                break;
            }
        }

        if (commonSector != nullptr) {
            break;
        }
    }

    // Then find the path from the earth to the common sector
    
    for (int i = 0; i < path1.size(); i++) {
        if (path1[i] == commonSector) {
            break;
        }

        path.push_back(path1[i]);
    }

    bool found = false;

    for (int i = path2.size() - 1; i >= 0; i--) {
        if (path2[i] == commonSector) {
            found = true;
        }

        if (found) {
            path.push_back(path2[i]);
        }
    }

    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.

    if (path.size() == 0) {
        cout << "A path to Dr. Elara could not be found." << endl;
        return;
    }

    cout << "The stellar path to Dr. Elara: ";

    for (int i = 0; i < path.size()-1; i++) {
        cout << path[i]->sector_code << "->";
    }
    cout << path[path.size() - 1]->sector_code << endl;
}