#include "Bitmap.h"
#include <fstream>

std::vector<std::vector<double>> Bitmap::bmpTo2DArray(const std::string& fileName)
{
    // Open the BMP file in binary mode
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open BMP file.");
    }

    // Read BMP header
    BMPHeader bmpHeader;
    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));

    // Ensure the file is a BMP
    if (bmpHeader.fileType != 0x4D42) {  // 'BM' in hex
        throw std::runtime_error("Error: File is not a BMP file.");
    }

    // Read DIB header
    DIBHeader dibHeader;
    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));

    // Only support 24-bit uncompressed BMP files
    if (dibHeader.bitsPerPixel != 24 || dibHeader.compression != 0) {
        throw std::runtime_error("Error: Only 24-bit uncompressed BMP files are supported.");
    }

    int width = dibHeader.width;
    int height = dibHeader.height;

    // Create a 2D vector to hold the pixel data (normalized to doubles)
    std::vector<std::vector<double>> imageData(height, std::vector<double>(width));

    // BMP pixel data is padded to align to 4-byte boundaries
    int rowSize = (width * 3 + 3) & ~3;  // Row size (aligned to 4 bytes)

    // Seek to the start of the pixel array
    file.seekg(bmpHeader.offsetData, std::ios::beg);

    // Read pixel data row by row
    for (int i = 0; i < height; ++i) {
        std::vector<uint8_t> rowData(rowSize);
        file.read(reinterpret_cast<char*>(rowData.data()), rowSize);

        for (int j = 0; j < width; ++j) {
            // BMP stores pixels as BGR, so we need to reverse to RGB
            uint8_t blue = rowData[j * 3];
            uint8_t green = rowData[j * 3 + 1];
            uint8_t red = rowData[j * 3 + 2];

            // Convert the pixel to grayscale by averaging the RGB values
            double grayscaleValue = (red + green + blue) / 3.0;

            // Normalize the value to the range [0, 1] (if desired)
            imageData[height - i - 1][j] = pow((grayscaleValue / 255.0), 8);
        }
    }

    file.close();
    return imageData;
}

std::vector<double> Bitmap::bmpToArray(const std::string& fileName)
{
    // Open the BMP file in binary mode
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open BMP file.");
    }

    // Read BMP header
    BMPHeader bmpHeader;
    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));

    // Ensure the file is a BMP
    if (bmpHeader.fileType != 0x4D42) {  // 'BM' in hex
        throw std::runtime_error("Error: File is not a BMP file.");
    }

    // Read DIB header
    DIBHeader dibHeader;
    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));

    // Only support 24-bit uncompressed BMP files
    if (dibHeader.bitsPerPixel != 24 || dibHeader.compression != 0) {
        throw std::runtime_error("Error: Only 24-bit uncompressed BMP files are supported.");
    }

    int width = dibHeader.width;
    int height = dibHeader.height;

    // Create a 2D vector to hold the pixel data (normalized to doubles)
    std::vector<double> imageData;

    // BMP pixel data is padded to align to 4-byte boundaries
    int rowSize = (width * 3 + 3) & ~3;  // Row size (aligned to 4 bytes)

    // Seek to the start of the pixel array
    file.seekg(bmpHeader.offsetData, std::ios::beg);

    // Read pixel data row by row
    for (int i = 0; i < height; ++i) {
        std::vector<uint8_t> rowData(rowSize);
        file.read(reinterpret_cast<char*>(rowData.data()), rowSize);

        for (int j = 0; j < width; ++j) {
            // BMP stores pixels as BGR, so we need to reverse to RGB
            uint8_t blue = rowData[j * 3];
            uint8_t green = rowData[j * 3 + 1];
            uint8_t red = rowData[j * 3 + 2];

            // Convert the pixel to grayscale by averaging the RGB values
            double grayscaleValue = (red + green + blue) / 3.0;

            // Normalize the value to the range [0, 1] (if desired)
            imageData.push_back(pow((grayscaleValue / 255.0), 8));
        }
    }

    file.close();
    return imageData;
}

std::vector<std::pair<std::vector<double>, double>> Bitmap::loadbmpsTo2DArray(const std::string& fileName, int amount)
{
    std::vector<std::pair<std::vector<double>, double>> imageDataArr;
    std::string currentFile;

    int count = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < amount / 10; j++) {
            currentFile = fileName + '/' + std::to_string(i) + '/' + std::to_string(j) + ".bmp";
            imageDataArr.push_back({ bmpToArray(currentFile), i });
        }
    }

    return imageDataArr;
}
