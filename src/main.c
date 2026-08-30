// #define DNCURSES_DLL

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<ncurses/ncurses.h>
#include "bmp.h"

char brightnessToASCII(uint8_t brightness);

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

    pixel* pixelBuffer;
    BMPHeader* fileHeader = newHeader(BMPfile);

    uint16_t imgWidth = fileHeader->width;
    uint16_t imgHeight = fileHeader->height;

    fillPixelBuffer(&pixelBuffer, fileHeader, BMPfile);

    uint16_t windowWidth;
    uint16_t windowHeight;

    initscr();
    noecho();
    erase();
    getmaxyx(stdscr, windowHeight, windowWidth);

    uint16_t displayedWidth = imgWidth;
    uint16_t displayedHeight = imgHeight;

    //TODO cant display anything larger than the terminal window
    if (windowHeight < imgHeight || windowWidth < imgWidth) {
        displayedWidth = windowWidth/imgWidth;
        displayedHeight = windowHeight/imgHeight;
    }

    for (uint16_t y = 0; y < displayedHeight; y++) {
        for (uint16_t x = 0; x < displayedWidth; x++) {
            pixel p = getPixel(pixelBuffer, imgWidth, y+1, x+1);
            uint8_t brightness = getBrightness(p);
            char c = brightnessToASCII(brightness);
            move(displayedHeight - y, x); //ncurses coords starts top left but bmp starts bottom left
            printw("%c", c);
        }
    }

    refresh();

    getch();
    endwin();
    free(pixelBuffer);
    free(fileHeader);
    fclose(BMPfile);
    return 0;
}

char brightnessToASCII(uint8_t brightness) {
    static const char ASCII[] = {'#','0','/','~',' '};
    
    uint8_t max = sizeof(ASCII);
    uint8_t resultIndex = max-1;
    
    for (uint8_t i = 0; i < max; i++) {
        if (brightness > (255/max)*(i+1)) {
            resultIndex--;
        }
    }
    return ASCII[resultIndex];
}