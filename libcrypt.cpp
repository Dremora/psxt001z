#include "common.h"

u32 lc_addresses[LIBCRYPT_NUM_SECTORS] = 
{
	13955, 13960, 14081, 14086, 14335, 14340, 14429, 14434,
	14499, 14504, 14749, 14754, 14906, 14911, 14980, 14985,
	15092, 15097, 15162, 15167, 15228, 15233, 15478, 15483,
	15769, 15774, 15881, 15886, 15951, 15956, 16017, 16022,
	41895, 41900, 42016, 42021, 42282, 42287, 42430, 42435,
	42521, 42526, 42663, 42668, 42862, 42867, 43027, 43032, 
	43139, 43144, 43204, 43209, 43258, 43263, 43484, 43489,
	43813, 43818, 43904, 43909, 44009, 44014, 44162, 44167
};

u8 lc1_sectors_contents[768] = 
{
	0x41, 0x01, 0x01, 0x07, 0x06, 0x05, 0x00, 0x23, 0x08, 0x05, 0x38, 0x39,
	0x41, 0x01, 0x01, 0x03, 0x06, 0x11, 0x00, 0x03, 0x08, 0x90, 0x5d, 0xa0,
	0x41, 0x01, 0x01, 0x07, 0x07, 0x56, 0x00, 0x23, 0x09, 0x56, 0xdf, 0xde,
	0x41, 0x01, 0x01, 0x03, 0x07, 0x60, 0x00, 0x03, 0x09, 0xe1, 0xf2, 0x50,
	0x41, 0x01, 0x01, 0x03, 0x13, 0x10, 0x00, 0x03, 0x53, 0x10, 0x50, 0xec,
	0x41, 0x01, 0x01, 0x43, 0x11, 0x15, 0x00, 0x01, 0x13, 0x15, 0x23, 0x1e,
	0x41, 0x01, 0x01, 0x03, 0x12, 0x09, 0x00, 0x03, 0x14, 0x2d, 0x04, 0x73,
	0x41, 0x01, 0x01, 0x03, 0x1a, 0x34, 0x00, 0x03, 0x04, 0x34, 0xe2, 0xcf,
	0x41, 0x01, 0x01, 0x03, 0x13, 0x20, 0x00, 0x03, 0x15, 0x04, 0x82, 0x35,
	0x41, 0x01, 0x01, 0x01, 0x13, 0x29, 0x00, 0x43, 0x15, 0x29, 0x72, 0xe2,
	0x41, 0x01, 0x01, 0x03, 0x1e, 0x49, 0x00, 0x03, 0x08, 0x49, 0x32, 0xc5,
	0x41, 0x01, 0x01, 0x01, 0x16, 0x54, 0x00, 0x43, 0x18, 0x54, 0xd4, 0x79,
	0x41, 0x01, 0x01, 0x03, 0x18, 0x57, 0x00, 0x03, 0x20, 0xd6, 0xbc, 0x27,
	0x41, 0x01, 0x01, 0x03, 0x38, 0x61, 0x00, 0x03, 0x24, 0x61, 0x91, 0xa9,
	0x41, 0x01, 0x01, 0x0b, 0x19, 0x55, 0x00, 0x13, 0x21, 0x55, 0x14, 0x07,
	0x41, 0x01, 0x01, 0x03, 0x19, 0x62, 0x00, 0x03, 0x21, 0x20, 0x5d, 0x48,
	0x41, 0x01, 0x01, 0x03, 0x23, 0x17, 0x00, 0x03, 0x63, 0x17, 0x6d, 0xc6,
	0x41, 0x01, 0x01, 0x43, 0x21, 0x22, 0x00, 0x01, 0x23, 0x22, 0x24, 0x89,
	0x41, 0x01, 0x01, 0x03, 0x02, 0x12, 0x00, 0x03, 0x20, 0x12, 0x49, 0x43,
	0x41, 0x01, 0x01, 0x03, 0x22, 0x07, 0x00, 0x03, 0x24, 0x1f, 0x3a, 0xb1,
	0x41, 0x01, 0x01, 0x03, 0x23, 0x13, 0x00, 0x03, 0x25, 0x0b, 0x93, 0xc9,
	0x41, 0x01, 0x01, 0x0b, 0x23, 0x08, 0x00, 0x13, 0x25, 0x08, 0xce, 0x5d,
	0x41, 0x01, 0x01, 0x03, 0x06, 0x28, 0x00, 0x03, 0x2c, 0x28, 0xd7, 0xd6,
	0x41, 0x01, 0x01, 0x0b, 0x26, 0x33, 0x00, 0x13, 0x28, 0x33, 0x9c, 0x29,
	0x41, 0x01, 0x01, 0x03, 0x30, 0x59, 0x00, 0x03, 0x32, 0x1b, 0x2c, 0xc6,
	0x41, 0x01, 0x01, 0x03, 0x20, 0x24, 0x00, 0x03, 0x3a, 0x24, 0xe6, 0xac,
	0x41, 0x01, 0x01, 0x13, 0x31, 0x56, 0x00, 0x0b, 0x33, 0x56, 0x97, 0xed,
	0x41, 0x01, 0x01, 0x03, 0x31, 0x65, 0x00, 0x03, 0x33, 0x41, 0xba, 0x63,
	0x41, 0x01, 0x01, 0x01, 0x32, 0x51, 0x00, 0x43, 0x34, 0x51, 0xd7, 0xa9,
	0x41, 0x01, 0x01, 0x03, 0x33, 0x56, 0x00, 0x03, 0xb4, 0x56, 0xc0, 0x9a,
	0x41, 0x01, 0x01, 0x03, 0x32, 0x42, 0x00, 0x03, 0xb5, 0x42, 0x69, 0xe2,
	0x41, 0x01, 0x01, 0x03, 0x33, 0x07, 0x00, 0x03, 0x35, 0x45, 0x1a, 0x10,
	0x41, 0x01, 0x01, 0x09, 0x18, 0x65, 0x00, 0x09, 0x20, 0x41, 0x40, 0x72,
	0x41, 0x01, 0x01, 0x19, 0x18, 0x50, 0x00, 0x01, 0x20, 0x50, 0x25, 0xeb,
	0x41, 0x01, 0x01, 0x08, 0x20, 0x16, 0x00, 0x89, 0x22, 0x16, 0x95, 0xa8,
	0x41, 0x01, 0x01, 0x09, 0x20, 0x01, 0x00, 0x09, 0x22, 0x25, 0xb8, 0x26,
	0x41, 0x01, 0x01, 0x09, 0x23, 0x53, 0x00, 0x09, 0x25, 0x77, 0x21, 0x03,
	0x41, 0x01, 0x01, 0x0b, 0x23, 0x62, 0x00, 0x49, 0x25, 0x62, 0x68, 0x4c,
	0x41, 0x01, 0x01, 0x0d, 0x25, 0x55, 0x00, 0x29, 0x27, 0x55, 0xae, 0x41,
	0x41, 0x01, 0x01, 0x09, 0x25, 0x61, 0x00, 0x09, 0x27, 0xe0, 0xe7, 0x0e,
	0x41, 0x01, 0x01, 0x08, 0x26, 0x71, 0x00, 0x89, 0x28, 0x71, 0x95, 0xcb,
	0x41, 0x01, 0x01, 0x09, 0x27, 0x21, 0x00, 0x09, 0x29, 0x05, 0x80, 0x4b,
	0x41, 0x01, 0x01, 0x0b, 0x28, 0x63, 0x00, 0x49, 0x30, 0x63, 0xed, 0x18,
	0x41, 0x01, 0x01, 0x09, 0x29, 0x68, 0x00, 0x09, 0xb0, 0x68, 0xb0, 0x8c,
	0x41, 0x01, 0x01, 0x29, 0x31, 0x37, 0x00, 0x0d, 0x33, 0x37, 0x6c, 0x68,
	0x41, 0x01, 0x01, 0x09, 0x31, 0x4a, 0x00, 0x09, 0x33, 0x52, 0x7c, 0x8b,
	0x41, 0x01, 0x01, 0x09, 0x73, 0x52, 0x00, 0x09, 0x37, 0x52, 0x4b, 0x06,
	0x41, 0x01, 0x01, 0x19, 0x33, 0x57, 0x00, 0x01, 0x35, 0x57, 0x38, 0xf4,
	0x41, 0x01, 0x01, 0x09, 0x35, 0x04, 0x00, 0x09, 0x37, 0x1c, 0x54, 0x6a,
	0x41, 0x01, 0x01, 0x09, 0x31, 0x19, 0x00, 0x09, 0x17, 0x19, 0xa4, 0xbd,
	0x41, 0x01, 0x01, 0x01, 0x36, 0x04, 0x00, 0x19, 0x38, 0x04, 0x9c, 0xdf,
	0x41, 0x01, 0x01, 0x09, 0x36, 0x0b, 0x00, 0x09, 0x38, 0x49, 0x6c, 0x08,
	0x41, 0x01, 0x01, 0x49, 0x36, 0x58, 0x00, 0x0b, 0x38, 0x58, 0x99, 0xbf,
	0x41, 0x01, 0x01, 0x09, 0x36, 0x73, 0x00, 0x09, 0x38, 0x6b, 0xfe, 0x96,
	0x41, 0x01, 0x01, 0x0b, 0x39, 0x59, 0x00, 0x49, 0x41, 0x59, 0x54, 0x0d,
	0x41, 0x01, 0x01, 0x09, 0x39, 0x24, 0x00, 0x09, 0x41, 0x66, 0x9e, 0x67,
	0x41, 0x01, 0x01, 0x09, 0x44, 0x1b, 0x00, 0x09, 0x46, 0x03, 0x78, 0x0d,
	0x41, 0x01, 0x01, 0x09, 0x46, 0x18, 0x00, 0x09, 0x06, 0x18, 0x25, 0x99,
	0x41, 0x01, 0x01, 0x09, 0x45, 0x2b, 0x00, 0x09, 0x47, 0x69, 0xd3, 0xc5,
	0x41, 0x01, 0x01, 0x09, 0x05, 0x34, 0x00, 0x09, 0x45, 0x34, 0x35, 0x79,
	0x41, 0x01, 0x01, 0x09, 0x44, 0x59, 0x00, 0x09, 0x08, 0x59, 0x6e, 0x0a,
	0x41, 0x01, 0x01, 0x49, 0x46, 0x64, 0x00, 0x0b, 0x48, 0x64, 0xa4, 0x60,
	0x41, 0x01, 0x01, 0x09, 0x08, 0x62, 0x00, 0x09, 0x52, 0x62, 0x03, 0x5a,
	0x41, 0x01, 0x01, 0x19, 0x48, 0x67, 0x00, 0x01, 0x50, 0x67, 0x70, 0xa8
};

u8 libcryptdrv(int argc, char *argv[]) {
	FILE *f;
	HANDLE hDevice;
	ScsiPassThroughDirect SRB;
	char offset = 0, path[] ="\\\\.\\X:";
	BYTE i, sub[12], buffer[BUFFER_LEN], buffer2352[23520], buffer2[BUFFER_LEN], buffer3[BUFFER_LEN], buffer4[BUFFER_LEN];;
	LPBYTE status1, status2, status;
	WORD crc;
	DWORD sector, sector_start, sector_end, a, lcsectors = 0, todo = 9300, done = 0;

	if (argc != 1 || (argc == 1 && (argv[0][1] != 0 && (argv[0][1] != ':' || argv[0][2] != 0)))) {
		printf("LibCrypt drive detector\nUsage: psxt001z.exe --libcryptdrv <drive letter>\n");
		return 0;
	}

	path[4] = argv[0][0];
	if ((hDevice = CreateFile(path, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE) {
		printf("Can't open device!\n");
		return 0;
	}

	if (fopen_s(&f, F_NAME, "wb") != 0) {
		printf("Can\'t open file %s!\n", F_NAME);
		return 0;
	}

	status1 = (LPBYTE)calloc(4650, 1);
	status2 = (LPBYTE)calloc(4650, 1);

	// Offset detection
	printf("Determining offset...\r");
	ReadSub(buffer, 0, f, offset, hDevice, SRB);

	switch (buffer[8]) {
		case 0x01:
			offset = 75 - btoi(buffer[9]);
			break;
		case 0x02:
			offset = - btoi(buffer[9]);
			break;
		default:
			printf("Can't determine offset!\n%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
			return 0;
	}

	sub[0] = buffer[0];
	sub[1] = 0x01;
	sub[2] = 0x01;
	sub[6] = 0x00;

	printf("Subchannels offset correction: %d\n", offset);

	// Section 1) 02:58:00 - 03:69:74 -- status1
	// Section 2) 08:58:00 - 09:69:74 -- status2

	// Step 1

	for (i = 0; i < CYCLES * 3; i++) {

		if (!todo) goto end;

		if (i % 3 == 0) {
			sector_start = 13350;
			sector_end   = 18000;
			status = status1;
		} else if (i % 3 == 1) {
			sector_start = 40350;
			sector_end   = 45000;
			status = status2;
		} else {
			printf("Left: %4u / Flushing cache...             \r", todo);
			ClearCache(buffer2352, f, offset, hDevice, SRB);
			continue;
		}

		for (sector = sector_start; sector < sector_end; sector++) {
			if (status[sector - sector_start])
				continue;
			ReadSub(buffer, sector, f, offset, hDevice, SRB);
			printf("Left: %4u / Sector %u...         \r", todo, sector);
			// generating q-channel
			sub[3] = itob(sector/60/75);
			sub[4] = itob((sector / 75) % 60);
			sub[5] = itob(sector % 75);
			sub[7] = itob((sector + 150)/60/75);
			sub[8] = itob(((sector + 150) / 75) % 60);
			sub[9] = itob((sector + 150) % 75);
			crc = crc16(sub, 10);
			sub[10] = HIBYTE(crc);
			sub[11] = LOBYTE(crc);
			if (!memcmp(sub, buffer, 12)) {
				status[sector - sector_start] = 1;
				todo--;
				done++;
			}
		}
	}

	// Step 2

	for (i = 0; i < 2; i++) {
		if (i == 0) {
			sector_start = 13350;
			sector_end   = 18000;
			status = status1;
		} else {
			sector_start = 40350;
			sector_end   = 45000;
			status = status2;
		}

		for (sector = sector_start; sector < sector_end; sector++) {
			if (status[sector - sector_start])
				continue;
			ReadSub(buffer, sector, f, offset, hDevice, SRB);
			printf("Left: %4u / Sector %u...         \r", todo, sector);
			// generating q-channel
			sub[3] = itob(sector / 60 / 75);
			sub[4] = itob((sector / 75) % 60);
			sub[5] = itob(sector % 75);
			sub[7] = itob((sector + 150) / 60 / 75);
			sub[8] = itob(((sector + 150) / 75) % 60);
			sub[9] = itob((sector + 150) % 75);
			crc = crc16(sub, 10);
			sub[10] = HIBYTE(crc);
			sub[11] = LOBYTE(crc);
			if (memcmp(sub, buffer, 12) != 0) {
				printf("Left: %4u / Sector %u: flushing cache...        \r", todo, sector);
				do {
					ReadSub(buffer, sector, f, offset, hDevice, SRB);
					ClearCache(buffer2352, f, offset, hDevice, SRB);
					ReadSub(buffer2, sector, f, offset, hDevice, SRB);
					ClearCache(buffer2352, f, offset, hDevice, SRB);
					ReadSub(buffer3, sector, f, offset, hDevice, SRB);
					ClearCache(buffer2352, f, offset, hDevice, SRB);
				} while (memcmp(buffer, buffer2, BUFFER_LEN) != 0 || memcmp(buffer, buffer3, BUFFER_LEN) != 0);
				//} while (!matrix(buffer, buffer2, buffer3, buffer4, BUFFER_LEN));
				if (memcmp(buffer, sub, BUFFER_LEN) != 0) {
					fprintf(f, "MSF: %02x:%02x:%02x Q-Data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", sub[7], sub[8], sub[9], buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
					lcsectors++;
					//fwrite(SRB.SRB_BufPointer, 1, SRB.SRB_BufLen - 4, f);
					fflush(f);
				}
			}
			todo--;
			done++;
		}
	}

	end:

	free(status1);
	free(status2);

	printf("Done!                                                           \nProtected sectors: ");
	if (lcsectors == 0)
		printf("None\n");
	else
		printf("%u\n", lcsectors);
	fclose(f);
	return 1;
}

int libcryptdrvfast(int argc, char *argv[]) {
	FILE *f;
	HANDLE hDevice;
	ScsiPassThroughDirect SRB;
	s8 offset = 0, path[] ="\\\\.\\X:";
	u8 buffer[BUFFER_LEN], buffer2352[23520], sub[12], lc1sectors = 0, lc2sectors = 0, othersectors = 0;
	u16 crc;

	if (argc != 1 || (argc == 1 && (argv[0][1] != 0 && (argv[0][1] != ':' || argv[0][2] != 0)))) {
		printf("LibCrypt drive detector (fast)\nUsage: psxt001z.exe --libcryptdrvfast <drive letter>\n");
		return 0;
	}

	path[4] = argv[0][0];

	if ((hDevice = CreateFile(path, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE) {
		printf("Can't open device!\n");
		return 0;
	}

	if (fopen_s(&f, F_NAME, "wb") != 0) {
		printf("Can\'t open file %s!\n", F_NAME);
		return 0;
	}

	// Offset detection
	ReadSub(buffer, 0, f, offset, hDevice, SRB);
	//if (buffer[5] != buffer[9]) {
	//	printf("Error determining offset!\nSector 0: %02x%02x%02x %02x:%02x:%02x %02x %02x:%02x:%02x %02x%02x\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
	//}
	switch (buffer[8]) {
		case 0x01:
			offset = 75 - btoi(buffer[9]);
			break;
		case 0x02:
			offset = - btoi(buffer[9]);
			break;
		default:
			printf("Can't determine offset!\nSector 0: %02x%02x%02x %02x:%02x:%02x %02x %02x:%02x:%02x %02x%02x\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
			return 0;
	}
	printf("Subchannels offset correction: %d\n", offset);
	sub[0] = buffer[0];
	sub[1] = 0x01;
	sub[2] = 0x01;
	sub[6] = 0x00;

	for (int i = 0; i < LIBCRYPT_NUM_SECTORS; i++) {
		printf("\nReading sector %u... ", lc_addresses[i]);
		ReadSub(buffer, lc_addresses[i], f, offset, hDevice, SRB);
		// generating q-channel
		sub[3] = itob(lc_addresses[i]/60/75);
		sub[4] = itob((lc_addresses[i] / 75) % 60);
		sub[5] = itob(lc_addresses[i] % 75);
		sub[7] = itob((lc_addresses[i] + 150)/60/75);
		sub[8] = itob(((lc_addresses[i] + 150) / 75) % 60);
		sub[9] = itob((lc_addresses[i] + 150) % 75);
		crc = crc16(sub, 10);
		sub[10] = HIBYTE(crc);
		sub[11] = LOBYTE(crc);
		for (int a = 1; a <= READ_TIMES; a++) {
			if (!memcmp(sub, buffer, 12)) {
				printf("original sector");
				break;
			} else if (!memcmp(lc1_sectors_contents + (12 * i), buffer, 12)) {
				printf("LibCrypt, LC1 sector");
				fprintf(f, "MSF: %02x:%02x:%02x Q-Data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", sub[7], sub[8], sub[9], buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
				lc1sectors++;
				break;
			} else {
				if (a < READ_TIMES) {
					ClearCache(buffer2352, 0, offset, hDevice, SRB);
					continue;
				} else {
					printf("unknown");
					fprintf(f, "MSF: %02x:%02x:%02x Q-Data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", sub[7], sub[8], sub[9], buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
					othersectors++;
				}
			}
		}


		
	}
	printf("\n\nOriginal sectors: %u", LIBCRYPT_NUM_SECTORS - lc1sectors - lc2sectors - othersectors);
	printf("\nLC1 sectors:      %u", lc1sectors);
	printf("\nLC2 sectors:      %u", lc2sectors);
	printf("\nOther sectors:    %u", othersectors);
	fprintf(f, "\nOriginal sectors: %u", LIBCRYPT_NUM_SECTORS - lc1sectors - lc2sectors - othersectors);
	fprintf(f, "\nLC1 sectors:      %u", lc1sectors);
	fprintf(f, "\nLC2 sectors:      %u", lc2sectors);
	fprintf(f, "\nOther sectors:    %u", othersectors);
	fclose(f);
	return 1;
}

void ReadSub(LPBYTE buffer, DWORD sector, FILE *f, char offset, HANDLE hDevice, ScsiPassThroughDirect SRB) {
	static DWORD returned;
	ZeroMemory(&SRB, sizeof(ScsiPassThroughDirect));
	SRB.Length = sizeof(ScsiPassThroughDirect);
	SRB.CDBLength = 12;
	SRB.DataIn = SCSI_IOCTL_DATA_IN;
	SRB.DataTransferLength = BUFFER_LEN;
	SRB.TimeOutValue = 30;
	SRB.DataBuffer = buffer;
	SRB.CDB[0] = RAW_READ_CMD;
	SRB.CDB[2] = HIBYTE(HIWORD(sector + offset));
	SRB.CDB[3] = LOBYTE(HIWORD(sector + offset));
	SRB.CDB[4] = HIBYTE(LOWORD(sector + offset));
	SRB.CDB[5] = LOBYTE(LOWORD(sector + offset));
	SRB.CDB[8] = 1;
	SRB.CDB[10] = 1;

	if (!DeviceIoControl(hDevice, IOCTL_SCSI_PASS_THROUGH_DIRECT, &SRB, sizeof(ScsiPassThroughDirect), &SRB, SENSE_SIZE, &returned, 0)) {
		printf("\nError reading subchannel data!\n");
		if (f != 0)
			fprintf(f, "Error reading subchannel data!");
		exit(0);
	}
	deinterleave(buffer);
	return;
}

void ClearCache(LPBYTE buffer, FILE *f, char offset, HANDLE hDevice, ScsiPassThroughDirect SRB) {
	static DWORD returned;
	for (DWORD sector = 0; sector < 1000; sector += 10) {
		ZeroMemory(&SRB, sizeof(ScsiPassThroughDirect));
		SRB.Length = sizeof(ScsiPassThroughDirect);
		SRB.CDBLength = 12;
		SRB.DataIn = SCSI_IOCTL_DATA_IN;
		SRB.DataTransferLength = 23520;
		SRB.TimeOutValue = 30;
		SRB.DataBuffer = buffer;
		SRB.CDB[0] = RAW_READ_CMD;
		SRB.CDB[2] = HIBYTE(HIWORD(sector + offset));
		SRB.CDB[3] = LOBYTE(HIWORD(sector + offset));
		SRB.CDB[4] = HIBYTE(LOWORD(sector + offset));
		SRB.CDB[5] = LOBYTE(LOWORD(sector + offset));
		SRB.CDB[8] = 10;
		SRB.CDB[9] = 0xF8;

		DeviceIoControl(hDevice, IOCTL_SCSI_PASS_THROUGH_DIRECT, &SRB, sizeof(ScsiPassThroughDirect), &SRB, SENSE_SIZE, &returned, 0); 

		if (!DeviceIoControl(hDevice, IOCTL_SCSI_PASS_THROUGH_DIRECT, &SRB, sizeof(ScsiPassThroughDirect), &SRB, SENSE_SIZE, &returned, 0)) {
			printf("\nError clearing cache!\n");
			if (f != 0)
				fprintf(f, "Error clearing cache!");
			exit(0);
		}
	}
	return;
}

BYTE matrix(LPBYTE buffer, LPBYTE buffer2, LPBYTE buffer3, LPBYTE buffer4, DWORD length) {
	for (register DWORD i = 0; i < length; i++) {
		if (*(buffer + i) == *(buffer2 + i)) {
			if (*(buffer + i) == *(buffer3 + i)) continue;
			if (*(buffer + i) == *(buffer4 + i)) continue;
		} else if (*(buffer + i) == *(buffer3 + i) && *(buffer + i) == *(buffer4 + i)) continue;
		else if (*(buffer2 + i) == *(buffer3 + i) && *(buffer2 + i) == *(buffer4 + i)) continue;
		return 0;
	}
	return 1;
}

void deinterleave(LPBYTE buffer) {
	static BYTE buffertmp[12];
	ZeroMemory(buffertmp, 12);
	static int i;
	for (i = 0; i < 12; i++) {
		buffertmp[i] |= ((buffer[i * 8    ] & 0x40) << 1);
		buffertmp[i] |= ((buffer[i * 8 + 1] & 0x40)     );
		buffertmp[i] |= ((buffer[i * 8 + 2] & 0x40) >> 1);
		buffertmp[i] |= ((buffer[i * 8 + 3] & 0x40) >> 2);
		buffertmp[i] |= ((buffer[i * 8 + 4] & 0x40) >> 3);
		buffertmp[i] |= ((buffer[i * 8 + 5] & 0x40) >> 4);
		buffertmp[i] |= ((buffer[i * 8 + 6] & 0x40) >> 5);
		buffertmp[i] |= ((buffer[i * 8 + 7] & 0x40) >> 6);
	}
	CopyMemory(buffer, buffertmp, 12);
	return;
}

u8 libcrypt(int argc, char *argv[]) {
	if (argc != 1 && argc != 2) {
		printf("LibCrypt detector\nUsage: psxt001z.exe --libcrypt <sub> [<sbi>]\n");
		printf("  Check subchannel for LibCrypt protection.\n");
		printf("  [in]  <sub>  Specifies the subchannel file to be scanned.\n");
		printf("  [out] <sbi>  Specifies the subchannel file in SBI format where protected\n               sectors will be written.\n\n");
		return 0;
	}
	// Variables
	FILE * subfile, * sbi;
	u8 buffer[16], sub[16];//, pregap = 0;
	u16 crc;
	u32 sector, psectors = 0, size, tpos;
	s8 * address, ext[3];

	ZeroMemory(sub, 12);

	// opening .sub
	if (fopen_s(&subfile, argv[0], "rb") != 0) {
		perror(argv[0]);
		return 0;
	}
	// checking extension
	if (!(address = strrchr(argv[0], '.'))) {
		printf("%s: undefined file extension - can't detect file type\n", argv[0]);
		return 0;
	}
	address++;
	// converting extension to lowercase
	for (u8 i = 0; address[i] != 0 && i < 3; i++)
		ext[i] = tolower(address[i]);
	// file type
	if (memcmp(ext, "sub", 3) != 0) {
		printf("%s: unknown file extension\n", argv[0]);
		return 0;
	}
	// filesize
	size = getfilesize(subfile);
	if (size % 96 != 0) {
		printf("%s: wrong size\n", argv[0]);
		return 0;
	}

	// sbi
	if (argc == 2) {
		if (fopen_s(&sbi, argv[1], "w+b") != 0) {
			perror(argv[1]);
			return 0;
		}
		fwrite("SBI", 1, 4, sbi);
	}

	for (sector = 150; sector < ((size / 96) + 150); sector++) {
		fseek(subfile, 12, SEEK_CUR);
		if (!fread(buffer, 1, 12, subfile))
			return 1;
		fseek(subfile, 72, SEEK_CUR);

		// New track
		if ((btoi(buffer[1]) == (btoi(sub[1]) + 1)) && (buffer[2] == 0 || buffer[2] == 1)) {
			memcpy(sub, buffer, 6);
			tpos = ((btoi(buffer[3] * 60) + btoi(buffer[4])) * 75) + btoi(buffer[5]);
		}

		//New index
		else if (btoi(buffer[2]) == (btoi(sub[2]) + 1) && buffer[1] == sub[1]) {
			memcpy(sub + 2, buffer + 2, 4);
			tpos = ((btoi(buffer[3] * 60) + btoi(buffer[4])) * 75) + btoi(buffer[5]);
		}

		//MSF1 [3-5]
		else {
			if (sub[2] == 0)
				tpos--;
			else
				tpos++;
			sub[3] = itob(tpos / 60 / 75);
			sub[4] = itob((tpos / 75) % 60);
			sub[5] = itob(tpos % 75);
		}

		//MSF2 [7-9]
		sub[7] = itob(sector / 60 / 75);
		sub[8] = itob((sector / 75) % 60);
		sub[9] = itob(sector % 75);

		// CRC-16 [10-11]
		crc = crc16(sub, 10);
		sub[10] = HIBYTE(crc);
		sub[11] = LOBYTE(crc);

		//if (buffer[10] != sub[10] && buffer[11] != sub[11] && (buffer[3] != sub[3] || buffer[7] != sub[7] || buffer[4] != sub[4] || buffer[8] != sub[8] || buffer[5] != sub[5] || buffer[9] != sub[9])) {
		//if (buffer[10] != sub[10] || buffer[11] != sub[11] || buffer[3] != sub[3] || buffer[7] != sub[7] || buffer[4] != sub[4] || buffer[8] != sub[8] || buffer[5] != sub[5] || buffer[9] != sub[9]) {
		if (memcmp(buffer, sub, 6) != 0 || memcmp(buffer + 7, sub + 7, 5) != 0) {
			printf("MSF: %02x:%02x:%02x Q-Data: %02x%02x%02x %02x:%02x:%02x %02x %02x:%02x:%02x %02x%02x  xor %04x %04x", sub[7], sub[8], sub[9], buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11], crc ^ ((buffer[10] << 8) + buffer[11]), crc16(buffer, 10) ^ ((buffer[10] << 8) + buffer[11]));
			//printf("\nMSF: %02x:%02x:%02x Q-Data: %02x%02x%02x %02x:%02x:%02x %02x %02x:%02x:%02x %02x%02x", sub[7], sub[8], sub[9], sub[0], sub[1], sub[2], sub[3], sub[4], sub[5], sub[6], sub[7], sub[8], sub[9], sub[10], sub[11]);
			if (buffer[3] != sub[3] && buffer[7] != sub[7] && buffer[4] == sub[4] && buffer[8] == sub[8] && buffer[5] == sub[5] && buffer[9] == sub[9])
				printf(" P1 xor %02x %02x", buffer[3] ^ sub[3], buffer[7] ^ sub[7]);
			else if (buffer[3] == sub[3] && buffer[7] == sub[7] && buffer[4] != sub[4] && buffer[8] != sub[8] && buffer[5] == sub[5] && buffer[9] == sub[9])
				printf(" P2 xor %02x %02x", buffer[4] ^ sub[4], buffer[8] ^ sub[8]);
			else if (buffer[3] == sub[3] && buffer[7] == sub[7] && buffer[4] == sub[4] && buffer[8] == sub[8] && buffer[5] != sub[5] && buffer[9] != sub[9])
				printf(" P3 xor %02x %02x", buffer[5] ^ sub[5], buffer[9] ^ sub[9]);
			else
				printf(" ?");
			printf("\n");
			psectors++;
			if (argc == 2) {
				fwrite(sub+7, 1, 3, sbi);
				fputc(1, sbi);
				fwrite(buffer, 1, 10, sbi);
			}
		}
	}
//	}
	printf("Number of modified sectors: %u\n", psectors);
	return 1;
}

int xorlibcrypt(void) {
	s8 b;
	u8 d;
	u8 i, a, x, sub[12] = {
		0x41, 0x01, 0x01, 0x00, 
		0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00
	};
	u16 crc;
	for (i = 0; i < LIBCRYPT_NUM_SECTORS; i++) {
		sub[3] = itob(lc_addresses[i]/60/75);
		sub[4] = itob((lc_addresses[i] / 75) % 60);
		sub[5] = itob(lc_addresses[i] % 75);
		sub[7] = itob((lc_addresses[i] + 150)/60/75);
		sub[8] = itob(((lc_addresses[i] + 150) / 75) % 60);
		sub[9] = itob((lc_addresses[i] + 150) % 75);
		crc = crc16(sub, 10);
		sub[10] = HIBYTE(crc);
		sub[11] = LOBYTE(crc);
		printf("%u %02x:%02x:%02x", lc_addresses[i], sub[7], sub[8], sub[9]);
		printf(" %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x %02x%02x", sub[0], sub[1], sub[2], sub[3], sub[4], sub[5], sub[6], sub[7], sub[8], sub[9], sub[10], sub[11]);
		printf(" %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x %02x%02x", lc1_sectors_contents[i*12], lc1_sectors_contents[(i*12)+1], lc1_sectors_contents[(i*12)+2], lc1_sectors_contents[(i*12)+3], lc1_sectors_contents[(i*12)+4], lc1_sectors_contents[(i*12)+5], lc1_sectors_contents[(i*12)+6], lc1_sectors_contents[(i*12)+7], lc1_sectors_contents[(i*12)+8], lc1_sectors_contents[(i*12)+9], lc1_sectors_contents[(i*12)+10], lc1_sectors_contents[(i*12)+11]);
		d = 0;
		for (a = 3; a < 12; a++) {
			x = (lc1_sectors_contents[(i*12)+a] ^ sub[a]);
			printf(" %x%x%x%x%x%x%x%x", (x>>7)&0x1, (x>>6)&0x1, (x>>5)&0x1, (x>>4)&0x1, (x>>3)&0x1, (x>>2)&0x1, (x>>1)&0x1, x&0x1);
			if (x == 0)
				continue;
			for (b = 7; b >= 0; b--) {
				if ((x>>b)&0x1) {
					d = (d<<1);
					d |= ((sub[a]>>b)&0x1);
				}	
			}
		}
		printf(" %x%x%x%x", (d>>3)&0x1, (d>>2)&0x1, (d>>1)&0x1, d&0x1);
		printf("\n");
	}
	return 1;
}