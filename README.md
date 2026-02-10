# MCU LED Matrix 9 to 0 Countdown Firmware

## 1. 專案概述

本專案旨在設計一套 MCU 韌體，透過 SPI 介面驅動 48x32 LED 點陣矩陣，實現數字 9 至 0 的高解析度倒數動畫。

程式碼設計重點在於遵守題目變數定義（`unsigned int` 陣列）的前提下，透過演算法優化記憶體使用與顯示效果，解決傳統查表法造成的程式碼冗長問題。


## 2. 題目挑戰與設計概念

### 挑戰 A：程式碼冗長與維護性

題目要求定義 10 組 `unsigned int Frame_Pattern[32][48]` 陣列。若採用傳統 Hard-coding 的方式，直接填入所有像素亮度值：

* 程式碼行數過多：需定義約 15,360 個數值，導致原始碼難以閱讀與維護。
* Flash 空間浪費：若儲存為常數，將消耗大量 Flash 空間。

### 挑戰 B：顯示品質

若僅儲存極小的 6x8 字型並進行 4 倍放大：

* 馬賽克效應：像素顆粒過大，數字邊緣呈鋸齒狀。
* 視覺變形：單純放大無法呈現圓潤的字體筆畫。


## 3. 核心解決方案

本專案採用「執行時動態生成 (Runtime Generation)」 與 「高解析度位元字庫 (High-Res Bitmap Font)」的混合策略。

### 策略一：高解析度點陣字庫壓縮

* 設計：捨棄粗糙的 6x8 字型，改為內建 16x24 高解析度字型。
* 儲存：利用 `uint16_t` (16-bit) 儲存一行像素資訊 (Bit-mapping)。
* 效益：
* 每個數字僅佔 48 Bytes (24 rows * 2 bytes)。
* 加起來 10 個數字的總字庫大小僅 480 Bytes，極度節省空間。
* 顯示效果細緻，線條平滑且比例完美。

### 策略二：符合題目規範的動態展開

* 合規性：程式碼中明確宣告題目要求的 10 組全域變數 `Frame_Pattern0` ~ `Frame_Pattern9`，並初始化為 `{0}`。
* 初始化邏輯：
    1. 系統啟動時，呼叫 `System_Init_Patterns()`。
    2. 程式自動讀取壓縮的 16x24 字庫。
    3. 計算置中偏移量(Offset)，將位元資料解壓縮並填入 `Frame_Pattern` 陣列中。
    4. 自動轉換為題目要求的 `unsigned int` 亮度格式 (0/255)。

### 策略三：標準 SPI 驅動協定

實作標準的串列傳輸邏輯，確保畫面穩定不閃爍：

1. Latch (CS) Control：傳輸前拉低，傳輸後拉高，鎖存數據以防止掃描殘影。
2. Data Transmission：依序傳送 32x48 個像素點的亮度資訊。


## 4. 系統架構圖 (Data Flow)

+-----------------------------+
|          壓縮字庫           |
|        Font_16x24           |
|         < 0.5KB             |
+-----------------------------+
              |
              |  系統初始化
              |  解壓縮 & 置中
              V
+-----------------------------+
|         全域緩衝區          |
|      Frame_Pattern0~9       |
|       (題目要求格式)        |
+-----------------------------+
              |
              |  Main Loop
              |  倒數邏輯
              V
+-----------------------------+
|         SPI 驅動層          |
|       SPI_Send_Frame        |
+-----------------------------+
              |
              |  MOSI / CLK / CS
              V
+-----------------------------+
|        LED Driver IC        |
+-----------------------------+
              |
              |  顯示
              V
+-----------------------------+
|      48x32 LED Matrix       |
+-----------------------------+

## 5. 檔案結構說明

led_matrix.h (標頭檔)   ：定義矩陣尺寸、字型參數、全域變數宣告與函式原型。
led_matrix.c (核心實作) ：包含 16x24 字庫數據、解壓縮演算法 (Load_Pattern) 以及 SPI 底層驅動邏輯。
main.c (主程式)         ：系統初始化流程與 9 至 0 的倒數迴圈控制。

## 6. SPI 硬體介面定義

* SPI Interface:
    * `SCK`: Serial Clock
    * `MOSI`: Master Out Slave In (Data)
    * `CS/LATCH`: Chip Select (Frame Synchronization)

* 亮度控制:
* `LED_ON` : 255 (Max Brightness)
* `LED_OFF`: 0

## 7. 擴充性

若需調整字體大小或更換顯示圖案，僅需修改 `led_matrix.c` 中的 `Font_16x24` 陣列數據，無需更動底層驅動邏輯，具備高度的可維護性。





#################################################################
######################## Englisg Version ########################
#################################################################

# MCU LED Matrix 9-to-0 Countdown Firmware

## 1. Project Overview

This project aims to design MCU firmware that drives a 48x32 LED dot matrix via an SPI interface, realizing a high-resolution countdown animation from 9 to 0.

The key focus of the code design is to comply with the required variable definition (using `unsigned int` arrays) while optimizing memory usage and display quality through algorithmic approaches. This solves the issue of code verbosity caused by traditional look-up table methods.

## 2. Challenges & Design Concepts

### Challenge A: Code Verbosity and Maintainability

The problem requires defining 10 sets of `unsigned int Frame_Pattern[32][48]` arrays. If a traditional **Hard-coding** method is adopted to directly fill in all pixel brightness values:

* Excessive Code Length: It would require defining approximately 15,360 values, making the source code difficult to read and maintain.
* Flash Memory Waste: Storing these as constants would consume a significant amount of Flash memory.

### Challenge B: Display Quality

If only a minimal 6x8 font is stored and then scaled up by 4x:

* Mosaic Effect: The pixel blocks become too large, resulting in jagged edges on the numbers.
* Visual Distortion: Simple upscaling cannot render the rounded strokes of the fonts effectively.

## 3. Core Solutions

This project adopts a hybrid strategy of "Runtime Generation" and "High-Res Bitmap Font."

### Strategy 1: High-Resolution Bitmap Font Compression

* Design: Discard the coarse 6x8 font in favor of a built-in 16x24 high-resolution font.
* Storage: Use `uint16_t` (16-bit) to store pixel information for a single row (Bit-mapping).
* Benefits:
* Each digit occupies only 48 Bytes (24 rows * 2 bytes).
* The total size for all 10 digits is only 480 Bytes, maximizing space efficiency.
* The display effect is detailed, with smooth lines and perfect proportions.



### Strategy 2: Dynamic Expansion Compliant with Specifications

* Compliance: The code explicitly declares the 10 global variables `Frame_Pattern0` ~ `Frame_Pattern9` as required by the prompt, initializing them to `{0}`.
* Initialization Logic:
    1. On system startup, `System_Init_Patterns()` is called.
    2. The program reads the compressed 16x24 font data.
    3. It calculates the centering offset, decompresses the bit data, and fills it into the `Frame_Pattern` arrays.
    4. It automatically converts the data into the required `unsigned int` brightness format (0/255).



### Strategy 3: Standard SPI Driver Protocol

Implements standard serial transmission logic to ensure a stable, flicker-free display:

1. Latch (CS) Control: Pull Low before transmission, Pull High after transmission to latch data and prevent scanning artifacts (ghosting).
2. Data Transmission: Sequentially transmit brightness information for 32x48 pixels.

## 4. System Architecture (Data Flow)

+-----------------------------+
|       Compressed Font       |
|         Font_16x24          |
|          < 0.5KB            |
+-----------------------------+
              |
              |  System Init
              |  Decompress & Center
              V
+-----------------------------+
|        Global Buffer        |
|      Frame_Pattern0~9       |
| (Format Required by Prompt) |
+-----------------------------+
              |
              |  Main Loop
              |  Countdown Logic
              V
+-----------------------------+
|       SPI Driver Layer      |
|       SPI_Send_Frame        |
+-----------------------------+
              |
              |  MOSI / CLK / CS
              V
+-----------------------------+
|        LED Driver IC        |
+-----------------------------+
              |
              |  Display
              V
+-----------------------------+
|      48x32 LED Matrix       |
+-----------------------------+


## 5. File Structure Description

* led_matrix.h (Header): Defines matrix dimensions, font parameters, global variable declarations, and function prototypes.
* led_matrix.c (Core Implementation): Contains 16x24 font data, the decompression algorithm (`Load_Pattern`), and low-level SPI driver logic.
* main.c (Main Program): Handles the system initialization process and the 9-to-0 countdown loop control.

## 6. SPI Hardware Interface Definition

* SPI Interface:
* `SCK`: Serial Clock
* `MOSI`: Master Out Slave In (Data)
* `CS/LATCH`: Chip Select (Frame Synchronization)


* Brightness Control:
* `LED_ON`: 255 (Max Brightness)
* `LED_OFF`: 0



## 7. Scalability

If font size adjustments or pattern changes are required, only the `Font_16x24` array data in `led_matrix.c` needs to be modified. The low-level driver logic remains untouched, ensuring high maintainability.