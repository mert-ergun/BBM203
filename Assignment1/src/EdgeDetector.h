#ifndef EDGE_DETECTOR_H
#define EDGE_DETECTOR_H

#include "ImageMatrix.h"
#include "Convolution.h"
#include <vector>

class EdgeDetector {
    public:
        EdgeDetector(); 
        ~EdgeDetector(); 
        
        std::vector<std::pair<int, int>> detectEdges(const ImageMatrix& input_image);

    private:
        double **sobel_x_kernel;
        double **sobel_y_kernel;
        Convolution convolution_x;
        Convolution convolution_y;
};

#endif // EDGE_DETECTOR_H


