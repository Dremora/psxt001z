#include "common.h"

int memsearch(const unsigned char *buf_where, const unsigned char *buf_search, int buf_where_len, int buf_search_len) {
	for (int i = 0; i <= buf_where_len-buf_search_len; i++) {
		for (int j = 0; j < buf_search_len; j++) {
			if (buf_where[i+j] != buf_search[j]) break;
			if (j + 1 == buf_search_len) return i;
		}
	}
	return -1;
}

int __main(int argc, char *argv[]) {
	if (argc < 2 || argc > 3) {
		printf("Syntax: px_p8 [-t] filename\n");
		exit(1);
	}
	int sectors;
	if (argc == 3 && !strcmp(argv[1], "-t")) {
		argv[1] = argv[2];
		sectors = 2352;
	} else sectors = 4704;
	FILE *sector_file;
	unsigned char *sector = new unsigned char[sectors];
	const unsigned char sync[12] = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
	sector_file = fopen(argv[1], "rb");
	if (sector_file == NULL) {
		perror(argv[1]);
		exit(1);
	}

	if (argc == 3) {
		unsigned int hex;
		for (int i = 0; fscanf(sector_file, "%02x", &hex) != EOF && i < 3252; i++) {
			sector[i] = (unsigned char)hex;
		}
	} else {
		fread(sector, 1, sectors, sector_file);
	}
	int offset = memsearch(sector, sync, sectors, 12);
	if (offset == -1) {
		printf("Error searching for sync!\n");
		exit(1);
	}
	int ShiftRegister = 0x1;
	//printf("MSF: %02x:%02x:%02x\n", sector[offset+12], sector[offset+12+1], sector[offset+12+2]);
	for (int i = 0; i < 3; i++)  { 
		sector[offset+12+i] ^= (ShiftRegister&0xFF); 
		for (int j = 0; j < 8; j++) {
			int hibit = ((ShiftRegister & 1)^((ShiftRegister & 2)>>1)) << 15; 
			ShiftRegister = (hibit | ShiftRegister) >> 1; 
		} 
	}
	int start_sector = (btoi(sector[offset+12])*60 + btoi(sector[offset+13]))*75 + btoi(sector[offset+14]) - 150;
	printf("MSF: %02x:%02x:%02x\n", sector[offset+12], sector[offset+12+1], sector[offset+12+2]);
	offset -= start_sector * 2352;
	printf("Combined offset: %+d bytes / %+d samples\n", offset, offset / 4);
	return 0;
}