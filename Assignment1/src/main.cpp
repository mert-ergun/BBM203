#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <utility> 
#include <cmath>

#include "ImageMatrix.h"
#include "Convolution.h"
#include "ImageSharpening.h"
#include "EncodeMessage.h"

using namespace std;


int main(){

    // Test 1 create an arbitrary 10x10 image
    const double* im1_data[10] = {
        new double[10] {182, 151, 174, 154, 119, 247, 43, 170, 39, 146},
        new double[10] {12, 49, 172, 73, 44, 81, 158, 205, 138, 19},
        new double[10] {251, 76, 218, 212, 188, 28, 24, 210, 0, 164},
        new double[10] {188, 96, 174, 77, 67, 134, 64, 33, 55, 182},
        new double[10] {28, 47, 97, 83, 101, 2, 46, 199, 36, 185},
        new double[10] {55, 89, 233, 221, 127, 194, 48, 147, 213, 105},
        new double[10] {86, 199, 15, 41, 131, 237, 48, 82, 45, 213},
        new double[10] {26, 188, 116, 14, 82, 96, 24, 173, 68, 238},
        new double[10] {46, 101, 106, 11, 129, 81, 21, 253, 252, 168},
        new double[10] {78, 105, 165, 8, 232, 81, 184, 91, 243, 69}
    };

    // create an ImageMatrix object with the above data
    ImageMatrix im1(im1_data,10, 10);

    // THESE ARE ARBITRARY EDGE PIXELS. THIS FUNCTION HAS NO DEPENDENCY ON THE EDGE DETECTION FUNCTION.
    vector<pair<int, int>> EdgePixels1 = {
        make_pair(0, 0),
        make_pair(0, 1),
        make_pair(0, 2),
        make_pair(0, 3),
        make_pair(0, 4),
        make_pair(1, 0),
        make_pair(1, 1),
        make_pair(1, 2),
        make_pair(1, 3),
        make_pair(1, 4),
        make_pair(2, 0),
        make_pair(2, 1),
        make_pair(2, 2),
        make_pair(2, 3),
        make_pair(2, 4),
        make_pair(3, 0),
        make_pair(3, 1),
        make_pair(3, 2),
        make_pair(3, 3),
        make_pair(3, 4),
        make_pair(4, 0),
        make_pair(4, 1),
        make_pair(4, 2),
        make_pair(4, 3),
        make_pair(4, 4),
    };

    EncodeMessage em1;
    ImageMatrix im1_encoded = em1.encodeMessageToImage(im1, "TestMessage", EdgePixels1);

    for (int i = 0; i < im1_encoded.get_height(); i++) {
        for (int j = 0; j < im1_encoded.get_width(); j++) {
            cout << im1_encoded.get_data(i, j) << " ";
        }
        cout << endl;
    }

}
