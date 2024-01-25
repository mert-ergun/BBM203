#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() {
    kernelHeight = 0;
    kernelWidth = 0;
    kernel = nullptr;
    stride = 1;
    padding = false;
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad){
    kernelHeight = kh;
    kernelWidth = kw;
    stride = stride_val;
    padding = pad;

    kernel = new double*[kernelHeight];
    for(int i = 0; i < kernelHeight; i++){
        kernel[i] = new double[kernelWidth];
        for(int j = 0; j < kernelWidth; j++){
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    if (kernel != nullptr) {
        for(int i = 0; i < kernelHeight; i++){
            delete[] kernel[i];
        }
        delete[] kernel;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other){
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;
    kernel = new double*[kernelHeight];
    for(int i = 0; i < kernelHeight; i++){
        kernel[i] = new double[kernelWidth];
        for(int j = 0; j < kernelWidth; j++){
            kernel[i][j] = other.kernel[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    // Check for self-assignment
    if(this == &other){
        return *this;
    }

    if (kernel != nullptr) {
        for(int i = 0; i < kernelHeight; i++){
            delete[] kernel[i];
        }
        delete[] kernel;
    }

    // Copy the data members
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;
    kernel = new double*[kernelHeight];
    for(int i = 0; i < kernelHeight; i++){
        kernel[i] = new double[kernelWidth];
        for(int j = 0; j < kernelWidth; j++){
            kernel[i][j] = other.kernel[i][j];
        }
    }

    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
/**
 * @brief Applies convolution operation on the input image with the given kernel.
 * 
 * @param input_image The input image to be convolved.
 * @return ImageMatrix The convolved image.
 */
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    // Create a new image matrix with the required dimensions
    ImageMatrix convolvingImage(input_image.get_height(), input_image.get_width());

    if (padding) {
        convolvingImage = zeroPadding(input_image);
    } else {
        convolvingImage = input_image;
    }

    // Calculate dimensions of the convolved image
    int convolvedHeight = ((input_image.get_height() - kernelHeight) + 2 * padding) / stride + 1;
    int convolvedWidth = ((input_image.get_width() - kernelWidth) + 2 * padding) / stride + 1;

    // Create a new image matrix with the required dimensions
    ImageMatrix convolvedImage(convolvedHeight, convolvedWidth);

    // Convolve the image
    for(int i = 0; i < convolvedHeight; i++){
        for(int j = 0; j < convolvedWidth; j++){
            double sum = 0;
            for(int m = 0; m < kernelHeight; m++){
                for(int n = 0; n < kernelWidth; n++){
                    sum += convolvingImage.get_data(i * stride + m, j * stride + n) * kernel[m][n];
                }
            }
            convolvedImage.set_data(i, j, sum);
        }
    }

    return convolvedImage;
}

ImageMatrix Convolution::zeroPadding(const ImageMatrix& input_image) const {
    // Get the dimensions of the input image
    int inputHeight = input_image.get_height();
    int inputWidth = input_image.get_width();

    // Create a new image matrix with the required dimensions
    ImageMatrix paddedImage(inputHeight + 2, inputWidth + 2);

    // Copy the input image to the padded image
    for(int i = 0; i < inputHeight; i++){
        for(int j = 0; j < inputWidth; j++){
            paddedImage.set_data(i + 1, j + 1, input_image.get_data(i, j));
        }
    }

    // Pad the first and last rows with zeros
    for(int j = 0; j < inputWidth + 2; j++){
        paddedImage.set_data(0, j, 0);
        paddedImage.set_data(inputHeight + 1, j, 0);
    }

    // Pad the first and last columns with zeros
    for(int i = 0; i < inputHeight + 2; i++){
        paddedImage.set_data(i, 0, 0);
        paddedImage.set_data(i, inputWidth + 1, 0);
    }

    return paddedImage;
}
