# 多媒體系統期末專題 – Encoder / Decoder

## 一、專案說明
本專案實作影像的 **Mode 0 編碼與解碼系統**，
將 24-bit BMP 圖片分離為 R/G/B 三個 channel，
並可透過 decoder 還原成與原圖 **完全相同** 的 BMP 檔案。

---

## 二、檔案說明

- `encoder.c`：影像編碼程式
- `decoder.c`：影像解碼程式
- `R.txt`：R channel 資料（ASCII）
- `G.txt`：G channel 資料（ASCII）
- `B.txt`：B channel 資料（ASCII）
- `dim.txt`：影像寬高資訊
- `Kimberly.bmp`：原始圖片
- `ResKimberly.bmp`：解碼後圖片

---

## 三、編譯方式

gcc encoder.c -o encoder
gcc decoder.c -o decoder
