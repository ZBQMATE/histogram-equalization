#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>

//input picture 24bit, without stuffing bits

int equalize() {
	
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	
	FILE *pic = fopen("./h.bmp", "rb");
	FILE *picout = fopen("./processed.bmp", "wb");
	
	fread(&fileheader, sizeof(BITMAPFILEHEADER), 1, pic);
	fread(&infoheader, sizeof(BITMAPINFOHEADER), 1, pic);
	
	int height = infoheader.biHeight;
	int width = infoheader.biWidth;
	int picsize = height * width;
	
	unsigned char img[height*width*4];
	
	for (int i = 0; i < height*width*4; i++) {
		img[i] = 0;
	}
	
	//histogram equalization
	
	//make transfer on blue pixels
	
	int histogram[256];
	
	for (int i = 0; i < 256; i++) {
		histogram[i] = 0;
	}
	
	unsigned char input_img[height*width*3];
	fread(&input_img, height*width*3, 1, pic);
	
	int a = 0;
	//****
	for (int i = 0; i < height*width*3; i = i+3) {
		a = input_img[i];
		histogram[a] = histogram[a] + 1;
	}
	
	//histogram to its propotional percentage
	
	double pct_histogram[256];
	
	for (int i = 0; i < 256; i++) {
		//***
		pct_histogram[i] = (double) histogram[i] / (height*width);
	}
	
	//accumulative histogram percentage
	
	double accu_histogram[256];
	accu_histogram[0] = pct_histogram[0];
	
	for (int i = 1; i < 256; i++) {
		accu_histogram[i] = accu_histogram[i-1] + pct_histogram[i];
	}
	
	//transform histogram
	
	int trs_histogram[256];
	
	for (int i = 0; i < 256; i++) {
		trs_histogram[i] = (int) 256 * accu_histogram[i];
	}
	
	//transfer the image
	
	int b = 0;
	//*****
	for (int i = 0; i < height*width; i = i+1) {
		a = input_img[3*i];
		b = trs_histogram[a];
		img[i*4] = b;
	}
	
	
	//save
	
	infoheader.biBitCount = 32;
	
	fwrite(&fileheader, sizeof(BITMAPFILEHEADER), 1, picout);
	fwrite(&infoheader, sizeof(BITMAPINFOHEADER), 1, picout);
	fwrite(img, picsize, 4, picout);
	
	fclose(pic);
	fclose(picout);
	
	return 0;
}

int main() {
	equalize();
}