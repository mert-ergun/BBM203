#include "ImageSharpening.h"
#include <iostream>

// Default constructor
ImageSharpening::ImageSharpening() {
    kernelHeight = 3;
    kernelWidth = 3;
    blurring_kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        blurring_kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            blurring_kernel[i][j] = 1.0 / 9.0;
        }
    }
}

ImageSharpening::~ImageSharpening(){
    if (blurring_kernel != nullptr) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] blurring_kernel[i];
        }
        delete[] blurring_kernel;
    }
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    Convolution convolver(blurring_kernel, kernelHeight, kernelWidth, 1, 1);
    ImageMatrix blurredImage = convolver.convolve(input_image);
    ImageMatrix sharpenedImage = input_image + (input_image - blurredImage) * k; 

    //Fix the overflow after sharpening
    for (int i = 0; i < sharpenedImage.get_height(); i++) {
        for (int j = 0; j < sharpenedImage.get_width(); j++) {
            if (sharpenedImage.get_data(i,j) > 255) {
                sharpenedImage.set_data(i,j,255);
            }
            else if (sharpenedImage.get_data(i,j) < 0) {
                sharpenedImage.set_data(i,j,0);
            }
        }
    }

    return sharpenedImage;
}
