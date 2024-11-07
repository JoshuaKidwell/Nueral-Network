#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <vector>
#include <string>

#pragma pack(push, 1)  // Disable padding
struct BMPHeader {
    uint16_t fileType;       // File type ("BM" for BMP files)
    uint32_t fileSize;       // File size in bytes
    uint16_t reserved1;      // Reserved
    uint16_t reserved2;      // Reserved
    uint32_t offsetData;     // Offset where the pixel array starts
};

struct DIBHeader {
    uint32_t headerSize;     // DIB header size
    int32_t width;           // Image width
    int32_t height;          // Image height
    uint16_t planes;         // Number of color planes (must be 1)
    uint16_t bitsPerPixel;   // Bits per pixel (e.g., 24 for RGB)
    uint32_t compression;    // Compression type (0 = uncompressed)
    uint32_t imageSize;      // Image size (can be 0 for uncompressed BMP)
    int32_t xPixelsPerMeter; // Horizontal resolution (pixels per meter)
    int32_t yPixelsPerMeter; // Vertical resolution (pixels per meter)
    uint32_t colorsInColorTable;  // Number of colors in the color palette
    uint32_t importantColorCount; // Number of important colors (0 = all)
};
#pragma pack(pop)  // Re-enable padding

class Bitmap
{
public:
    std::vector<std::vector<double>> bmpTo2DArray(const std::string& fileName);
    std::vector<double> bmpToArray(const std::string& fileName);
    std::vector<std::pair<std::vector<double>, double>> loadbmpsTo2DArray(const std::string& fileName, int);
};

#endif