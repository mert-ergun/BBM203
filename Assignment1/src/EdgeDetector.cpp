// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {
    sobel_x_kernel = new double*[3];
    sobel_y_kernel = new double*[3];
    for (int i = 0; i < 3; ++i) {
        sobel_x_kernel[i] = new double[3];
        sobel_y_kernel[i] = new double[3];
    }

    sobel_x_kernel[0][0] = -1.0;
    sobel_x_kernel[0][1] = 0.0;
    sobel_x_kernel[0][2] = 1.0;
    sobel_x_kernel[1][0] = -2.0;
    sobel_x_kernel[1][1] = 0.0;
    sobel_x_kernel[1][2] = 2.0;
    sobel_x_kernel[2][0] = -1.0;
    sobel_x_kernel[2][1] = 0.0;
    sobel_x_kernel[2][2] = 1.0;

    sobel_y_kernel[0][0] = -1.0;
    sobel_y_kernel[0][1] = -2.0;
    sobel_y_kernel[0][2] = -1.0;
    sobel_y_kernel[1][0] = 0.0;
    sobel_y_kernel[1][1] = 0.0;
    sobel_y_kernel[1][2] = 0.0;
    sobel_y_kernel[2][0] = 1.0;
    sobel_y_kernel[2][1] = 2.0;
    sobel_y_kernel[2][2] = 1.0;

    convolution_x = Convolution(sobel_x_kernel, 3, 3, 1, true);
    convolution_y = Convolution(sobel_y_kernel, 3, 3, 1, true);
}

// Destructor
EdgeDetector::~EdgeDetector() {
    if (sobel_x_kernel != nullptr) {
        for (int i = 0; i < 3; ++i) {
            delete[] sobel_x_kernel[i];
            delete[] sobel_y_kernel[i];
        }
        delete[] sobel_x_kernel;
        delete[] sobel_y_kernel;
    }
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    // Gradient magnitude
    ImageMatrix gradient_magnitude(input_image.get_height(), input_image.get_width());
    ImageMatrix Ix = convolution_x.convolve(input_image);
    ImageMatrix Iy = convolution_y.convolve(input_image);

    for (int i = 0; i < input_image.get_height(); ++i) {
        for (int j = 0; j < input_image.get_width(); ++j) {
            gradient_magnitude.set_data(i, j, sqrt(Ix.get_data(i, j) * Ix.get_data(i, j) + Iy.get_data(i, j) * Iy.get_data(i, j)));
        }
    }

    // Determine the threshold
    double threshold = 0.0;
    for (int i = 0; i < input_image.get_height(); ++i) {
        for (int j = 0; j < input_image.get_width(); ++j) {
            threshold += gradient_magnitude.get_data(i, j);
        }
    }
    threshold /= (input_image.get_height() * input_image.get_width());

    // Detect edges
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < input_image.get_height(); ++i) {
        for (int j = 0; j < input_image.get_width(); ++j) {
            if (gradient_magnitude.get_data(i, j) > threshold) {
                edges.push_back(std::make_pair(i, j));
            }
        }
    }

    return edges;
}

