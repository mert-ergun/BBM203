#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {
    
}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    // Sharpen the image
    ImageSharpening sharpening;
    ImageMatrix sharpened_image = sharpening.sharpen(img, 2);

    // Detect edges in the image
    EdgeDetector edge_detector;
    std::vector<std::pair<int, int>> edge_pixels = edge_detector.detectEdges(sharpened_image);

    // Decode the message
    DecodeMessage decoder;
    std::string decoded_message = decoder.decodeFromImage(sharpened_image, edge_pixels);

    return decoded_message;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    // Sharpen the image
    ImageSharpening sharpening;
    ImageMatrix sharpened_image = sharpening.sharpen(img, 2);

    // Detect edges in the image
    EdgeDetector edge_detector;
    std::vector<std::pair<int, int>> edge_pixels = edge_detector.detectEdges(sharpened_image);

    // Encode the message
    EncodeMessage encoder;
    ImageMatrix encoded_image = encoder.encodeMessageToImage(img, message, edge_pixels);

    return encoded_image;
}
