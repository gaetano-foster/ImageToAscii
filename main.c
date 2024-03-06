#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define B		0x42
#define M		0x4D

struct BMP_info_header
{
	union {		// Two bytes- should be B (0x42) and M (0x4D)
		struct {
			BYTE nb;
			BYTE nm;
		};
		WORD name; 
	};
	DWORD size;
	INT32 junk;
	DWORD image_offset;
};

struct BMP_header
{
	DWORD size;
	INT32 width;
	INT32 height;
	union {
		struct {
			WORD color_planes_count;
			WORD bit_depth;
		};
		DWORD junk;
	};
	DWORD compression;
	DWORD image_size;
};

typedef struct pixel_t
{
	BYTE b;
	BYTE g;
	BYTE r;
} pixel_t;

typedef struct image_t
{
	INT32 width;
	INT32 height;
	pixel_t *pixels;
} image_t;

typedef struct ascii_t // ascii image
{
	INT32 width;
	INT32 height;
	char *pixels;
} ascii_t;

BOOL
LoadBMP(image_t *image,
		const char *filepath)
{
	BOOL return_value = TRUE;
	FILE *file = fopen(filepath, "rb");

	if (!file) {
		fprintf(stderr, "Error: Could not open file:\n%s\n", filepath);
		return_value = FALSE;
	}
	else {
		struct BMP_info_header header;
		struct BMP_header dib_header;
		pixel_t *pixels;

		printf("File %s opened.\n", filepath);
		fread(&header.name, 2, 1, file); // because of automatic memory alignment of structs, we need to read to name separately.
		if (header.nb != B
		||  header.nm != M) // all bitmap files must start with BM; if they don't, the file is either corrupted or not a bitmap
		{
			fprintf(stderr, "Error: Invalid or corrupted file format.\n");
			return_value = FALSE;
		}
		else {
			fread(&header.size, 3 * sizeof(DWORD), 1, file); // read into the rest of the struct, starting at the pointer to size
			fread(&dib_header, sizeof(struct BMP_header), 1, file);

			if (dib_header.size != 124
			||  dib_header.compression != 0
			||  dib_header.bit_depth != 24) // we are working only with non-compressed 24 bit bitmaps
			{
				fprintf(stderr, "Error: Invalid or corrupted file format.\nSize: %d\nCompression: %d\nBit Depth: %d\n", dib_header.size, dib_header.compression, dib_header.bit_depth);
				return_value = FALSE;
			}
			else { // image metadata correct
				DWORD pixel_count = dib_header.width * dib_header.height;
				DWORD byte_depth = dib_header.bit_depth / 8;
				pixels = malloc(pixel_count * byte_depth);

				if (pixels) {
					fseek(file, header.image_offset, SEEK_SET);
					int pixels_read = fread(pixels, byte_depth, pixel_count, file);
					if (pixels_read != pixel_count) { // something is wrong
						fprintf(stderr, "Error: Pixel count incorrect. File is likely corrupted.\n");
						free(pixels);
						return_value = FALSE;
					}
					else {
						image->width = dib_header.width;
						image->height = dib_header.height;
						image->pixels = pixels;
						printf("Successfully read file %s\n", filepath);
					}
				}
				else {
					fprintf(stderr, "Error: Failed to allocate %d pixels.\n", pixel_count);
					return_value = FALSE;
				}
			}
		}

		fclose(file);
	}

	return return_value;
}

char 
PixelToAscii(pixel_t pixel) 
{
	const int MAX_VALUE = 765; // 255 + 255 + 255
	const char *pallete = " `.-':_,^=;>+!rc/z?sLTv)J7(|F{CfI31tlu[neoZ5Yxjya2ESwkP6h9d4VpOGbUAXHm8RD#$Bg0MNWQ%&@@";
	int value = pixel.r + pixel.g + pixel.b;
	double percent = (double)value / (double)MAX_VALUE;
	
	return pallete[85-(int)(percent * 85)];
}

BOOL
ImageToAscii(image_t image,
			 ascii_t *ascii)
{
	ascii->height = image.height;
	ascii->width = image.width + 1; // for \n
	ascii->pixels = malloc(ascii->height * ascii->width * sizeof(char) + 1);

	if (ascii->pixels) {
		for (int y = 0; y < ascii->height; y++) {
			for (int x = 0; x < ascii->width; x++) {
				if (x < ascii->width - 1)
					ascii->pixels[y*ascii->width+x] = PixelToAscii(image.pixels[(image.height-y)*image.width+x]);
				else 
					ascii->pixels[y*ascii->width+x] = '\n';
			}
		}
		ascii->pixels[ascii->height * ascii->width] = '\0';
		return TRUE;
	}
	else {
		fprintf(stderr, "Error: Failed to allocate characters.\n");
		return FALSE;
	}
}

int
main(int argc,
	 char **argv)
{
	image_t image;
	ascii_t ascii;

	if (argv[1] == NULL) {
		fprintf(stderr, "Error: No file inputted.\n");
		return -1;
	}

	LoadBMP(&image, argv[1]);
	ImageToAscii(image, &ascii);

	FILE *fp;
	fp = fopen("image.txt", "w");

	fprintf(fp, "\n%s\n", ascii.pixels);

	fclose(fp);

	free(image.pixels);
	free(ascii.pixels);
	return 0;
}