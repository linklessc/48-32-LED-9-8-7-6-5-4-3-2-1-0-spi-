# LED Matrix Countdown (Bitmap Font Version)

## 題目需求分析與實作策略
本程式依據題目要求，透過 SPI 驅動 48x32 LED 矩陣。針對「定義數字點陣造成 Code 太長」與「顯示品質」的矛盾，採取以下工程解法：

### 1. 核心技術：Bit-Mapped Font (位元對映字庫)
- **避免 Code 太長**：不直接手寫 10 組 `48x32` 的 `unsigned int` 陣列（這會佔用大量 Flash 且難以閱讀）。
- **解決放大變粗問題**：不使用 6x8 低解析度字型硬放大。改為定義一套 **16x24** 的高解析度字庫。
- **儲存效率**：
  - 字寬 16 pixels，剛好可用 `uint16_t` (unsigned short) 的位元來儲存一行。
  - 每個數字僅佔 48 bytes，10 個數字不到 0.5KB。

### 2. 記憶體架構
- **Frame Buffers**: 依題目要求保留 `unsigned int Frame_Pattern0~9` 的全域定義。
- **Initialization**: 系統啟動時，透過 `Load_Pattern()` 函數，將壓縮的 `16x24` 字庫解壓並置中填入 Frame Buffers。

### 3. SPI 驅動
- 模擬標準 SPI 時序 (Latch / Clock / Data)。
