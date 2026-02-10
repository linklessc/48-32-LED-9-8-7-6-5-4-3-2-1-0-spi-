#include "led_matrix.h"

/* ==========================================
 * 1. Frame Buffer 定義 (符合題目要求)
 * 初始化為 0，稍後由程式填入
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

/* 方便迴圈操作的指標陣列 */
unsigned int* Pattern_Table[10] = {
    (unsigned int*)Frame_Pattern0, (unsigned int*)Frame_Pattern1,
    (unsigned int*)Frame_Pattern2, (unsigned int*)Frame_Pattern3,
    (unsigned int*)Frame_Pattern4, (unsigned int*)Frame_Pattern5,
    (unsigned int*)Frame_Pattern6, (unsigned int*)Frame_Pattern7,
    (unsigned int*)Frame_Pattern8, (unsigned int*)Frame_Pattern9
};

/* ==========================================
 * 2. 16x24 高解析字庫 (解決 Code 太長與變粗問題)
 * 每一行用 16-bit 表示 (0xXXXX)
 * ========================================== */
const uint16_t Font_16x24[10][24] = {
    // 0
    {0x0000,0x0FF0,0x1FF8,0x381C,0x300C,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x6006,0x6006,0x6006,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x300C,0x381C,0x1FF8,0x0FF0,0x0000,0x0000},
    // 1 (靠右對齊視覺修正)
    {0x0000,0x0180,0x0380,0x0780,0x0F80,0x1F80,0x3980,0x0180,
     0x0180,0x0180,0x0180,0x0180,0x0180,0x0180,0x0180,0x0180,
     0x0180,0x0180,0x0180,0x0180,0x0180,0x3FF8,0x0000,0x0000},
    // 2
    {0x0000,0x0FF0,0x1FF8,0x381C,0x300C,0x6006,0x0006,0x0006,
     0x000C,0x0018,0x0030,0x0060,0x00C0,0x0180,0x0300,0x0600,
     0x0C00,0x1800,0x3000,0x6000,0x7FFE,0x7FFE,0x0000,0x0000},
    // 3
    {0x0000,0x0FF0,0x1FF8,0x301C,0x000C,0x0006,0x0006,0x001C,
     0x00F0,0x03E0,0x03E0,0x00F0,0x001C,0x0006,0x0006,0x0006,
     0x0006,0x0006,0x300C,0x301C,0x1FF8,0x0FF0,0x0000,0x0000},
    // 4
    {0x0000,0x0030,0x0070,0x00F0,0x01F0,0x0370,0x0670,0x0C70,
     0x1870,0x3070,0x6070,0xC070,0xC070,0xFFFF,0xFFFF,0x0070,
     0x0070,0x0070,0x0070,0x0070,0x0070,0x00F8,0x0000,0x0000},
    // 5
    {0x0000,0x7FFE,0x7FFE,0x6000,0x6000,0x6000,0x6000,0x6FF0,
     0x6FF8,0x601C,0x000C,0x0006,0x0006,0x0006,0x0006,0x0006,
     0x0006,0x0006,0x300C,0x381C,0x1FF8,0x0FF0,0x0000,0x0000},
    // 6
    {0x0000,0x07F0,0x1FF8,0x380C,0x6000,0x6000,0x6000,0x6FF0,
     0x6FF8,0x781C,0x600C,0x6006,0x6006,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x300C,0x381C,0x1FF8,0x0FF0,0x0000,0x0000},
    // 7
    {0x0000,0x7FFE,0x7FFE,0x0006,0x000C,0x0018,0x0018,0x0030,
     0x0030,0x0060,0x0060,0x00C0,0x00C0,0x0180,0x0180,0x0300,
     0x0300,0x0600,0x0600,0x0C00,0x0C00,0x1800,0x0000,0x0000},
    // 8
    {0x0000,0x0FF0,0x1FF8,0x300C,0x6006,0x6006,0x6006,0x300C,
     0x1FF8,0x0FF0,0x1FF8,0x300C,0x6006,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x300C,0x301C,0x1FF8,0x0FF0,0x0000,0x0000},
    // 9
    {0x0000,0x0FF0,0x1FF8,0x380C,0x300C,0x6006,0x6006,0x6006,
     0x6006,0x6006,0x6006,0x300E,0x181E,0x0FF6,0x0006,0x0006,
     0x000C,0x301C,0x1FF8,0x0FE0,0x0000,0x0000,0x0000,0x0000}
};

/* ==========================================
 * 3. 系統初始化與繪圖邏輯
 * ========================================== */

/* 將壓縮的 Font_16x24 解壓到 Frame_Pattern */
void Load_Pattern(unsigned int frame[MATRIX_HEIGHT][MATRIX_WIDTH], int num) {
    // 計算置中座標
    int offset_x = (MATRIX_WIDTH - FONT_W) / 2; // (48-16)/2 = 16
    int offset_y = (MATRIX_HEIGHT - FONT_H) / 2;// (32-24)/2 = 4

    for (int y = 0; y < FONT_H; y++) {
        // 取出該 Row 的 16-bit 資料
        uint16_t row_data = Font_16x24[num][y];

        for (int x = 0; x < FONT_W; x++) {
            // 檢查 bit (從最高位元開始檢查)
            // 0x8000 是 binary 1000...0000
            if (row_data & (0x8000 >> x)) {
                // 填入 Frame Buffer，並加上偏移量
                frame[y + offset_y][x + offset_x] = LED_ON;
            } else {
                frame[y + offset_y][x + offset_x] = LED_OFF;
            }
        }
    }
}

/* 初始化所有圖案 */
void System_Init_Patterns(void) {
    for (int i = 0; i < 10; i++) {
        Load_Pattern((unsigned int (*)[48])Pattern_Table[i], i);
    }
}

/* ==========================================
 * 4. SPI 驅動邏輯
 * ========================================== */
void MCU_SPI_Write(unsigned int data) {
    // 模擬 SPI 硬體寫入
    // SPI->DR = data;
    // while(SPI->BSY);
}

void SPI_Send_Frame(unsigned int frame[MATRIX_HEIGHT][MATRIX_WIDTH]) {
    // 假設 Driver IC 是串接的，需要送出 32x48 個點的資料
    // CS_LOW();
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            MCU_SPI_Write(frame[y][x]);
        }
    }
    // CS_HIGH(); // Latch Data
}

void Delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for(i=0; i<ms; i++) for(j=0; j<2000; j++);
}
