# MMSP JPEG-like Project

## 一、系統實作進度

這個專案希望我們實作一個JPEG編碼與解碼系統，使用C語言完成。系統支援**四種模式**，分別為Mode 0,Mode 1,Mode 2,Mode 3，而此次專題實作我只製作了Mode 0的部分，剩下的Mode皆只有理論部分。

### 1. 系統流程

BMP Image (24-bit)  
│  
▼  
Encoder  
│  
├─ Mode 0 → R.txt / G.txt / B.txt / dim.txt  
├─ Mode 1 → Y.txt / Cb.txt / Cr.txt / dim.txt  
├─ Mode 2 → coeff.txt / dim.txt  
└─ Mode 3 → bitstream.bin / dim.txt  
│  
▼  
Decoder  
│  
├─ Mode 0 → ResBMP (由 R/G/B.txt + dim.txt 重建)  
├─ Mode 1 → ResBMP (由 Y/Cb/Cr.txt 重建)  
├─ Mode 2 → BMP (placeholder)  
└─ Mode 3 → BMP (placeholder)  

### 2. 工作日誌（文字版）

- **2025/12/26**  
  - 完成 Mode 0 的 encoder / decoder，能正確生成 R/G/B.txt 與重建 BMP。  

- **2025/12/27**  
  - 修正 BMP 上下翻轉問題，確保解碼後圖像方向正確。  

- **2025/12/27**  
  - 撰寫 README，補充心得與系統流程說明。  

## 二、心得與感想

在這次專案中，我學到以下幾點：

1. **影像處理流程的理解**：從RGB→YCbCr，再量化、解碼回來的每個步驟都需要處理。
2. **Debug workflow**：GitHub 幫助我自動檢查 Mode 0 的 encoder/decoder，讓我可以知道是哪個環節出錯了。
3. **軟體工程觀念**：Makefile、yml、artifact 的使用讓專案可維護性大大提高。
4. **cmp糾錯**：直接在terminal使用cmp可以知道兩個檔案的首個不同點在哪裡。

---

## 三、GitHub Workflow 與 Artifact

### 1. Workflow 說明

- Workflow 檔案路徑：`.github/workflows/run.yml`  
- 功能：
  1. 編譯 encoder / decoder  
  2. 下載測試圖檔 `Kimberly.bmp`  
  3. 執行 Mode 0 encoder → decoder  
  4. 產生 artifact

### 2. Artifact 設定

```yaml
- name: Upload Mode0 artifact
  uses: actions/upload-artifact@v3
  with:
    name: mode0-artifact
    path: | 
      R.txt
      G.txt
      B.txt
      dim.txt
      ResKimberly.bmp