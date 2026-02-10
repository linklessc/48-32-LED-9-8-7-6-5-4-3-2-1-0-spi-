# LED Matrix Countdown Firmware

## Project Overview
This firmware is designed to drive a 48x32 LED Matrix using an MCU via SPI interface. It displays a countdown animation from 9 to 0.

## Design Strategy

### 1. Compliance with Requirements
The project strictly defines the 10 global arrays `unsigned int Frame_PatternX[32][48]` as requested. 

### 2. Memory & Visual Optimization (Bitmap Font)
Instead of hard-coding 10 huge arrays (which would consume ~60KB of Flash and create unmaintainable code), this solution uses a **Runtime Generation** approach:
- A compact **16x24 Bitmap Font** is defined (consuming only ~480 Bytes).
- During system initialization (`System_Init_Patterns`), the firmware decodes this bitmap and populates the required `Frame_Pattern` arrays.
- **Benefit**: This ensures the digits are high-resolution, perfectly centered, and aesthetically pleasing (no mosaic/aliasing effects from simple scaling), while keeping the source code clean.

### 3. SPI Driver Logic
The `SPI_Send_Frame` function implements a standard serial transmission protocol:
- **Chip Select (CS/Latch)**: Toggled to latch data into the LED driver IC to prevent ghosting.
- **Data Width**: Converts the `unsigned int` brightness level to 8-bit data for transmission.

## File Structure
- `led_matrix.h`: System parameters and function prototypes.
- `led_matrix.c`: Font data, pattern generation logic, and SPI driver implementation.
- `main.c`: Main execution loop handling the countdown sequence.