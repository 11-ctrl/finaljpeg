# MMSP JPEG-like Project

## 一、系統實作進度

本專案實作了一個簡化版的 JPEG-like 編碼與解碼系統，使用 C 語言完成。系統支援 **四種模式**，其中 Mode 0 為可完整交作業的版本。

### 1. 系統流程（Block Diagram 文字版）

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

markdown
複製程式碼

### 2. 工作日誌（文字版）

- **2025/12/23**  
  - 完成 Mode 0 的 encoder / decoder，能正確生成 R/G/B.txt 與重建 BMP。  

- **2025/12/24**  
  - 修正 BMP 上下翻轉問題，確保解碼後圖像方向正確。  

- **2025/12/25**  
  - 建立 GitHub Actions workflow，自動跑 Mode 0，並產生 artifact 供評分。  

- **2025/12/26**  
  - 撰寫 README，補充心得與系統流程說明。  

---

## 二、心得與感想

在這次專案中，我學到以下幾點：

1. **影像處理流程的理解**：從 RGB → YCbCr，再量化、解碼回來的每個步驟都需要小心處理。
2. **BMP 格式細節**：BMP 的 row padding 與上下翻轉非常容易出錯。
3. **Debug workflow**：GitHub Actions 幫助我自動檢查 Mode 0 的 encode/decode，讓測試變得穩定且可重現。
4. **軟體工程觀念**：Makefile、yml、artifact 的使用讓專案可維護性大大提高。

總結來說，這次專案不僅練習了影像壓縮理論，也熟悉了完整的自動化流程。

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