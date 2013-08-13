#include "common.h"

/*

DERUS file structure
  Offset Size  Contents
  HEADER
  ======
  0x0000    6  DERUS2
  0x0006    1  Image flags:
              0x01  0=Mode 1 images
                    1=Mode 2 images
              0x02  0=EDC in Mode 2 Form 2 sectors, original image
                    1=No EDC in Mode 2 Form 2 sectors, original image
  0x0005    9  0x00 {9} [reserved for future use]
  0x0010    4  Old image size (sectors)
  0x0014    4  New image size (sectors)
  0x0018   16  Old image MD5
  0x0028   16  New image MD5
  SECTOR
  ======
  0x0000    1 Sector flags:
              0x01  0=Use data from image (add DWORD sector number to the Sector data)
                    1=Use data from derus (add 2048 (Mode 1)/2056 (Mode 2 Form 1)/2332 (Mode 2 Form 2) cooked data bytes in Sector data)
              0x06  0=Copy EDC from image (flags & 0x01 should be 0)
                    1=Copy EDC from derus (add DWORD EDC value to the Sector data)
                    2=Generate EDC
                    3=Zero EDC
              0x18  0=Copy ECC from image (flags & 0x01 should be 0) (always 0 for Form 2 sectors)
                    1=Copy ECC from derus (add 276 ECC bytes to the Sector data)
                    2=Generate ECC
                    3=Zero ECC
  0x0001  var Sector data

*/


/*
void makederus(u32 argc, s8 * argv[]) {
	//Common variables & structures
	FILE * badimage, * goodimage, * derus;
	u8 buffer[2352], buffer2[2336], digest[16], ctrlbyte;
	u32 i, a, brokensectors = 0, badimagesize, badimagesectors, goodimagesize, goodimagesectors;
	struct sectors {
		u32 adress;
		u8 md5[16];
	};
	MD5_CTX context;

	//Opening & creating files
	if (fopen_s(&badimage, argv[2], "rb") != 0) {
		perror(argv[2]);
		return;
	}
	badimagesize = getfilesize(badimage);
	if (badimagesize % 2352 != 0) {
		printf("File \"%s\" is not in Mode2/2352 image!\n", argv[2]);
		fclose(badimage);
		return;
	}
	badimagesectors = badimagesize / 2352;

	if (fopen_s(&goodimage, argv[3], "rb") != 0) {
		perror(argv[3]);
		fclose(badimage);
		return;
	}
	goodimagesize = getfilesize(goodimage);
	if (goodimagesize % 2352 != 0) {
		printf("File \"%s\" is not in Mode2/2352 image!\n", argv[3]);
		fclose(goodimage); fclose(badimage);
		return;
	}
	goodimagesectors = goodimagesize / 2352;

	if (fopen_s(&derus, argv[4], "w+b") != 0) {
		printf("Error creating file \"%s\"!\n", argv[4]);
		fclose(goodimage); fclose(badimage);
		return;
	}

	//Writing header
	fprintf(derus, "DERUS");
	fseek(derus, 11, SEEK_CUR);
	_putw(badimagesectors, derus);
	_putw(goodimagesectors, derus);

	printf("Non-original image:\nFilename: %s\nSize (bytes): %u\nSize (sectors): %u\nCalculating MD5... ", argv[2], badimagesectors * 2352, badimagesectors);
	MD5Init(&context);
	while (fread(buffer, 1, 2352, badimage))
		MD5Update(&context, buffer, 2352);
	MD5Final(digest, &context);
	fseek(badimage, 0, SEEK_SET);
	for (i = 0; i < 16; i++) {
		putc(digest[i], derus);
		printf("%02x", digest[i]);
	}

	printf("\n\nOriginal image:\nFilename: %s\nSize (bytes): %u\nSize (sectors): %u\nCalculating MD5... ", argv[3], goodimagesectors * 2352, goodimagesectors);
	MD5Init(&context);
	while (fread(buffer, 1, 2352, goodimage))
		MD5Update(&context, buffer, 2352);
	MD5Final(digest, &context);
	fseek(goodimage, 0, SEEK_SET);
	for (i = 0; i < 16; i++) {
		putc(digest[i], derus);
		printf("%02x", digest[i]);
	}

	printf("\n\nDerus filename: %s\n", argv[4]);

	//Allocating space for original image checksums, calculating checksums
	printf("Calculating original image checksums...");
	u8 * goodimagechecksums = (u8*)calloc(goodimagesectors * 16, 1);
	for (i = 0; i < goodimagesectors; i++) {
		MD5Init(&context);
		fseek(goodimage, 16, SEEK_CUR);
		fread(buffer, 1, 2336, goodimage);
		MD5Update(&context, buffer, 2336);
		MD5Final(&*(goodimagechecksums + i * 16), &context);
	}
	printf(" Done!\n");

	//Allocating space for non-original image checksums, calculating checksums
	printf("Calculating non-original image checksums...");
	sectors *sectors1  = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors2  = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors3  = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors4  = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors5  = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors6  = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors7  = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors8  = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors9  = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors10 = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors11 = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors12 = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors13 = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors14 = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors15 = (sectors*)calloc(badimagesectors *40, 1);
	sectors *sectors16 = (sectors*)calloc(badimagesectors *40, 1);

	u32 i1 = 0,  i2 = 0,  i3 = 0,  i4 = 0,
		i5 = 0,  i6 = 0,  i7 = 0,  i8 = 0,
		i9 = 0,  i10 = 0, i11 = 0, i12 = 0,
		i13 = 0, i14 = 0, i15 = 0, i16 = 0;

	for (i = 0; i < badimagesectors; i++) {
		MD5Init(&context);
		fseek(badimage, 18, SEEK_CUR);
		fread(&ctrlbyte, 1, 1, badimage);
		fseek(badimage, -3, SEEK_CUR);
		fread(buffer, 1, 2336, badimage);
		if (!goodimageedc && ctrlbyte >> 5 & 0x1) {
			buffer[2332] = 0x00;
			buffer[2333] = 0x00;
			buffer[2334] = 0x00;
			buffer[2335] = 0x00;
		}
		MD5Update (&context, buffer, 2336);
		MD5Final(digest, &context);

			 if(digest[0] < 0x10)  {for (a = 0; a < 16; a++)   sectors1[i1].md5[a] = digest[a];   sectors1[i1].adress = i; i1++;}
		else if(digest[0] < 0x20)  {for (a = 0; a < 16; a++)   sectors2[i2].md5[a] = digest[a];   sectors2[i2].adress = i; i2++;}
		else if(digest[0] < 0x30)  {for (a = 0; a < 16; a++)   sectors3[i3].md5[a] = digest[a];   sectors3[i3].adress = i; i3++;}
		else if(digest[0] < 0x40)  {for (a = 0; a < 16; a++)   sectors4[i4].md5[a] = digest[a];   sectors4[i4].adress = i; i4++;}
		else if(digest[0] < 0x50)  {for (a = 0; a < 16; a++)   sectors5[i5].md5[a] = digest[a];   sectors5[i5].adress = i; i5++;}
		else if(digest[0] < 0x60)  {for (a = 0; a < 16; a++)   sectors6[i6].md5[a] = digest[a];   sectors6[i6].adress = i; i6++;}
		else if(digest[0] < 0x70)  {for (a = 0; a < 16; a++)   sectors7[i7].md5[a] = digest[a];   sectors7[i7].adress = i; i7++;}
		else if(digest[0] < 0x80)  {for (a = 0; a < 16; a++)   sectors8[i8].md5[a] = digest[a];   sectors8[i8].adress = i; i8++;}
		else if(digest[0] < 0x90)  {for (a = 0; a < 16; a++)   sectors9[i9].md5[a] = digest[a];   sectors9[i9].adress = i; i9++;}
		else if(digest[0] < 0xA0)  {for (a = 0; a < 16; a++) sectors10[i10].md5[a] = digest[a]; sectors10[i10].adress = i; i10++;}
		else if(digest[0] < 0xB0)  {for (a = 0; a < 16; a++) sectors11[i11].md5[a] = digest[a]; sectors11[i11].adress = i; i11++;}
		else if(digest[0] < 0xC0)  {for (a = 0; a < 16; a++) sectors12[i12].md5[a] = digest[a]; sectors12[i12].adress = i; i12++;}
		else if(digest[0] < 0xD0)  {for (a = 0; a < 16; a++) sectors13[i13].md5[a] = digest[a]; sectors13[i13].adress = i; i13++;}
		else if(digest[0] < 0xE0)  {for (a = 0; a < 16; a++) sectors14[i14].md5[a] = digest[a]; sectors14[i14].adress = i; i14++;}
		else if(digest[0] < 0xF0)  {for (a = 0; a < 16; a++) sectors15[i15].md5[a] = digest[a]; sectors15[i15].adress = i; i15++;}
		else					   {for (a = 0; a < 16; a++) sectors16[i16].md5[a] = digest[a]; sectors16[i16].adress = i; i16++;}
	}
	printf(" Done!\n");

	//Comparing checksums
	printf("Comparing checksums...");

	for (i = 0; i < goodimagesectors; i++) {

		if (*(goodimagechecksums + i * 16) < 0x10) {
			for (a = 0; a < i1; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors1[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors1[a].adress, derus);
					break;
				} else if (a + 1 == i1) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0x20) {
			for (a = 0; a < i2; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors2[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors2[a].adress, derus);
					break;
				} else if (a + 1 == i2) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0x30) {
			for (a = 0; a < i3; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors3[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors3[a].adress, derus);
					break;
				} else if (a + 1 == i3) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0x40) {
			for (a = 0; a < i4; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors4[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors4[a].adress, derus);
					break;
				} else if (a + 1 == i4) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0x50) {
			for (a = 0; a < i5; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors5[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors5[a].adress, derus);
					break;
				} else if (a + 1 == i5) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0x60) {
			for (a = 0; a < i6; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors6[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors6[a].adress, derus);
					break;
				} else if (a + 1 == i6) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0x70) {
			for (a = 0; a < i7; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors7[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors7[a].adress, derus);
					break;
				} else if (a + 1 == i7) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0x80) {
			for (a = 0; a < i8; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors8[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors8[a].adress, derus);
					break;
				} else if (a + 1 == i8) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0x90) {
			for (a = 0; a < i9; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors9[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors9[a].adress, derus);
					break;
				} else if (a + 1 == i9) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0xA0) {
			for (a = 0; a < i10; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors10[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors10[a].adress, derus);
					break;
				} else if (a + 1 == i10) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0xB0) {
			for (a = 0; a < i11; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors11[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors11[a].adress, derus);
					break;
				} else if (a + 1 == i11) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0xC0) {
			for (a = 0; a < i12; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors12[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors12[a].adress, derus);
					break;
				} else if (a + 1 == i12) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0xD0) {
			for (a = 0; a < i13; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors13[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors13[a].adress, derus);
					break;
				} else if (a + 1 == i13) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0xE0) {
			for (a = 0; a < i14; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors14[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors14[a].adress, derus);
					break;
				} else if (a + 1 == i14) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else if (*(goodimagechecksums + i * 16) < 0xF0) {
			for (a = 0; a < i15; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors15[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors15[a].adress, derus);
					break;
				} else if (a + 1 == i15) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}

		else {
			for (a = 0; a < i16; a++) {
				if (buffercmp(goodimagechecksums + i * 16, sectors16[a].md5, 16)) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(sectors16[a].adress, derus);
					break;
				} else if (a + 1 == i16) {
					fseek(derus, 56 + i * 4, SEEK_SET);
					_putw(0xFFFFFFFF, derus);
					fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
					fseek(goodimage, i * 2352 + 16, SEEK_SET);
					fread(buffer, 1, 2336, goodimage);
					fwrite(buffer, 1, 2336, derus);
					brokensectors++;
				}
			}
		}
	}
	free(sectors1);
	free(sectors2);
	free(sectors3);
	free(sectors4);
	free(sectors5);
	free(sectors6);
	free(sectors7);
	free(sectors8);
	free(sectors9);
	free(sectors10);
	free(sectors11);
	free(sectors12);
	free(sectors13);
	free(sectors14);
	free(sectors15);
	free(sectors16);
	free(goodimagechecksums);
	printf(" Done!\nNumber of broken sectors: %u\n\n", brokensectors);
	printf("Patch is ready to use.");
	return;
}

*/

void makederus(u32 argc, s8 * argv[]) {
	//Common variables & structures
	FILE * badimage, * goodimage, * derus;
	u8 buffer[2352], buffer2[2336], digest[16], ctrlbyte, badimageedc, goodimageedc, rebuild = 1;
	u32 i, a, brokensectors = 0;
	struct sectors {
		u32 adress;
		u8 md5[16];
	};
	MD5_CTX context;

	//Opening & creating files
	if (fopen_s(&badimage, argv[2], "rb") != 0) {
		printf("Error opening file \"%s\" for reading!\n", argv[2]);
		return;
	}
	u32 badimagesize = getfilesize(badimage);
	if (badimagesize % 2352 != 0) {
		printf("File \"%s\" is not in Mode2/2352 image!\n", argv[2]);
		fclose(badimage);
		return;
	}
	u32 badimagesectors = badimagesize / 2352;
	badimageedc = getedc(badimage);

	if (fopen_s(&goodimage, argv[3], "rb") != 0) {
		printf("Error opening file \"%s\" for reading!\n", argv[3]);
		fclose(badimage);
		return;
	}
	u32 goodimagesize = getfilesize(goodimage);
	if (goodimagesize % 2352 != 0) {
		printf("File \"%s\" is not in Mode2/2352 image!\n", argv[3]);
		fclose(goodimage); fclose(badimage);
		return;
	}
	u32 goodimagesectors = goodimagesize / 2352;
	goodimageedc = getedc(goodimage);

	if (fopen_s(&derus, argv[4], "w+b") != 0) {
		printf("Error creating file \"%s\"!\n", argv[4]);
		fclose(goodimage); fclose(badimage);
		return;
	}

	//Derus.drs
	fprintf(derus, "DERUS");
	putc(rebuild, derus);
	putc(badimageedc, derus);
	putc(goodimageedc, derus);
	fseek(derus, 8, SEEK_CUR);
	_putw(badimagesectors, derus);
	_putw(goodimagesectors, derus);

	printf("Non-original image:\nFilename: %s\nSize (bytes): %u\nSize (sectors): %u\nEDC in Form 2 sectors: ", argv[2], badimagesectors * 2352, badimagesectors);
	if (badimageedc) printf("YES\n"); else printf("NO\n");
	printf("Calculating MD5...");
	MD5Init(&context);
	while (fread(buffer, 1, 2352, badimage)) MD5Update(&context, buffer, 2352);
	MD5Final(digest, &context);
	fseek(badimage, 0, SEEK_SET);
	printf(" ");
	for (int i = 0; i < 16; i++) {
		putc(digest[i], derus);
		printf("%02x", digest[i]);
	}

	printf("\n\nOriginal image:\nFilename: %s\nSize (bytes): %u\nSize (sectors): %u\nEDC in Form 2 sectors: ", argv[3], goodimagesectors * 2352, goodimagesectors);
	if (goodimageedc) printf("YES\n"); else printf("NO\n");
	printf("Calculating MD5...");
	MD5Init(&context);
	while (fread(buffer, 1, 2352, goodimage)) MD5Update(&context, buffer, 2352);
	MD5Final(digest, &context);
	fseek(goodimage, 0, SEEK_SET);
	printf(" ");
	for (i = 0; i < 16; i++) {
		putc(digest[i], derus);
		printf("%02x", digest[i]);
	}

	printf("\n\nDerus filename: %s\n", argv[4]);

	if (rebuild) {
		printf("Derus mode: Rebuild\n\n");
		//Allocating space for original image checksums, calculating checksums
		printf("Calculating original image checksums...");
		u8 * goodimagechecksums = (u8*)calloc(goodimagesectors * 16, 1);
		for (i = 0; i < goodimagesectors; i++) {
			MD5Init(&context);
			fseek(goodimage, 16, SEEK_CUR);
			fread(buffer, 1, 2336, goodimage);
			MD5Update(&context, buffer, 2336);
			MD5Final(&*(goodimagechecksums + i * 16), &context);
		}
		printf(" Done!\n");

		//Allocating space for non-original image checksums, calculating checksums
		printf("Calculating non-original image checksums...");
		sectors *sectors1  = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors2  = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors3  = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors4  = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors5  = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors6  = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors7  = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors8  = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors9  = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors10 = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors11 = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors12 = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors13 = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors14 = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors15 = (sectors*)calloc(badimagesectors *40, 1);
		sectors *sectors16 = (sectors*)calloc(badimagesectors *40, 1);

		u32 i1 = 0,  i2 = 0,  i3 = 0,  i4 = 0,
			i5 = 0,  i6 = 0,  i7 = 0,  i8 = 0,
			i9 = 0,  i10 = 0, i11 = 0, i12 = 0,
			i13 = 0, i14 = 0, i15 = 0, i16 = 0;

		for (i = 0; i < badimagesectors; i++) {
			MD5Init(&context);
			fseek(badimage, 18, SEEK_CUR);
			fread(&ctrlbyte, 1, 1, badimage);
			fseek(badimage, -3, SEEK_CUR);
			fread(buffer, 1, 2336, badimage);
			if (!goodimageedc && ctrlbyte >> 5 & 0x1) {
				buffer[2332] = 0x00;
				buffer[2333] = 0x00;
				buffer[2334] = 0x00;
				buffer[2335] = 0x00;
			}
			MD5Update (&context, buffer, 2336);
			MD5Final(digest, &context);

				 if(digest[0] < 0x10)  {for (a = 0; a < 16; a++) sectors1[i1].md5[a] = digest[a]; sectors1[i1].adress = i; i1++;}
			else if(digest[0] < 0x20)  {for (a = 0; a < 16; a++) sectors2[i2].md5[a] = digest[a]; sectors2[i2].adress = i; i2++;}
			else if(digest[0] < 0x30)  {for (a = 0; a < 16; a++) sectors3[i3].md5[a] = digest[a]; sectors3[i3].adress = i; i3++;}
			else if(digest[0] < 0x40)  {for (a = 0; a < 16; a++) sectors4[i4].md5[a] = digest[a]; sectors4[i4].adress = i; i4++;}
			else if(digest[0] < 0x50)  {for (a = 0; a < 16; a++) sectors5[i5].md5[a] = digest[a]; sectors5[i5].adress = i; i5++;}
			else if(digest[0] < 0x60)  {for (a = 0; a < 16; a++) sectors6[i6].md5[a] = digest[a]; sectors6[i6].adress = i; i6++;}
			else if(digest[0] < 0x70)  {for (a = 0; a < 16; a++) sectors7[i7].md5[a] = digest[a]; sectors7[i7].adress = i; i7++;}
			else if(digest[0] < 0x80)  {for (a = 0; a < 16; a++) sectors8[i8].md5[a] = digest[a]; sectors8[i8].adress = i; i8++;}
			else if(digest[0] < 0x90)  {for (a = 0; a < 16; a++) sectors9[i9].md5[a] = digest[a]; sectors9[i9].adress = i; i9++;}
			else if(digest[0] < 0xA0)  {for (a = 0; a < 16; a++) sectors10[i10].md5[a] = digest[a]; sectors10[i10].adress = i; i10++;}
			else if(digest[0] < 0xB0)  {for (a = 0; a < 16; a++) sectors11[i11].md5[a] = digest[a]; sectors11[i11].adress = i; i11++;}
			else if(digest[0] < 0xC0)  {for (a = 0; a < 16; a++) sectors12[i12].md5[a] = digest[a]; sectors12[i12].adress = i; i12++;}
			else if(digest[0] < 0xD0)  {for (a = 0; a < 16; a++) sectors13[i13].md5[a] = digest[a]; sectors13[i13].adress = i; i13++;}
			else if(digest[0] < 0xE0)  {for (a = 0; a < 16; a++) sectors14[i14].md5[a] = digest[a]; sectors14[i14].adress = i; i14++;}
			else if(digest[0] < 0xF0)  {for (a = 0; a < 16; a++) sectors15[i15].md5[a] = digest[a]; sectors15[i15].adress = i; i15++;}
			else					   {for (a = 0; a < 16; a++) sectors16[i16].md5[a] = digest[a]; sectors16[i16].adress = i; i16++;}
		}
		printf(" Done!\n");

		//Comparing checksums
		printf("Comparing checksums...");

		for (i = 0; i < goodimagesectors; i++) {

			if (*(goodimagechecksums + i * 16) < 0x10) {
				for (a = 0; a < i1; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors1[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors1[a].adress, derus);
						break;
					} else if (a + 1 == i1) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0x20) {
				for (a = 0; a < i2; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors2[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors2[a].adress, derus);
						break;
					} else if (a + 1 == i2) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0x30) {
				for (a = 0; a < i3; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors3[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors3[a].adress, derus);
						break;
					} else if (a + 1 == i3) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0x40) {
				for (a = 0; a < i4; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors4[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors4[a].adress, derus);
						break;
					} else if (a + 1 == i4) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0x50) {
				for (a = 0; a < i5; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors5[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors5[a].adress, derus);
						break;
					} else if (a + 1 == i5) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0x60) {
				for (a = 0; a < i6; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors6[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors6[a].adress, derus);
						break;
					} else if (a + 1 == i6) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0x70) {
				for (a = 0; a < i7; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors7[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors7[a].adress, derus);
						break;
					} else if (a + 1 == i7) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0x80) {
				for (a = 0; a < i8; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors8[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors8[a].adress, derus);
						break;
					} else if (a + 1 == i8) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0x90) {
				for (a = 0; a < i9; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors9[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors9[a].adress, derus);
						break;
					} else if (a + 1 == i9) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0xA0) {
				for (a = 0; a < i10; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors10[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors10[a].adress, derus);
						break;
					} else if (a + 1 == i10) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0xB0) {
				for (a = 0; a < i11; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors11[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors11[a].adress, derus);
						break;
					} else if (a + 1 == i11) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0xC0) {
				for (a = 0; a < i12; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors12[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors12[a].adress, derus);
						break;
					} else if (a + 1 == i12) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0xD0) {
				for (a = 0; a < i13; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors13[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors13[a].adress, derus);
						break;
					} else if (a + 1 == i13) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0xE0) {
				for (a = 0; a < i14; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors14[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors14[a].adress, derus);
						break;
					} else if (a + 1 == i14) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else if (*(goodimagechecksums + i * 16) < 0xF0) {
				for (a = 0; a < i15; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors15[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors15[a].adress, derus);
						break;
					} else if (a + 1 == i15) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}

			else {
				for (a = 0; a < i16; a++) {
					if (buffercmp(goodimagechecksums + i * 16, sectors16[a].md5, 16)) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(sectors16[a].adress, derus);
						break;
					} else if (a + 1 == i16) {
						fseek(derus, 56 + i * 4, SEEK_SET);
						_putw(0xFFFFFFFF, derus);
						fseek(derus, 56 + goodimagesectors * 4 + brokensectors * 2336, SEEK_SET);
						fseek(goodimage, i * 2352 + 16, SEEK_SET);
						fread(buffer, 1, 2336, goodimage);
						fwrite(buffer, 1, 2336, derus);
						brokensectors++;
					}
				}
			}
		}
		free(sectors1);
		free(sectors2);
		free(sectors3);
		free(sectors4);
		free(sectors5);
		free(sectors6);
		free(sectors7);
		free(sectors8);
		free(sectors9);
		free(sectors10);
		free(sectors11);
		free(sectors12);
		free(sectors13);
		free(sectors14);
		free(sectors15);
		free(sectors16);
		free(goodimagechecksums);

	} else {
		printf("Derus mode: Normal\n\nComparing images...");
		if (goodimageedc) {
			for (i = 0; i < goodimagesectors; i++) {
				fseek(goodimage, 16, SEEK_CUR);
				fread(buffer, 1, 2336, goodimage);
				if (i <= badimagesectors) {
					fseek(badimage, 16, SEEK_CUR);
					fread(buffer2, 1, 2336, badimage);
					if (buffercmp(buffer, buffer2, 2336)) continue;
				}
				putw(i, derus);
				fwrite(buffer, 1, 2336, derus);
				brokensectors++;
			}
		} else {
			for (i = 0; i < goodimagesectors; i++) {
				fseek(goodimage, 18, SEEK_CUR);
				fread(&ctrlbyte, 1, 1, goodimage);
				fseek(goodimage, -3, SEEK_CUR);
				fread(buffer, 1, 2336, goodimage);
				if (i <= badimagesectors) {
					fseek(badimage, 16, SEEK_CUR);
					fread(buffer2, 1, 2336, badimage);
					if (ctrlbyte >> 5 & 0x1) {
						if (buffercmp(buffer, buffer2, 2332)) continue;
					} else if (buffercmp(buffer, buffer2, 2336)) continue;
				}
				putw(i, derus);
				fwrite(buffer, 1, 2336, derus);
				brokensectors++;
			}
		}
	}
	printf(" Done!\nNumber of broken sectors: %u\n\n", brokensectors);
	printf("Patch is ready to use.");
	return;
}


void derus (s8 * argv[]) {
	FILE * badimage, * goodimage, * derus;
	u8 buffer[2336], buffer_md5[2352], derus_goodimagemd5[16], derus_badimagemd5[16], goodimagemd5[16], badimagemd5[16], derus_goodedc, derus_badedc, derus_rebuild, goodimagefilename[2048];
	u32 i, derus_goodimagesectors, derus_badimagesectors;
	MD5_CTX context;
	u8 header[16] = {0x00, 0xFF, 0xFF, 0xFF,
					 0xFF, 0xFF, 0xFF, 0xFF,
					 0xFF, 0xFF, 0xFF, 0x00,
					 0x00, 0x00, 0x00, 0x02};

	//Opening & creating files
	if (fopen_s(&badimage, argv[2], "r+b") != 0) {
		printf("Error opening file \"%s\"!\n", argv[2]);
		return;
	}
	u32 badimagesize = getfilesize(badimage);
	if (badimagesize % 2352 != 0) {
		printf("File \"%s\" is not in Mode2/2352 image!\n", argv[2]);
		fclose(badimage);
		return;
	}
	u32 badimagesectors = badimagesize / 2352;

	if (fopen_s(&derus, argv[3], "rb") != 0) {
		printf("Error opening file \"%s\" for reading!\n", argv[3]);
		fclose(badimage);
		return;
	}
	fread(buffer, 1, 5, derus);

	if (memcmp(buffer, "DERUS", 5) != 0) {
		printf("File \"%s\" is not in not in .drs format!\n", argv[3]);
		fclose(badimage); fclose(derus);
		return;
	}

	//Reading info from derus
	fread(&derus_rebuild, 1, 1, derus);
	fread( &derus_badedc, 1, 1, derus);
	fread(&derus_goodedc, 1, 1, derus);
	fseek(derus, 8, SEEK_CUR);
	fread( &derus_badimagesectors, 4, 1, derus);
	fread(&derus_goodimagesectors, 4, 1, derus);
	fread( derus_badimagemd5, 1, 16, derus);
	fread(derus_goodimagemd5, 1, 16, derus);

	if (derus_rebuild) {
		strcat((s8 *)goodimagefilename, argv[2]);
		s8 * point;
		if (!(point = strchr((s8 *)goodimagefilename, '.'))) strcat((s8 *)goodimagefilename, "-derused.bin");
		else strcpy((s8 *)point, "-derused.bin");
		if (fopen_s(&goodimage, (const char *)goodimagefilename, "w+b") != 0) {
			printf("Error creating file \"%s\"!\n", goodimagefilename);
			fclose(badimage); fclose(derus);
			return;
		}
	}

	//Bad image info
	printf("Non-original image:\nFilename: %s\nSize (bytes): %u\nSize (sectors): %u\n", argv[2], badimagesectors * 2352, badimagesectors);
	if (badimagesectors != derus_badimagesectors) {
		printf("File \"%s\" has wrong size!\nCorrect size (bytes): %u\nCorrect size (sectors): %u", argv[2], derus_badimagesectors * 2352, derus_badimagesectors);
		return;
	}
	printf("Calculating MD5...");
	MD5Init(&context);
	while (fread(buffer_md5, 1, 2352, badimage)) MD5Update(&context, buffer_md5, 2352);
	MD5Final(badimagemd5, &context);
	fseek(badimage, 0, SEEK_SET);
	printf(" ");
	for (i = 0; i < 16; i++) printf("%02x", badimagemd5[i]);
	if (memcmp(badimagemd5, derus_badimagemd5, 16) != 0) {
		printf("\nFile \"%s\" has wrong MD5!\nCorrect MD5: ", argv[2]);
		for (i = 0; i < 16; i++) printf("%02x", derus_badimagemd5[i]);
		return;
	}
	printf("\n\n");

	//Recreating image
	if (derus_rebuild) {
		printf("Recreating image...");
		u32 * sectors = (u32 *)calloc(derus_goodimagesectors, 4);
		for (i = 0; i < derus_goodimagesectors; i++) fread(sectors + i, 4, 1, derus);
		for (i = 0; i < derus_goodimagesectors; i++) {
			msf(i, header + 12);
			fwrite(header, 1, 16, goodimage);
			if (*(sectors + i) != 0xFFFFFFFF) {
				fseek(badimage, sectors[i] * 2352 + 16, SEEK_SET);
				fread(buffer, 1, 2336, badimage);
				fwrite(buffer, 1, 2336, goodimage);
			} else {
				fread(buffer, 1, 2336, derus);
				fwrite(buffer, 1, 2336, goodimage);
			}
		}
		printf(" Done!\n");
		if (!derus_goodedc) {
			printf("Removing EDC...");
			fseek(goodimage, 0, SEEK_SET);
			u8 z, zero[4] = {0x00, 0x00, 0x00, 0x00};
			for (i = 0; i < derus_goodimagesectors; i++) {
				fseek(goodimage, 18, SEEK_CUR);
				fread(&z, 1, 1, goodimage);
				if (z >> 5 & 0x1) {
					fseek(goodimage, 2329, SEEK_CUR);
					fwrite(zero, 1, 4, goodimage);
				} else fseek(goodimage, 2333, SEEK_CUR);
			}
			printf(" Done!\n");
		}
		printf("\nChecking new image...\n");
		fseek(goodimage, 0, SEEK_SET);
		u32 goodimagesectors = getfilesize(goodimage) / 2352;
		printf("Size (bytes): %u\nSize (sectors): %u\n", goodimagesectors * 2352, goodimagesectors);
		if (goodimagesectors != derus_goodimagesectors) {
			printf("File \"%s\" has wrong size!\nCorrect size (bytes): %u\nCorrect size (sectors): %u", goodimagefilename, derus_goodimagesectors * 2352, derus_goodimagesectors);
			return;
		}
		printf("Calculating MD5...");
		MD5Init(&context);
		while (fread(buffer_md5, 1, 2352, goodimage)) MD5Update(&context, buffer_md5, 2352);
		MD5Final(goodimagemd5, &context);
		printf(" ");
		for (int i = 0; i < 16; i++) printf("%02x", goodimagemd5[i]);
		if (!buffercmp(goodimagemd5, derus_goodimagemd5, 16)) {
			printf("\nFile \"%s\" has wrong MD5!\nCorrect MD5: ", goodimagefilename);
			for (int i = 0; i < 16; i++) printf("%02x", derus_goodimagemd5[i]);
			return;
		}
	} else {
		printf("Patching image...");
		while (fread(&i, 4, 1, derus) != 0) {
			fseek(badimage, i * 2352, SEEK_SET);
			msf(i, header + 12);
			fwrite(header, 1, 16, badimage);
			fread(buffer, 1, 2336, derus);
			fwrite(buffer, 1, 2336, badimage);
		}
		if (derus_goodimagesectors < derus_badimagesectors) {
			fclose(badimage);
			//wchar_t newfilename[2048];
			//for(u8 i = 0; i < strlen(argv[2]); i++) {
			//	newfilename[i] = argv[2][i];
			//	*((char *)newfilename +i*2 +1) = 0;
			//}
			//newfilename[strlen(argv[2])] = 0;
			HANDLE hfile = CreateFileA(argv[2], GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
			SetFilePointer(hfile, derus_goodimagesectors * 2352, 0, FILE_BEGIN);
			SetEndOfFile(hfile);
			CloseHandle(hfile);
			fopen_s(&badimage, argv[2], "r+b");
		}
		printf(" Done!\n");
		if (!derus_goodedc) {
			printf("Removing EDC...");
			fseek(badimage, 0, SEEK_SET);
			u8 z, zero[4] = {0x00, 0x00, 0x00, 0x00};
			for (i = 0; i < derus_goodimagesectors; i++) {
				fseek(badimage, 18, SEEK_CUR);
				fread(&z, 1, 1, badimage);
				if (z >> 5 & 0x1) {
					fseek(badimage, 2329, SEEK_CUR);
					fwrite(zero, 1, 4, badimage);
				} else fseek(badimage, 2333, SEEK_CUR);
			}
			printf(" Done!\n");
		}
		printf("\nChecking new image...\n");
		u32 goodimagesectors = getfilesize(badimage) / 2352;
		printf("Size (bytes): %u\nSize (sectors): %u\n", goodimagesectors * 2352, goodimagesectors);
		if (goodimagesectors != derus_goodimagesectors) {
			printf("File \"%s\" has wrong size!\nCorrect size (bytes): %u\nCorrect size (sectors): %u", argv[2], derus_goodimagesectors * 2352, derus_goodimagesectors);
			return;
		}
		printf("Calculating MD5...");
		MD5Init(&context);
		fseek(badimage, 0, SEEK_SET);
		while (fread(buffer_md5, 1, 2352, badimage)) MD5Update(&context, buffer_md5, 2352);
		MD5Final(goodimagemd5, &context);
		printf(" ");
		for (int i = 0; i < 16; i++) printf("%02x", goodimagemd5[i]);
		if (!buffercmp(goodimagemd5, derus_goodimagemd5, 16)) {
			printf("\nFile \"%s\" has wrong MD5!\nCorrect MD5: ", argv[2]);
			for (int i = 0; i < 16; i++) printf("%02x", derus_goodimagemd5[i]);
			return;
		}
	}
	printf("\n\nDone!\n");
}