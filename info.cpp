#include "common.h"

int info(char *filename, int fix) {
	// Variables
	FILE *image;
	crc32 crc;
	bool errors = 0;
	unsigned char buffer[2352], buffer2[2352], bad, edc_form_2[4] = {0x3F, 0x13, 0xB0, 0xBE},
		synñheader[16] = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00},
		subheader[8] = {0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00},
		&mode = synñheader[15], imageedc;

	int sector, sectors, size, realsectors, realsize, sectorsize;

	// Opening image
	char *open_mode = fix ? "r+b" : "rb";
	if (fopen_s(&image, filename, open_mode) != 0) {
		perror(filename);
		return 1;
	}
	size = getfilesize(image);
	printf("File: %s\n", filename);

	// Type
	fread(buffer, 1, 12, image);
	if (memcmp(buffer, synñheader, 12) == 0) {
		sectorsize = 2352;
	} else {
		sectorsize = 2048;
	}
	if (size % sectorsize != 0) {
		printf("%s: not ModeX/%d image!\n", filename, sectorsize);
		return 1;
	}
	sectors = size / sectorsize;

	// Mode
	if (sectorsize = 2352) {
		fseek(image, 0xF, SEEK_SET);
		fread(&mode, 1, 1, image);
		if (mode != 1 && mode != 2) {
			printf("%s: unknown mode!\n", filename);
			return 1;
		}
	} else {
		mode = -1;
	}

	// Size
	fseek(image, sectorsize * 16 + ((mode == 2) ? 24 : ((mode == 1) ? 16 : 0)) + 0x50, SEEK_SET);
	fread(&realsectors, 4, 1, image); // ISO size
	fseek(image, 0, SEEK_SET);
	realsize = realsectors * sectorsize;
	if (sectors == realsectors) {
		printf("Size (bytes):   %d (OK)\n", size);
		printf("Size (sectors): %d (OK)\n", sectors);
	} else {
		printf("Size (bytes):   %d\n", size);
		printf("From image:     %d\n", realsize);
		printf("Size (sectors): %d\n", sectors);
		printf("From image:     %d\n", realsectors);
	}

	if (mode > 0) {
		printf("Mode: %d\n", mode);
	}
	if (mode == 2) {
		// EDC in Form 2
		imageedc = getedc(image);
		printf("EDC in Form 2 sectors: ");
		imageedc ? printf("YES\n") : printf("NO\n");

		// Sysarea
		printf("System area: ");
		fseek(image, 0, SEEK_SET);
		for (int i = 0; i < 16; i++) {
			fread(buffer, 1, 2352, image);
			crc.ProcessCRC(buffer, 2352);
		}
		u32 imagecrc = crc.m_crc32;
		switch (imagecrc) {
			case 0x11e3052d: printf("Eu EDC"); break;
			case 0x808c19f6: printf("Eu NoEDC"); break;
			case 0x70ffa73e: printf("Eu Alt NoEDC"); break;
			case 0x783aca30: printf("Jap EDC"); break;
			case 0xe955d6eb: printf("Jap NoEDC"); break;
			case 0x9b519a2e: printf("US EDC"); break;
			case 0x0a3e86f5: printf("US NoEDC"); break;
			case 0x6773d4db: printf("US Alt NoEDC"); break;
			default: printf("Unknown, crc %08x", imagecrc);
		}
		printf("\n");

		// Postgap
		fseek(image, (sectors - 150) * sectorsize + 16, SEEK_SET);
		fread(buffer, 1, 2336, image);
		printf("Postgap type: Form ");
		if (buffer[2] >> 5 & 0x1) {
			printf("2");
			if (!memcmp(subheader, buffer, 8)) printf(", zero subheader");
			else printf(", non-zero subheader");
			if (zerocmp(buffer+8, 2324)) printf(", zero data");
			else printf(", non-zero data");
			if (zerocmp(buffer+2332, 4)) printf(", no EDC");
			else printf(", EDC");
		} else {
			printf("1");
			if (zerocmp(buffer, 8)) printf(", zero subheader");
			else printf(", non-zero subheader");
			if (zerocmp(buffer+8, 2328)) printf(", zero data");
			else printf(", non-zero data");
		}
		memcpy(buffer2, buffer, 2336);
	}

	if (mode < 0) return 0;

	for (sector = sectors - 150; sector < sectors; sector++) {
		bad = 0;
		fseek(image, sector * sectorsize, SEEK_SET);
		fread(buffer, 1, sectorsize, image);

		// Sync
		msf(sector, synñheader + 12);
		if (memcmp(synñheader, buffer, 16) != 0) {
			printf("\nSector %u: Sync/Header", sector);
			bad = 1;
			if (fix) {
				fseek(image, sector * sectorsize, SEEK_SET);
				fwrite(synñheader, 1, 16, image);
				printf(" (fixed)");
			}
		}

		// Mode 2
		if (mode == 2 && memcmp(buffer + 16, buffer2, 2336)) {
			if (bad) printf(", Subheader/Data/EDC/ECC");
			else {
				printf("\nSector %u: Subheader/Data/EDC/ECC", sector);
				bad = 1;
			}
			if (fix) {
				fseek(image, sector * sectorsize + 16, SEEK_SET);
				fwrite(buffer2, 1, 2336, image);
				printf(" (fixed)");
			}
		}
		if (bad && (sector +1 != sectors)) errors = true;
	}
	if (errors) {
		printf("\nNOTICE: One or more errors were found not in the last sector.\nPlease mention this when submitting dump info.\n");
		system("pause");
	} else printf("\nDone.\n");
	return 0;
}