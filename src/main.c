// #define DNCURSES_DLL

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<math.h>
#include<ncurses/ncurses.h>
#include "bmp.h"

char brightnessToASCII(uint8_t brightness);
void resize(pixel* initBuffer, size_t initWidth, size_t initHeight, pixel** outBuffer, size_t outWidth, size_t outHeight);
void printImg(pixel* pbuffer, size_t height, size_t width);
static inline int clampInt(int val, int min, int max);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage %s FILEPATH", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* filePath = argv[1];
    FILE* BMPfile = fopen(filePath, "rb");

    if (BMPfile == NULL) {
        fprintf(stderr, "File could not be opened");
        exit(EXIT_FAILURE);
    }

    pixel* imgBuffer;
    BMPHeader* fileHeader = newHeader(BMPfile);

    uint16_t imgWidth = fileHeader->width;
    uint16_t imgHeight = fileHeader->height;

    fillPixelBuffer(&imgBuffer, fileHeader, BMPfile);

    uint16_t terminalWidth;
    uint16_t terminalHeight;

    initscr();
    noecho();
    erase();
    getmaxyx(stdscr, terminalHeight, terminalWidth);

    pixel* terminalBuffer = malloc(terminalWidth*terminalHeight*sizeof(pixel));
    
    size_t devar = terminalWidth*terminalHeight*sizeof(pixel);
    size_t devar2 = imgWidth*imgHeight;
    size_t devar3 = sizeof(pixel);
    size_t devar4 = sizeof(imgBuffer);

    if (terminalBuffer == NULL) {
        fprintf(stderr, "Terminal buffer allocation failed");
        exit(EXIT_FAILURE);
    }

    if (terminalHeight >= imgHeight && terminalWidth >= imgWidth) {
        printImg(imgBuffer, imgHeight, imgWidth);
    } else {
        resize(imgBuffer, imgWidth, imgHeight, &terminalBuffer, terminalWidth, terminalHeight);
        printImg(terminalBuffer, terminalHeight, terminalWidth);
    }

    refresh();

    getch();
    endwin();
    free(imgBuffer);
    free(terminalBuffer);
    free(fileHeader);
    fclose(BMPfile);
    return 0;
}

void resize(pixel* initBuffer, size_t initWidth, size_t initHeight, pixel** outBuffer, size_t outWidth, size_t outHeight) {
    float xRatio = (float)initWidth/outWidth;
    float yRatio = (float)initHeight/outHeight;

    for (size_t outY = 0; outY < outHeight; outY++) {

        float initY = (outY + 0.5) * yRatio - 0.5;
        int y = clampInt((int)roundf(initY), 0, initHeight - 1);
        
        for (size_t outX = 0; outX < outWidth; outX++) {
            float initX = (outX + 0.5) * xRatio - 0.5;
            int x = clampInt((int)roundf(initX), 0, initWidth - 1);

            int outDex = outY * outWidth + outX;
            int initDex = y * (initWidth) + x;
            (*outBuffer)[outDex] = initBuffer[initDex];
        }
    }
}

void printImg(pixel* pbuffer, size_t height, size_t width) {
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            pixel p = getPixel(pbuffer, width, y, x);
            uint8_t brightness = getBrightness(p);
            char c = brightnessToASCII(brightness);
            move(height - 1 - y, x); //ncurses coords starts top left but bmp starts bottom left so flipping the y is necessary
            printw("%c", c);
        }
    }
}

char brightnessToASCII(uint8_t brightness) {
    static const char ASCII[] = {'@', '#', '0', 'O', 'X', '/', '+' ,'~' ,'.' , ' '};
    
    uint16_t max = sizeof(ASCII);
    uint16_t resultIndex = max;
    
    for (uint8_t i = 0; i < max; i++) {
        if (brightness > (255/max)*(i+1)) {
            resultIndex--;
        }
    }
    return ASCII[resultIndex];
}

static inline int clampInt(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}