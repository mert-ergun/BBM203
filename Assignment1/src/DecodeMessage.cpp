// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string binaryString;
    int pixelValue;
    for (int i = 0; i < edgePixels.size(); i++) {
        pixelValue = image.get_data(edgePixels[i].first, edgePixels[i].second);
        binaryString += std::to_string(pixelValue & 1);
    }

    // Pad the binary string with leading zeros to form complete 7-bit bytes
    while (binaryString.size() % 7 != 0) {
        binaryString.insert(0, "0");
    }

    std::string decodedMessage;
    for (size_t i = 0; i < binaryString.size(); i += 7) {
        std::string byte = binaryString.substr(i, 7);
        int asciiValue = std::stoi(byte, nullptr, 2);

        // Ensure all characters are printable
        if (asciiValue <= 32) {
            asciiValue += 33;
        } else if (asciiValue == 127) {
            asciiValue = 126;
        }

        decodedMessage += static_cast<char>(asciiValue);
    }

    return decodedMessage;
}

