#include "led_matrix.h"

/* ==========================================
 * 1. Frame Buffer Definitions
 * Requirement: Must use unsigned int [32][48]
 * initialized to 0 to save code space.
 * ========================================== */
unsigned int Frame_Pattern0[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};
unsigned int Frame_Pattern1[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};
unsigned int Frame_Pattern2[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};
unsigned int Frame_Pattern3[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};
unsigned int Frame_Pattern4[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};
unsigned int Frame_Pattern5[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};
unsigned int Frame_Pattern6[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};
unsigned int Frame_Pattern7[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};
unsigned int Frame_Pattern8[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};
unsigned int Frame_Pattern9[MATRIX_HEIGHT][MATRIX_WIDTH] = {0};

/* Pointer array for easy iteration */
unsigned int* Pattern_Table[10] = {
    (unsigned int*)Frame_Pattern0, (unsigned int*)Frame_Pattern1,
    (unsigned int*)Frame_Pattern2, (unsigned int*)Frame_Pattern3,
    (unsigned int*)Frame_Pattern4, (unsigned int*)Frame_Pattern5,
    (unsigned int*)Frame_Pattern6, (unsigned int*)Frame_Pattern7,
    (unsigned int*)Frame_Pattern8, (unsigned int*)Frame_Pattern9
};

/* ==========================================
 * 2. 16x24 Compressed Font Library
 * Solves "Code Too Long" & "Blocky Graphics"
 * Format: 1 bit = 1 pixel. 16 bits = 1 row.
 * ========================================== */
const uint16_t Font_16x24[10][24] = {
    // Number 0
    {0x0000,0x0FF0,0x1FF8,0x381C,0x300C,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x6006,0x6006,0x6006,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x300C,0x381C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Number 1
    {0x0000,0x0180,0x0380,0x0780,0x0F80,0x1F80,0x3980,0x0180,
     0x0180,0x0180,0x0180,0x0180,0x0180,0x0180,0x0180,0x0180,
     0x0180,0x0180,0x0180,0x0180,0x0180,0x3FF8,0x0000,0x0000},
    // Number 2
    {0x0000,0x0FF0,0x1FF8,0x381C,0x300C,0x6006,0x0006,0x0006,
     0x000C,0x0018,0x0030,0x0060,0x00C0,0x0180,0x0300,0x0600,
     0x0C00,0x1800,0x3000,0x6000,0x7FFE,0x7FFE,0x0000,0x0000},
    // Number 3
    {0x0000,0x0FF0,0x1FF8,0x301C,0x000C,0x0006,0x0006,0x001C,
     0x00F0,0x03E0,0x03E0,0x00F0,0x001C,0x0006,0x0006,0x0006,
     0x0006,0x0006,0x300C,0x301C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Number 4
    {0x0000,0x0030,0x0070,0x00F0,0x01F0,0x0370,0x0670,0x0C70,
     0x1870,0x3070,0x6070,0xC070,0xC070,0xFFFF,0xFFFF,0x0070,
     0x0070,0x0070,0x0070,0x0070,0x0070,0x00F8,0x0000,0x0000},
    // Number 5
    {0x0000,0x7FFE,0x7FFE,0x6000,0x6000,0x6000,0x6000,0x6FF0,
     0x6FF8,0x601C,0x000C,0x0006,0x0006,0x0006,0x0006,0x0006,
     0x0006,0x0006,0x300C,0x381C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Number 6
    {0x0000,0x07F0,0x1FF8,0x380C,0x6000,0x6000,0x6000,0x6FF0,
     0x6FF8,0x781C,0x600C,0x6006,0x6006,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x300C,0x381C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Number 7
    {0x0000,0x7FFE,0x7FFE,0x0006,0x000C,0x0018,0x0018,0x0030,
     0x0030,0x0060,0x0060,0x00C0,0x00C0,0x0180,0x0180,0x0300,
     0x0300,0x0600,0x0600,0x0C00,0x0C00,0x1800,0x0000,0x0000},
    // Number 8
    {0x0000,0x0FF0,0x1FF8,0x300C,0x6006,0x6006,0x6006,0x300C,
     0x1FF8,0x0FF0,0x1FF8,0x300C,0x6006,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x300C,0x301C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Number 9
    {0x0000,0x0FF0,0x1FF8,0x380C,0x300C,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x6006,0x300E,0x181E,0x0FF6,0x0006,0x0006,
     0x000C,0x301C,0x1FF8,0x0FE0,0x0000,0x0000,0x0000,0x0000}
};

/* ==========================================
 * 3. Runtime Initialization Logic
 * ========================================== */

/* * Helper: Decompress 16x24 font into 48x32 Frame Buffer
 * Places the digit in the center of the LED Matrix.
 */
static void Load_Pattern(unsigned int frame[MATRIX_HEIGHT][MATRIX_WIDTH], int num) {
    // Calculate centering offsets
    int offset_x = (MATRIX_WIDTH - FONT_W) / 2; // Center horizontally
    int offset_y = (MATRIX_HEIGHT - FONT_H) / 2;// Center vertically

    for (int y = 0; y < FONT_H; y++) {
        uint16_t row_data = Font_16x24[num][y]; // Get 16-bit row data

        for (int x = 0; x < FONT_W; x++) {
            // Check bit from MSB to LSB (0x8000 -> 0x0001)
            if (row_data & (0x8000 >> x)) {
                // Set pixel brightness to Max
                frame[y + offset_y][x + offset_x] = LED_ON;
            } else {
                // Set pixel off
                frame[y + offset_y][x + offset_x] = LED_OFF;
            }
        }
    }
}

/* * API: Initialize all pattern arrays at startup.
 * Populates Frame_Pattern0 ~ Frame_Pattern9
 */
void System_Init_Patterns(void) {
    for (int i = 0; i < 10; i++) {
        // Cast the pointer back to a 2D array pointer
        Load_Pattern((unsigned int (*)[48])Pattern_Table[i], i);
    }
}

/* ==========================================
 * 4. SPI Driver Logic
 * ========================================== */

/* Macros for hardware abstraction (Replace with actual HAL) */
#define CS_PIN_LOW()  /* HAL_GPIO_WritePin(CS_PORT, CS_PIN, RESET) */
#define CS_PIN_HIGH() /* HAL_GPIO_WritePin(CS_PORT, CS_PIN, SET) */

/* Simulate SPI hardware transmission */
void MCU_SPI_Transmit(unsigned char data) {
    // Example for standard MCU:
    // while (!(SPI->SR & SPI_FLAG_TXE)); // Wait for buffer empty
    // SPI->DR = data;                    // Send data
    // while (SPI->SR & SPI_FLAG_BSY);    // Wait for completion
}

/* * API: Send a full 48x32 frame to the driver IC 
 */
void SPI_Send_Frame(unsigned int frame[MATRIX_HEIGHT][MATRIX_WIDTH]) {
    // 1. Pull CS Low to enable data latching sequence
    CS_PIN_LOW();

    // 2. Iterate through the matrix and send data
    // Assumes driver IC expects data row by row, or chained logic
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            // Cast unsigned int (0-255) to unsigned char for SPI
            unsigned char brightness = (unsigned char)frame[y][x];
            MCU_SPI_Transmit(brightness);
        }
    }

    // 3. Pull CS High to Latch/Update the display
    CS_PIN_HIGH();
}

/* Simple delay function */
void Delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for(i=0; i<ms; i++) for(j=0; j<2000; j++);
}
