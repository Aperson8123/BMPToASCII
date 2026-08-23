#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t colorPanes;
    uint16_t colorDepth;
    uint32_t compressionMethod;
    uint32_t imageSize;
    uint32_t pwidth;;
    uint32_t pheight; 
    uint32_t colorPaletteCount;
    uint32_t importantColors;
} BMPHeader;

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} pixel;
#pragma pack(pop)

//temporary
#define PATH "C:\\Cstuff\\data\\ColorTest.bmp"

int main(void) {
    FILE* BMPfile = fopen(PATH, "rb");

    if (BMPfile == NULL) {
        fprintf(stderr, "File could not be opened");
        exit(EXIT_FAILURE);
    }
    
    BMPHeader* fileHeader = malloc(sizeof(BMPHeader));
    size_t fileHeaderElements = fread(fileHeader, sizeof(BMPHeader), 1, BMPfile);

    if (fileHeaderElements != 1) {
        fprintf(stderr, "Error reading the file header");
        exit(EXIT_FAILURE);
    }

    if (fileHeader->type != 0x4D42) {
        printf("Filetype not recognized as BMP\n");
    }
    
    pixel* pbuffer = malloc(sizeof(fileHeader->imageSize));
    size_t pixelCount = fread(pbuffer, sizeof(pixel), fileHeader->imageSize/sizeof(pixel), BMPfile);

    size_t pwidth = fileHeader->pwidth;
    size_t rowPaddingSize = (pwidth % 4)*fileHeader->height;
    pixelCount -= rowPaddingSize/sizeof(pixel);

    printf("pixelCount: %lld\nrowPadding: %lld\n", pixelCount, rowPaddingSize);

    free(pbuffer);
    free(fileHeader);
    fclose(BMPfile);
    return 0;
}