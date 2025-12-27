#include <stdio.h>
#include <stdlib.h>

#pragma pack(push,1)
typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int biSize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitCount;
    unsigned int compression;
    unsigned int sizeImage;
    int xppm;
    int yppm;
    unsigned int clrUsed;
    unsigned int clrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

/* 寫 BMP */
void write_bmp(const char *filename,int w,int h,unsigned char *rgb){
    FILE *fp=fopen(filename,"wb");
    if(!fp){ perror("write bmp"); exit(1);}
    int row_padded=(w*3+3)&(~3);
    int img_size=row_padded*h;

    BITMAPFILEHEADER fh={0x4D42,54+img_size,0,0,54};
    BITMAPINFOHEADER ih={40,w,h,1,24,0,img_size,0,0,0,0};

    fwrite(&fh,sizeof(fh),1,fp);
    fwrite(&ih,sizeof(ih),1,fp);

    unsigned char *row=(unsigned char*)calloc(row_padded,1);

    for(int y=h-1;y>=0;y--){
        for(int x=0;x<w;x++){
            int i=(y*w+x)*3;
            row[x*3+2]=rgb[i+0]; // R
            row[x*3+1]=rgb[i+1]; // G
            row[x*3+0]=rgb[i+2]; // B
        }
        fwrite(row,1,row_padded,fp);
    }

    free(row);
    fclose(fp);
}

int main(int argc,char *argv[]){
    // Mode 0 總共有 6 個參數：decoder 0 out.bmp R.txt G.txt B.txt dim.txt
    if(argc != 7){
        fprintf(stderr,"Usage: decoder 0 out.bmp R.txt G.txt B.txt dim.txt\n");
        return 1;
    }

    const char *outbmp = argv[2];
    const char *Rtxt = argv[3];
    const char *Gtxt = argv[4];
    const char *Btxt = argv[5];
    const char *dimtxt = argv[6];

    // 讀取圖片尺寸
    int w,h;
    FILE *fd=fopen(dimtxt,"r");
    if(!fd){ perror("dim.txt"); exit(1);}
    if(fscanf(fd,"%d %d",&w,&h)!=2){
        fprintf(stderr,"Failed to read width and height from dim.txt\n");
        fclose(fd);
        return 1;
    }
    fclose(fd);

    unsigned char *rgb=(unsigned char*)malloc(w*h*3);
    if(!rgb){ perror("malloc"); exit(1); }

    // 讀取 R/G/B.txt
    FILE *fR=fopen(Rtxt,"r");
    FILE *fG=fopen(Gtxt,"r");
    FILE *fB=fopen(Btxt,"r");
    if(!fR || !fG || !fB){ perror("fopen R/G/B.txt"); exit(1); }

    for(int i=0;i<w*h;i++){
        int r,g,b;
        if(fscanf(fR,"%d",&r)!=1 || fscanf(fG,"%d",&g)!=1 || fscanf(fB,"%d",&b)!=1){
            fprintf(stderr,"Failed to read pixel %d\n",i);
            fclose(fR); fclose(fG); fclose(fB);
            free(rgb);
            return 1;
        }
        rgb[i*3+0]=(unsigned char)r;
        rgb[i*3+1]=(unsigned char)g;
        rgb[i*3+2]=(unsigned char)b;
    }

    fclose(fR); fclose(fG); fclose(fB);

    // 寫 BMP
    write_bmp(outbmp,w,h,rgb);
    free(rgb);

    printf("Mode 0 decoding done. Output: %s\n", outbmp);
    return 0;
}
