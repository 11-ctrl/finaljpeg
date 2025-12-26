#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#pragma pack(push,1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

static inline unsigned char clamp(int v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return (unsigned char)v;
}

void write_bmp(const char *filename, unsigned char *rgb, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Cannot open output bmp");
        exit(1);
    }

    int row_padded = (width * 3 + 3) & (~3);
    int img_size = row_padded * height;

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    bfh.bfType = 0x4D42;
    bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + img_size;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = 0;
    bih.biSizeImage = img_size;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    fwrite(&bfh, sizeof(bfh), 1, fp);
    fwrite(&bih, sizeof(bih), 1, fp);

    unsigned char *row = (unsigned char*)calloc(row_padded, 1);

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            row[x*3 + 0] = rgb[(y*width + x)*3 + 2]; // B
            row[x*3 + 1] = rgb[(y*width + x)*3 + 1]; // G
            row[x*3 + 2] = rgb[(y*width + x)*3 + 0]; // R
        }
        fwrite(row, row_padded, 1, fp);
    }

    free(row);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage error\n");
        return 1;
    }

    int mode = atoi(argv[1]);

    /* ================= Mode 0 ================= */
    if (mode == 0) {
        // decoder 0 Res.bmp R.txt G.txt B.txt dim.txt
        if (argc != 7) {
            printf("Usage: decoder 0 out.bmp R.txt G.txt B.txt dim.txt\n");
            return 1;
        }
        FILE *fR = fopen(argv[3], "r");
        FILE *fG = fopen(argv[4], "r");
        FILE *fB = fopen(argv[5], "r");
        FILE *fd = fopen(argv[6], "r");
        if (!fR || !fG || !fB || !fd) {
            printf("Mode 0: cannot open input files\n");
            return 1;
        }
        fclose(fR); fclose(fG); fclose(fB); fclose(fd);
        printf("Mode 0 done (interface check)\n");
        return 0;
    }

    /* ================= Mode 1 ================= */
    if (mode == 1) {
        // decoder 1 Y.txt Cb.txt Cr.txt out.bmp dim.txt
        if (argc != 7) {
            printf("Usage: decoder 1 Y.txt Cb.txt Cr.txt out.bmp dim.txt\n");
            return 1;
        }

        FILE *fY  = fopen(argv[2], "r");
        FILE *fCb = fopen(argv[3], "r");
        FILE *fCr = fopen(argv[4], "r");
        FILE *fd  = fopen(argv[6], "r");
        if (!fY || !fCb || !fCr || !fd) {
            printf("Cannot open input files\n");
            return 1;
        }

        int width, height;
        fscanf(fd, "%d %d", &width, &height);
        fclose(fd);

        int size = width * height;
        int *Y  = (int*)malloc(sizeof(int) * size);
        int *Cb = (int*)malloc(sizeof(int) * size);
        int *Cr = (int*)malloc(sizeof(int) * size);
        unsigned char *rgb = (unsigned char*)malloc(size * 3);

        for (int i = 0; i < size; i++) fscanf(fY,  "%d", &Y[i]);
        for (int i = 0; i < size; i++) fscanf(fCb, "%d", &Cb[i]);
        for (int i = 0; i < size; i++) fscanf(fCr, "%d", &Cr[i]);

        fclose(fY); fclose(fCb); fclose(fCr);

        for (int i = 0; i < size; i++) {
            double y  = Y[i];
            double cb = Cb[i] - 128.0;
            double cr = Cr[i] - 128.0;

            int r = (int)(y + 1.402 * cr);
            int g = (int)(y - 0.34414 * cb - 0.71414 * cr);
            int b = (int)(y + 1.772 * cb);

            rgb[i*3 + 0] = clamp(r);
            rgb[i*3 + 1] = clamp(g);
            rgb[i*3 + 2] = clamp(b);
        }

        write_bmp(argv[5], rgb, width, height);

        free(Y); free(Cb); free(Cr); free(rgb);
        printf("Mode 1 decode done\n");
        return 0;
    }

    printf("Mode not implemented\n");
    return 0;
}
