#include "led_matrix.h"

/* =============================================
 * 1. Global Frame Buffer Definitions
 * Initialize to 0 to save Flash space. 
 * Actual pixel data is loaded at runtime.
 * ============================================= */
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

/* Pointer array to simplify loop operations */
unsigned int* Pattern_Table[10] = {
    (unsigned int*)Frame_Pattern0, (unsigned int*)Frame_Pattern1,
    (unsigned int*)Frame_Pattern2, (unsigned int*)Frame_Pattern3,
    (unsigned int*)Frame_Pattern4, (unsigned int*)Frame_Pattern5,
    (unsigned int*)Frame_Pattern6, (unsigned int*)Frame_Pattern7,
    (unsigned int*)Frame_Pattern8, (unsigned int*)Frame_Pattern9
};

/* =============================================
 * 2. 16x24 Bitmap Font Data
 * Each row is represented by a uint16_t (16 pixels).
 * ============================================= */
const uint16_t Font_16x24[10][24] = {
    // Digit 0
    {0x0000,0x0FF0,0x1FF8,0x381C,0x300C,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x6006,0x6006,0x6006,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x300C,0x381C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Digit 1
    {0x0000,0x0180,0x0380,0x0780,0x0F80,0x1F80,0x3980,0x0180,
     0x0180,0x0180,0x0180,0x0180,0x0180,0x0180,0x0180,0x0180,
     0x0180,0x0180,0x0180,0x0180,0x0180,0x3FF8,0x0000,0x0000},
    // Digit 2
    {0x0000,0x0FF0,0x1FF8,0x381C,0x300C,0x6006,0x0006,0x0006,
     0x000C,0x0018,0x0030,0x0060,0x00C0,0x0180,0x0300,0x0600,
     0x0C00,0x1800,0x3000,0x6000,0x7FFE,0x7FFE,0x0000,0x0000},
    // Digit 3
    {0x0000,0x0FF0,0x1FF8,0x301C,0x000C,0x0006,0x0006,0x001C,
     0x00F0,0x03E0,0x03E0,0x00F0,0x001C,0x0006,0x0006,0x0006,
     0x0006,0x0006,0x300C,0x301C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Digit 4
    {0x0000,0x0030,0x0070,0x00F0,0x01F0,0x0370,0x0670,0x0C70,
     0x1870,0x3070,0x6070,0xC070,0xC070,0xFFFF,0xFFFF,0x0070,
     0x0070,0x0070,0x0070,0x0070,0x0070,0x00F8,0x0000,0x0000},
    // Digit 5
    {0x0000,0x7FFE,0x7FFE,0x6000,0x6000,0x6000,0x6000,0x6FF0,
     0x6FF8,0x601C,0x000C,0x0006,0x0006,0x0006,0x0006,0x0006,
     0x0006,0x0006,0x300C,0x381C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Digit 6
    {0x0000,0x07F0,0x1FF8,0x380C,0x6000,0x6000,0x6000,0x6FF0,
     0x6FF8,0x781C,0x600C,0x6006,0x6006,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x300C,0x381C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Digit 7
    {0x0000,0x7FFE,0x7FFE,0x0006,0x000C,0x0018,0x0018,0x0030,
     0x0030,0x0060,0x0060,0x00C0,0x00C0,0x0180,0x0180,0x0300,
     0x0300,0x0600,0x0600,0x0C00,0x0C00,0x1800,0x0000,0x0000},
    // Digit 8
    {0x0000,0x0FF0,0x1FF8,0x300C,0x6006,0x6006,0x6006,0x300C,
     0x1FF8,0x0FF0,0x1FF8,0x300C,0x6006,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x300C,0x301C,0x1FF8,0x0FF0,0x0000,0x0000},
    // Digit 9
    {0x0000,0x0FF0,0x1FF8,0x380C,0x300C,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x6006,0x300E,0x181E,0x0FF6,0x0006,0x0006,
     0x000C,0x301C,0x1FF8,0x0FE0,0x0000,0x0000,0x0000,0x0000}
};

/* =============================================
 * 3. Pattern Initialization Logic
 * ============================================= */

/**
 * @brief Helper function to decode bitmap and fill frame buffer.
 * @param frame Destination frame buffer.
 * @param num   The digit index (0-9).
 */
static void Load_Pattern(unsigned int frame[MATRIX_HEIGHT][MATRIX_WIDTH], int num) {
    // Calculate centering offsets
    int offset_x = (MATRIX_WIDTH - FONT_W) / 2; // (48-16)/2 = 16
    int offset_y = (MATRIX_HEIGHT - FONT_H) / 2;// (32-24)/2 = 4

    for (int y = 0; y < FONT_H; y++) {
        uint16_t row_data = Font_16x24[num][y];

        for (int x = 0; x < FONT_W; x++) {
            // Check bit (MSB first: 0x8000 >> x)
            if (row_data & (0x8000 >> x)) {
                frame[y + offset_y][x + offset_x] = LED_ON;
            } else {
                frame[y + offset_y][x + offset_x] = LED_OFF;
            }
        }
    }
}

void System_Init_Patterns(void) {
    for (int i = 0; i < 10; i++) {
        // Cast the pointer back to 2D array type for the helper function
        Load_Pattern((unsigned int (*)[MATRIX_WIDTH])Pattern_Table[i], i);
    }
}

/* =============================================
 * 4. SPI Driver Implementation
 * ============================================= */

/* Hardware Abstraction Macros (Replace with actual MCU Registers) */
#define CS_PIN_LOW()  /* GPIO_ResetBits(GPIOX, PIN_CS) */
#define CS_PIN_HIGH() /* GPIO_SetBits(GPIOX, PIN_CS) */

/**
 * @brief Transmit a single byte via SPI.
 * This is a hardware-dependent function.
 */
static void MCU_SPI_Transmit(unsigned char data) {
    // Example pseudo-code for hardware SPI:
    // SPI->DR = data;                  // Load data into Data Register
    // while( !(SPI->SR & SPI_BUSY) );  // Wait until transmission is complete
}

void SPI_Send_Frame(unsigned int frame[MATRIX_HEIGHT][MATRIX_WIDTH]) {
    /* Step 1: Assert Chip Select (Low) to begin transaction */
    CS_PIN_LOW();

    /* Step 2: Transmit pixel data (Row-major order assumed) */
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            // Convert 'unsigned int' brightness to 8-bit for SPI
            unsigned char brightness = (unsigned char)frame[y][x];
            MCU_SPI_Transmit(brightness);
        }
    }

    /* Step 3: De-assert Chip Select (High) to Latch/Update data */
    CS_PIN_HIGH();
}

void Delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 2000; j++) {
            // Simple busy loop
        }
    }
}
