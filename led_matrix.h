#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>

/* Hardware Configuration */
#define MATRIX_WIDTH  48
#define MATRIX_HEIGHT 32

/* LED Brightness Settings (0~255) */
#define LED_ON        255
#define LED_OFF       0

/* Font Settings (High-Res 16x24) */
#define FONT_W 16
#define FONT_H 24

/* * Global Frame Buffers
 * Declared extern to be accessible from main.c
 * Strictly follows the format required by the prompt.
 */
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

/* Function Prototypes */
void System_Init_Patterns(void);
void SPI_Send_Frame(unsigned int frame[MATRIX_HEIGHT][MATRIX_WIDTH]);
void Delay_ms(unsigned int ms);

#endif // LED_MATRIX_H
