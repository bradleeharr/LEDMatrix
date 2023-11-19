/*
 * Author: Bradleeharr <https://github.com/bradleeharr/ESP32-64x64-LED-Matrix-Image-Display>
 * Portions of this code are adapted from Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from LedEffects Plasma by Robert Atkins: https://bitbucket.org/ratkins/ledeffects/src/26ed3c51912af6fac5f1304629c7b4ab7ac8ca4b/Plasma.cpp?at=default
 * Copyright (c) 2013 Robert Atkins
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 

// HUB75E pinout
// R1 | G1
// B1 | GND
// R2 | G2
// B2 | E
//  A | B
//  C | D
// CLK| LAT
// OE | GND

/*  Default library pin configuration for the reference
  you can redefine only ones you need later on object creation

#define R1 25
#define G1 26
#define BL1 27
#define R2 14
#define G2 12
#define BL2 13
#define CH_A 23
#define CH_B 19
#define CH_C 5
#define CH_D 17
#define CH_E -1 // assign to any available pin if using two panels or 64x64 panels with 1/32 scan
#define CLK 16
#define LAT 4
#define OE 15

*/


#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#include <FastLED.h>
#include "Image.h"
// Configure for your panel(s) as appropriate!
#define PANEL_WIDTH 64
#define PANEL_HEIGHT 64  	
#define PANELS_NUMBER 2 	// Number of chained panels
#define PIN_E 32

#define PANE_WIDTH PANEL_WIDTH * PANELS_NUMBER
#define PANE_HEIGHT PANEL_HEIGHT
const int IMAGE_WIDTH = 256; // The width of your image
const int IMAGE_HEIGHT = 128; // The height of your image
// placeholder for the matrix object
MatrixPanel_I2S_DMA *dma_display = nullptr;
const int buttonPin = 0; // Use GPIO0 as an example


uint16_t time_counter = 0, cycles = 0, fps = 0;
unsigned long fps_timer;
uint8_t brightness = 55;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(115200);
  
  Serial.println(F("*****************************************************"));
  Serial.println(F("*        ESP32 PICTURE DISPLAY        *"));
  Serial.println(F("*****************************************************"));

  /*
    Two 64x64 panels chained, so we the setup is customized like this
  */
  HUB75_I2S_CFG mxconfig;
  mxconfig.mx_height = PANEL_HEIGHT;     
  mxconfig.chain_length = PANELS_NUMBER; 
  mxconfig.gpio.e = PIN_E;                
  mxconfig.driver = HUB75_I2S_CFG::FM6124;     

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->setBrightness8(brightness);   
  if( not dma_display->begin() )
      Serial.println("****** !KABOOM! I2S memory allocation failed ***********");
 
  Serial.println("Fill screen: RED");
  dma_display->fillScreenRGB888(255, 0, 0);
  delay(100);
  Serial.println("Starting image display...");
  fps_timer = millis();
}
int scrollPosition = 0;  // Starting position for scrolling
const int scrollSpeed = 2;  // Number of rows to scroll per frame
unsigned long displayTime = 30000; // Time to display each image in milliseconds
unsigned long lastImageChangeTime = 0; // Last time the image was changed
int currentImageIndex = 0;

void loop() {
    unsigned long currentTime = millis();
    int buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
        brightness += 10;
        dma_display->setBrightness8(brightness);    // range is 0-255, 0 - 0%, 255 - 100%

        Serial.println("Button pressed!");
        while(digitalRead(buttonPin) == LOW) {
          delay(10);
        }
      }

    // Check if it's time to switch to the next image
    if (currentTime - lastImageChangeTime > displayTime) {
        currentImageIndex = (currentImageIndex + random(3)) % epd_bitmap_allArray_LEN;
        scrollPosition = 0;
        lastImageChangeTime = currentTime;
    }

    // Pointer to the current image
    const unsigned long* currentImage = epd_bitmap_allArray[currentImageIndex];
    int IMAGE_WIDTH_CURRENT = IMAGE_WIDTH; 
    int IMAGE_HEIGHT_CURRENT = IMAGE_HEIGHT;

    // Draw the current viewport of the rotated image
    for (int x = 0; x < PANE_WIDTH; x++) {
        for (int y = 0; y < PANE_HEIGHT; y++) {
            int imgX = (scrollPosition + y) % IMAGE_WIDTH_CURRENT;
            int imgY = IMAGE_HEIGHT_CURRENT - 1 - x;
            int index = imgY * IMAGE_WIDTH_CURRENT + imgX;
            uint32_t colorData = pgm_read_dword(&(currentImage[index]));
            uint8_t red = (colorData >> 16) & 0xFF;
            uint8_t green = (colorData >> 8) & 0xFF;
            uint8_t blue = colorData & 0xFF;

            dma_display->drawPixelRGB888(x, y, red, green, blue);
        }
    }

    // Update the scroll position
    scrollPosition = (scrollPosition + scrollSpeed) % IMAGE_WIDTH_CURRENT; 

    // Add a small delay to control the speed of scrolling
    delay(150);
}