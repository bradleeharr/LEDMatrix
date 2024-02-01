# ESP32-64x64-LED-Matrix-Image-Display

<img src="https://github.com/bradleeharr/Image-Display-LED-Matrix/blob/main/20231121_182558%20(1).gif?raw=true"/>
Displaying images on a 64x64 LED matrix using an ESP32. The images are displayed using the HUB75E pinout and the FastLED library.

## Features

- Displays images on a 64x64 LED matrix
- Supports two chained 64x64 panels
- Can display images at different brightness levels
- Includes a scrolling feature for images larger than the display

## Installation and Use
1.  **Clone the Repository**:
2.  **Install the Necessary Libraries**: The project requires two libraries: ESP32-HUB75-MatrixPanel-I2S-DMA and FastLED. 
You can install these libraries using the Arduino IDE. 
  * open the Arduino IDE
  * go to Sketch > Include Library > Manage Libraries.
  * In the Library Manager, search for "ESP32-HUB75-MatrixPanel-I2S-DMA" and "FastLED", and install them.
4. **Connect Your ESP32 to Your Computer**: 
5. **Upload the Code to Your ESP32**: Open the Arduino IDE, select the correct board and port under Tools > Board and Tools > Port, and then click the Upload button.
6. **Connect your ESP32 to your LED display**

## Usage

Some sample images are provided in [Image.h](./Image.h). To add your own images, you would want to change the images in this file. You can use https://javl.github.io/image2cpp/ to convert images to bytemaps.

The images are packed together in an array `epd_bitmap_allArray.` so the images can be easily cycled through. The code will automatically start displaying images from the `epd_bitmap_allArray` array. The images are displayed in a random order and the display will scroll through the images. The brightness of the display can be adjusted using the button connected to GPIO0.

## Troubleshooting

If you encounter any issues, please check the following:

- Ensure all the necessary libraries are installed and correctly imported in your code.
- Check your wiring and connections to the LED matrix.
- If the code is crashing or behaving unexpectedly, try adjusting the brightness or changing the images being displayed.

if you have an issue that isn't listed, please feel free to add an issue or pull request.
## Acknowledgements

This project was made using the Aurora project and the LedEffects Plasma code. Many thanks to the authors of these projects for their work.
Images were converted to bytemaps using https://javl.github.io/image2cpp/.
---
