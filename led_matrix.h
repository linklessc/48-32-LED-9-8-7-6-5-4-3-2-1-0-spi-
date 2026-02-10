#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>

/* 硬體參數 */
#define MATRIX_WIDTH  48
#define MATRIX_HEIGHT 32
#define LED_ON        255
#define LED_OFF       0

/* 定義字庫大小 (使用 16x24 字型，顯示效果遠優於 6x8) */
#define FONT_W 16
#define FONT_H 24

/* 題目要求的 10 組 Frame Pattern (全域變數) */
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

/* API 宣告 */
void System_Init_Patterns(void);
void SPI_Send_Frame(unsigned int frame[MATRIX_HEIGHT][MATRIX_WIDTH]);
void Delay_ms(unsigned int ms);

#endif // LED_MATRIX_H
