# LED Matrix Countdown Firmware

## Project Overview
This firmware drives a 48x32 LED Matrix to display a countdown animation from 9 to 0 via SPI.

## Design Strategy

### 1. Memory Optimization & Compliance
The prompt requires defining ten `unsigned int Frame_Pattern[32][48]` arrays. Hard-coding these patterns would consume ~60KB of Flash and result in unmanageable code length.
**Solution:**
- The arrays are defined globally to meet the requirement but initialized to `0`.
- A **Runtime Expansion** technique is used. A compact 16x24 bit-mapped font (<1KB) is stored in Flash.
- On system startup, the `System_Init_Patterns()` function expands this font into the required 48x32 frame buffers.

### 2. Visual Quality (16x24 Font)
To avoid the "blocky" look of simply scaling a 6x8 font, this project uses a native **16x24 High-Resolution Font**.
- **Efficiency**: Each row is stored as a `uint16_t` (16 bits).
- **Aesthetics**: The numbers are smooth, centered, and proportional.

### 3. SPI Driver Logic
Implements standard SPI communication:
- **Chip Select (CS/Latch)**: Controlled to prevent ghosting during data transmission.
- **Data Protocol**: Transmits 32 rows x 48 columns of brightness data (0-255).

## File Structure
- `led_matrix.h`: Constants and API declarations.
- `led_matrix.c`: Font data, rendering algorithms, and SPI driver implementation.
- `main.c`: Main loop with countdown logic.
