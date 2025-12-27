#include <stdio.h>
#include <stdlib.h>

#pragma pack(push,1)
typedef struct {
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int   biSize;
    int   width;
    int   height;
    unsigned short planes;
    unsigned short bitCount;
    unsigned int   compression;
    unsigned int   sizeImage;
    int xppm;
    int yppm;
    unsigned int clrUsed;
    unsigned int clrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

/* =========================================================
 * Mode 0：RGB → R/G/B.txt
 * ========================================================= */
void encode_mode0(const char *bmpfile,
                  const char *Rtxt,
                  const char *Gtxt,
                  const char *Btxt,
                  const char *dimtxt)
{
    FILE *fp = fopen(bmpfile, "rb");
    if (!fp) { perror("open bmp"); exit(1); }

    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    fread(&fh, sizeof(fh), 1, fp);
    fread(&ih, sizeof(ih), 1, fp);

    if (fh.bfType != 0x4D42 || ih.bitCount != 24) {
        fprintf(stderr, "Only support 24-bit BMP\n");
        exit(1);
    }

    int w = ih.width;
    int h = ih.height;

    /* [修正點]：寫 dim.txt 時，保存完整的 Header 資訊
     * 格式: width height bfSize sizeImage xppm yppm clrUsed clrImportant
     */
    FILE *fd = fopen(dimtxt, "w");
    fprintf(fd, "%d %d %u %u %d %d %u %u\n", 
            w, h, 
            fh.bfSize,       // 原始檔案大小
            ih.sizeImage,    // 原始影像數據大小
            ih.xppm,         // 水平解析度
            ih.yppm,         // 垂直解析度
            ih.clrUsed,      // 調色盤使用數
            ih.clrImportant);// 重要顏色數
    fclose(fd);

    FILE *fR = fopen(Rtxt, "w");
    FILE *fG = fopen(Gtxt, "w");
    FILE *fB = fopen(Btxt, "w");

    int row_padded = (w * 3 + 3) & (~3);
    unsigned char *row = (unsigned char*)malloc(row_padded);

    fseek(fp, fh.bfOffBits, SEEK_SET);

    for (int y = h - 1; y >= 0; y--) {
        fread(row, 1, row_padded, fp);
        for (int x = 0; x < w; x++) {
            unsigned char B = row[x*3 + 0];
            unsigned char G = row[x*3 + 1];
            unsigned char R = row[x*3 + 2];
            fprintf(fR, "%d\n", R);
            fprintf(fG, "%d\n", G);
            fprintf(fB, "%d\n", B);
        }
    }

    free(row);
    fclose(fp);
    fclose(fR);
    fclose(fG);
    fclose(fB);

    printf("Mode 0 encoding done.\n");
}

/* =========================================================
 * main
 * ========================================================= */
int main(int argc, char *argv[]) {

    if (argc < 4) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    int mode = atoi(argv[1]);

    switch (mode) {
    case 0:
        if (argc != 7) {
            fprintf(stderr,
              "Usage: encoder 0 input.bmp R.txt G.txt B.txt dim.txt\n");
            return 1;
        }
        encode_mode0(argv[2], argv[3], argv[4], argv[5], argv[6]);
        break;

    case 1:
        printf("Mode 1 placeholder (YCbCr not implemented yet)\n");
        break;

    case 2:
        printf("Mode 2 placeholder (DCT not implemented yet)\n");
        break;

    case 3:
        printf("Mode 3 placeholder (Entropy coding not implemented yet)\n");
        break;

    default:
        fprintf(stderr, "Unknown mode %d\n", mode);
        return 1;
    }

    return 0;
}