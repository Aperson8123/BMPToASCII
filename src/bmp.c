#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include "bmp.h"

BMPHeader* newHeader(FILE* file) {
    BMPHeader* header = malloc(sizeof(BMPHeader));

    if (header == NULL) {
        fprintf(stderr, "File header memory allocation failed");
        exit(EXIT_FAILURE);
    }

    size_t headerElements = fread(header, sizeof(BMPHeader), 1, file);

    if (headerElements != 1) {
        fprintf(stderr, "Error reading the file header");
        exit(EXIT_FAILURE);
    }

    if (header->type != 0x4D42) {
        fprintf(stderr, "Filetype not recognized as BMP\n");
        exit(EXIT_FAILURE);
    }

    return header;
}

// TODO I think using pixel instead of unsigned char is prolly better
void fillPixelBuffer(pixel** pbuffer, BMPHeader* header, FILE* file) {
    uint32_t imageSize = header->imageSize;
    unsigned char* tempImageBuffer = malloc(imageSize);

    if (tempImageBuffer == NULL) {
        fprintf(stderr, "Image buffer memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    size_t tempImageBufferElements = fread(tempImageBuffer, sizeof(unsigned char), imageSize, file);

    if (tempImageBufferElements != imageSize) {
        fprintf(stderr, "Error when populating image buffer");
        exit(EXIT_FAILURE);
    }

    size_t pwidth = header->width;
    size_t pheight = header->height;
    uint8_t prowPadding = pwidth % 4;
    size_t rowPaddingSize = (prowPadding) * (pheight);

    *pbuffer = malloc(imageSize-rowPaddingSize);

    if (*pbuffer == NULL) {
        fprintf(stderr, "Pixel buffer memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < pheight; i++) {
        size_t pixelOffset = i * (pwidth);
        size_t byteOffset = i * pwidth * sizeof(pixel) + i * prowPadding;

        memcpy(*pbuffer + pixelOffset, tempImageBuffer + byteOffset, pwidth*sizeof(pixel));
    }

    free(tempImageBuffer);
}

pixel getPixel(pixel* pbuffer, uint16_t width, uint16_t y, uint16_t x) {
    uint16_t offset = (y*width + x+1)-1;
    return pbuffer[offset];
}

uint8_t getBrightness(pixel p) {
    return 0.2126*p.r + 0.7152*p.g + 0.0722*p.b;
}