#include "led_matrix.h"

int main(void) {
    /* 1. Hardware Initialization */
    // Initialize GPIO, SPI Clock, etc.
    // HAL_Init(); 
    // SPI_Init();

    /* 2. Pattern Initialization (CRITICAL STEP)
     * Expands the compressed bitmap font into the Frame_Pattern arrays.
     * This fulfills the requirement of using Frame_PatternX variables
     * without bloating the source code.
     */
    System_Init_Patterns();

    /* 3. Create a helper list for iteration */
    void *Frame_List[10] = {
        Frame_Pattern0, Frame_Pattern1, Frame_Pattern2, Frame_Pattern3,
        Frame_Pattern4, Frame_Pattern5, Frame_Pattern6, Frame_Pattern7,
        Frame_Pattern8, Frame_Pattern9
    };

    /* 4. Main Loop */
    while(1) {
        // Countdown loop: 9 -> 0
        for (int i = 9; i >= 0; i--) {
            
            // Send the current frame to the LED Driver
            SPI_Send_Frame((unsigned int (*)[MATRIX_WIDTH])Frame_List[i]);
            
            // Hold the frame for 1 second
            Delay_ms(1000);
        }
    }
    
    return 0;
}
