#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    std::vector<Sector*> sectors;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void inOrderTraversal();
    void displaySectorsPreOrder();
    void preOrderTraversal();
    void displaySectorsPostOrder();
    void postOrderTraversal();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* minValueNode(Sector* node);
    Sector* findSector(const std::string& sector_code);
    std::vector<Sector*> getAllSectors(std::vector<Sector*> &sectors);
    void deleteAllSectors();
    void deleteNode(Sector* node);
};

#endif // SPACESECTORBST_H
