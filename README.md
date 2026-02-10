# LED Matrix Countdown Firmware

## 專案概述
本程式實作於 MCU 環境，驅動 48x32 LED 矩陣顯示 9 至 0 倒數動畫。

## 設計策略說明 (針對題目要求與優化)

### 1. 符合題目格式
程式中保留了 `Frame_Pattern0` ~ `Frame_Pattern9` 的全域陣列定義。

### 2. 程式碼長度與顯示品質優化
若直接硬編碼 (Hard-code) 10 個 48x32 的陣列，程式碼將長達數萬行且難以維護，顯示效果若僅做簡單放大亦不佳。
**本實作採用的解決方案：**
- **Runtime Initialization (執行時生成)**: 在系統啟動時 (Main Loop 前)，透過 `Init_All_Patterns()` 函式動態計算像素。
- **Vector Drawing (向量繪圖)**: 採用「7段顯示器模擬演算法」直接在 48x32 解析度上繪製線條，而非單純放大 Bitmap，確保數字線條平滑、不變形且粗細適中。

### 3. 硬體抽象層
- 使用 `unsigned int` 儲存亮度 (0~255)，符合題目要求。
- SPI 傳輸部分模擬標準 MCU 行為。

## 檔案清單
- `led_matrix.h`: 函式原型與參數定義。
- `led_matrix.c`: 包含 10 組 Pattern 陣列定義、繪圖演算法與 SPI 驅動。
