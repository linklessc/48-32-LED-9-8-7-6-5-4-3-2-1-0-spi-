#include "led_matrix.h"
#include <string.h>

/* ==========================================
 * 1. 符合題目要求的陣列定義
 * 初始化設為 {0} 以保持程式碼簡潔
 * 實際像素將在 System_Init 中自動生成
 * ========================================== */
unsigned int Frame_Pattern0[32][48] = {0};
unsigned int Frame_Pattern1[32][48] = {0};
unsigned int Frame_Pattern2[32][48] = {0};
unsigned int Frame_Pattern3[32][48] = {0};
unsigned int Frame_Pattern4[32][48] = {0};
unsigned int Frame_Pattern5[32][48] = {0};
unsigned int Frame_Pattern6[32][48] = {0};
unsigned int Frame_Pattern7[32][48] = {0};
unsigned int Frame_Pattern8[32][48] = {0};
unsigned int Frame_Pattern9[32][48] = {0};

/* 方便操作的指標陣列 */
unsigned int* Pattern_Table[10] = {
    (unsigned int*)Frame_Pattern0, (unsigned int*)Frame_Pattern1,
    (unsigned int*)Frame_Pattern2, (unsigned int*)Frame_Pattern3,
    (unsigned int*)Frame_Pattern4, (unsigned int*)Frame_Pattern5,
    (unsigned int*)Frame_Pattern6, (unsigned int*)Frame_Pattern7,
    (unsigned int*)Frame_Pattern8, (unsigned int*)Frame_Pattern9
};

/* ==========================================
 * 2. 7段顯示器 向量繪圖邏輯 (解決變形問題)
 * ========================================== */

// 7段顯示器遮罩: A, B, C, D, E, F, G
// 0:ABCDEF, 1:BC, 2:ABDEG...
const uint8_t SEG_MAP[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

// 輔助：畫矩形 (筆畫)
static void Draw_Rect(unsigned int frame[32][48], int x, int y, int w, int h) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if ((y+i) < MATRIX_HEIGHT && (x+j) < MATRIX_WIDTH) {
                frame[y+i][x+j] = LED_ON;
            }
        }
    }
}

// 核心：將數字寫入指定的 Frame Pattern
static void Generate_Digit(unsigned int frame[32][48], int num) {
    // 定義字體幾何 (置中、美觀)
    int x = 12, y = 2;   // 起始座標
    int w = 24, h = 28;  // 寬高
    int t = 3;           // 線條粗細 (Thickness) -> 這裡控制不變粗
    int h_half = h / 2;

    uint8_t mask = SEG_MAP[num];

    // 依據遮罩畫出筆畫
    if (mask & 0x01) Draw_Rect(frame, x, y, w, t);             // Seg A (上)
    if (mask & 0x02) Draw_Rect(frame, x+w-t, y, t, h_half);    // Seg B (右上)
    if (mask & 0x04) Draw_Rect(frame, x+w-t, y+h_half, t, h_half); // Seg C (右下)
    if (mask & 0x08) Draw_Rect(frame, x, y+h-t, w, t);         // Seg D (下)
    if (mask & 0x10) Draw_Rect(frame, x, y+h_half, t, h_half); // Seg E (左下)
    if (mask & 0x20) Draw_Rect(frame, x, y, t, h_half);        // Seg F (左上)
    if (mask & 0x40) Draw_Rect(frame, x, y+h_half-(t/2), w, t);// Seg G (中)
}

/* ==========================================
 * 3. 系統功能實作
 * ========================================== */

void Matrix_System_Init(void) {
    // 一次性生成所有圖案，填入 Frame_PatternX 陣列中
    for (int i = 0; i <= 9; i++) {
        // 將指標轉型回二維陣列指標進行傳遞
        Generate_Digit((unsigned int (*)[48])Pattern_Table[i], i);
    }
}

void SPI_Send_Frame(unsigned int frame[32][48]) {
    // 模擬 SPI 硬體操作
    // CS_Low();
    for (int r = 0; r < MATRIX_HEIGHT; r++) {
        for (int c = 0; c < MATRIX_WIDTH; c++) {
            // SPI_Write(frame[r][c]); 
            // 這裡填入實際 MCU 的 SPI 暫存器操作
        }
    }
    // CS_High();
}

void Delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 2000; j++);
}
