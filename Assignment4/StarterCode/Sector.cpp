#include "Sector.h"
#include <cmath>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code

        // Distance formula: sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
        // Sector code: Distance(nearest integer) + (S if x == 0, R if x > 0, L if x < 0) + (S if y == 0, U if y > 0, D if y < 0) + (S if z == 0, F if z > 0, B if z < 0)

        // Example: (1, 2, 3) -> sqrt(1^2 + 2^2 + 3^2) = sqrt(14) = 3.74165738677 -> 4RUF

        distance_from_earth = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
        sector_code = std::to_string((int)distance_from_earth) + (x == 0 ? "S" : (x > 0 ? "R" : "L")) + (y == 0 ? "S" : (y > 0 ? "U" : "D")) + (z == 0 ? "S" : (z > 0 ? "F" : "B"));
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        distance_from_earth = other.distance_from_earth;
        sector_code = other.sector_code;
        left = other.left;
        right = other.right;
        parent = other.parent;
        color = other.color;
    }
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}