// Convolution.h

#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "ImageMatrix.h"

// Class `Convolution`: Provides the functionality to convolve an image with
// a kernel. Padding is a bool variable, indicating whether to use zero padding or not.
/**
 * @brief The Convolution class represents a convolution operation on an image using a kernel.
 * 
 */
class Convolution {
public:
    /**
     * @brief Construct a new Convolution object with default values.
     * 
     */
    Convolution(); // Default constructor

    /**
     * @brief Construct a new Convolution object with custom kernel and other parameters.
     * 
     * @param customKernel The custom kernel to be used for convolution.
     * @param kernelHeight The height of the kernel.
     * @param kernelWidth The width of the kernel.
     * @param stride The stride to be used for convolution.
     * @param padding The boolean variable indicating whether to use zero padding or not.
     */
    Convolution(double** customKernel, int kernelHeight, int kernelWidth, int stride, bool padding); // Parametrized constructor for custom kernel and other parameters

    /**
     * @brief Destroy the Convolution object.
     * 
     */
    ~Convolution(); // Destructor

    /**
     * @brief Construct a new Convolution object from another Convolution object.
     * 
     * @param other The other Convolution object to be copied.
     */
    Convolution(const Convolution &other); // Copy constructor

    /**
     * @brief Copy the contents of another Convolution object to this Convolution object.
     * 
     * @param other The other Convolution object to be copied.
     * @return Convolution& A reference to this Convolution object.
     */
    Convolution& operator=(const Convolution &other); // Copy assignment operator

    /**
     * @brief Convolve the input image with the kernel and return the convolved image.
     * 
     * @param input_image The input image to be convolved.
     * @return ImageMatrix The convolved image.
     */
    ImageMatrix convolve(const ImageMatrix& input_image) const; // Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.

    /**
     * @brief Perform zero padding on the input image.
     * 
     * @param input_image The input image to be padded.
     * @return ImageMatrix The padded image.
     */
    ImageMatrix zeroPadding(const ImageMatrix& input_image) const; // Function to perform zero padding on the input image

private:
    // Add any private member variables and functions.
    double** kernel; // The kernel to be used for convolution
    int kernelHeight; // The height of the kernel
    int kernelWidth; // The width of the kernel
    int stride; // The stride to be used for convolution
    bool padding; // The boolean variable indicating whether to use zero padding or not
};

#endif // CONVOLUTION_H
