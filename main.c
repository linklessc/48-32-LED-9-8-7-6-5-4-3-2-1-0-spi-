#include "led_matrix.h"

int main(void) {
    /* 1. 硬體初始化 (GPIO, SPI Clock...) */
    // System_Init(); 

    /* 2. 準備圖案數據 */
    // 將 16x24 字庫載入到全域變數 Frame_Pattern0~9
    System_Init_Patterns();

    /* * 3. 建立指標陣列 (Pointer Array)
     * 這是一個陣列，裡面存的是 "指向二維陣列的指標"
     * 這樣我們就可以用 Frame_List[i] 來代表 Frame_PatternX
     */
    void *Frame_List[10] = {
        Frame_Pattern0, Frame_Pattern1, Frame_Pattern2, Frame_Pattern3,
        Frame_Pattern4, Frame_Pattern5, Frame_Pattern6, Frame_Pattern7,
        Frame_Pattern8, Frame_Pattern9
    };

    while(1) {
        /* * 使用迴圈倒數 (9 -> 0)
         * 這種寫法極度精簡，且擴充性高
         */
        for (int i = 9; i >= 0; i--) {
            // 傳入對應的 Frame Pattern 指標
            SPI_Send_Frame((unsigned int (*)[MATRIX_WIDTH])Frame_List[i]);
            
            // 延遲 1 秒
            Delay_ms(1000);
        }
    }
    return 0;
}
