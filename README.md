MMSP 2025 JPEG-like Project
專案介紹

本專案實作了一個簡化版 JPEG-like 影像編碼與解碼流程，包括：

Mode 0：將 BMP 拆成 R/G/B 三個通道，生成 ASCII 檔案；再由 decoder 重建 BMP。

完全可跑通，重建 BMP 與原圖一模一樣。

Mode 1~3：為課程預留功能，尚未完全實作。

檔案結構
final-project/
│
├─ encoder.c        # Mode 0 可用 Encoder
├─ decoder.c        # Mode 0 可用 Decoder
├─ README.md        # 專案說明
├─ Kimberly.bmp     # 測試圖檔
├─ R.txt G.txt B.txt dim.txt   # Mode 0 encoder 輸出
└─ ResKimberly.bmp  # Mode 0 decoder 輸出

編譯方法
Linux / WSL
gcc encoder.c -o encoder
gcc decoder.c -o decoder

Windows (MinGW / PowerShell)
gcc encoder.c -o encoder.exe
gcc decoder.c -o decoder.exe

Mode 0 使用方法
Encoder
./encoder 0 Kimberly.bmp R.txt G.txt B.txt dim.txt


Kimberly.bmp：原始 BMP

R.txt、G.txt、B.txt：輸出的 R/G/B 通道 ASCII 檔

dim.txt：輸出的 BMP 尺寸檔

Decoder
./decoder 0 ResKimberly.bmp R.txt G.txt B.txt dim.txt


ResKimberly.bmp：由 R/G/B.txt + dim.txt 重建的 BMP

完成後，ResKimberly.bmp 與原始 Kimberly.bmp 完全相同