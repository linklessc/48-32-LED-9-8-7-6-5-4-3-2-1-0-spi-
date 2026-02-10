#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>

/* =============================================
 * System Configuration
 * ============================================= */
#define MATRIX_WIDTH  48
#define MATRIX_HEIGHT 32

/* LED Brightness Levels (0 to 255) */
#define LED_ON        255
#define LED_OFF       0

/* Font Dimensions (High Resolution 16x24) */
#define FONT_W 16
#define FONT_H 24

/* =============================================
 * Global Frame Buffers
 * (Strictly following the specific variable naming)
 * ============================================= */
extern unsigned int Frame_Pattern0[MATRIX_HEIGHT][MATRIX_WIDTH];
extern unsigned int Frame_Pattern1[MATRIX_HEIGHT][MATRIX_WIDTH];
extern unsigned int Frame_Pattern2[MATRIX_HEIGHT][MATRIX_WIDTH];
extern unsigned int Frame_Pattern3[MATRIX_HEIGHT][MATRIX_WIDTH];
extern unsigned int Frame_Pattern4[MATRIX_HEIGHT][MATRIX_WIDTH];
extern unsigned int Frame_Pattern5[MATRIX_HEIGHT][MATRIX_WIDTH];
extern unsigned int Frame_Pattern6[MATRIX_HEIGHT][MATRIX_WIDTH];
extern unsigned int Frame_Pattern7[MATRIX_HEIGHT][MATRIX_WIDTH];
extern unsigned int Frame_Pattern8[MATRIX_HEIGHT][MATRIX_WIDTH];
extern unsigned int Frame_Pattern9[MATRIX_HEIGHT][MATRIX_WIDTH];

/* =============================================
 * Function Prototypes
 * ============================================= */

/**
 * @brief Initialize all frame patterns by expanding the bitmap font.
 * Must be called once before the main loop.
 */
void System_Init_Patterns(void);

/**
 * @brief Send a full frame to the LED driver via SPI.
 * @param frame Pointer to the 32x48 frame buffer.
 */
void SPI_Send_Frame(unsigned int frame[MATRIX_HEIGHT][MATRIX_WIDTH]);

/**
 * @brief Simple software delay.
 * @param ms Milliseconds to wait.
 */
void Delay_ms(unsigned int ms);

#endif // LED_MATRIX_H
