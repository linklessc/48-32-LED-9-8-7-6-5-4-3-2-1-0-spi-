#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>

/* 硬體參數 */
#define MATRIX_WIDTH  48
#define MATRIX_HEIGHT 32
#define LED_ON        255
#define LED_OFF       0

/* * 宣告題目要求的 10 組 Frame Pattern 
 * 使用 extern 讓 main 可以存取
 */
extern unsigned int Frame_Pattern0[32][48];
extern unsigned int Frame_Pattern1[32][48];
extern unsigned int Frame_Pattern2[32][48];
extern unsigned int Frame_Pattern3[32][48];
extern unsigned int Frame_Pattern4[32][48];
extern unsigned int Frame_Pattern5[32][48];
extern unsigned int Frame_Pattern6[32][48];
extern unsigned int Frame_Pattern7[32][48];
extern unsigned int Frame_Pattern8[32][48];
extern unsigned int Frame_Pattern9[32][48];

/* 功能函式 */
void Matrix_System_Init(void); // 初始化並生成圖案
void SPI_Send_Frame(unsigned int frame[32][48]);
void Delay_ms(unsigned int ms);

#endif // LED_MATRIX_H
