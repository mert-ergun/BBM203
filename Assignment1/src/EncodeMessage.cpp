#include "EncodeMessage.h"
#include <cmath>

// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    // Character Transformation
    std::string transformedMessage = "";
    for (int i = 0; i < message.length(); i++) {
        bool prime = isPrime(i);
        
        if (prime) {
            int asciiValue = message[i] + fibonacci(i);
            if (asciiValue <= 32) {
                asciiValue += 33;
            } else if (asciiValue >= 127) {
                asciiValue = 126;
            }
            transformedMessage += static_cast<char>(asciiValue);
        } else {
            transformedMessage += message[i];
        }
    }

    // Right Circular Shifting
    int shiftAmount = message.length() / 2;
    std::string shiftedMessage = transformedMessage.substr(transformedMessage.length() - shiftAmount) + transformedMessage.substr(0, transformedMessage.length() - shiftAmount);

    // Message Embedding
    int bitIndex = 0;
    int pixelIndex = 0;
    int row = 0;
    int col = 0;
    ImageMatrix encodedImg = img;

    // Check if message is too long
    int maxMessageLength = std::min(static_cast<int>(positions.size() * 7), static_cast<int>(shiftedMessage.length()));

    for (int i = 0; i < maxMessageLength; i++) {
        int asciiValue = shiftedMessage[i];
        for (int j = 0; j < 7; j++) { // iterate only 7 times
            if (bitIndex >= positions.size()) {
                break;
            }
            row = positions[bitIndex].first;
            col = positions[bitIndex].second;
            int bit = (asciiValue >> j) & 1;
            int pixel = static_cast<int>(encodedImg.get_data(row, col)); 
            encodedImg.set_data(row, col, (pixel & 0xFE) | bit);
            bitIndex++;
        }
    }
    return encodedImg;
}

int EncodeMessage::fibonacci(int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

bool EncodeMessage::isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    int i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
        i += 6;
    }

    return true;
}