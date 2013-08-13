#include "common.h"

crc32 :: crc32() {
	const unsigned CRC_POLY = 0xEDB88320;
	unsigned i, j, r;
	for (i = 0; i < 256; i++){
		for (r = i, j = 8; j; j--)
			r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;
		table[i] = r;
	}
	m_crc32 = 0;
}

void crc32 :: ProcessCRC(void* pData, register int nLen) {
	const unsigned CRC_MASK = 0xD202EF8D;
	register unsigned char* pdata = reinterpret_cast<unsigned char*>(pData);
	register unsigned crc = m_crc32;
	while (nLen--){
		crc = table[(unsigned char)crc ^ *pdata++] ^ crc >> 8;
		crc ^= CRC_MASK;
	}
	m_crc32 = crc;
}

class track {
private:
	FILE *f;
	char *path;
	unsigned long start, crc, size;
	unsigned char riff[44], isriff, mode, smallfile, * fcontents, savetrack;
	signed long fastoffset[77], offset, current;
public:
	track(char *argv[], int argc) {
		char *end;
		fopen_s ( &f, argv[2], "rb" );
		start = strtoul(argv[3],&end,10);
		size = strtoul(argv[4],&end,10);
		crc = strtoul(argv[5],&end,16);
		offset = 0;
		isriff = 0;
		current = 0;
		smallfile = 0;
		mode = 1;
		savetrack = 0;
		printf ("File: %s\nStart: %d\nSize: %d\nCRC-32: %08x\n\n", argv[2], start, size, crc);
		for(char i = 6; i < argc; i++)
		if(argc >= 7) {
			if(!strcmp(argv[i],"r")) {
				riff[0] = 0x52;
				riff[1] = 0x49;
				riff[2] = 0x46;
				riff[3] = 0x46;
				riff[8] = 0x57;
				riff[9] = 0x41;
				riff[10] = 0x56;
				riff[11] = 0x45;
				riff[12] = 0x66;
				riff[13] = 0x6D;
				riff[14] = 0x74;
				riff[15] = 0x20;
				riff[16] = 0x10;
				riff[17] = 0x00;
				riff[18] = 0x00;
				riff[19] = 0x00;
				riff[20] = 0x01;
				riff[21] = 0x00;
				riff[22] = 0x02;
				riff[23] = 0x00;
				riff[24] = 0x44;
				riff[25] = 0xAC;
				riff[26] = 0x00;
				riff[27] = 0x00;
				riff[28] = 0x10;
				riff[29] = 0xB1;
				riff[30] = 0x02;
				riff[31] = 0x00;
				riff[32] = 0x04;
				riff[33] = 0x00;
				riff[34] = 0x10;
				riff[35] = 0x00;
				riff[36] = 0x64;
				riff[37] = 0x61;
				riff[38] = 0x74;
				riff[39] = 0x61;
				isriff = 1;
				*(unsigned long*)&riff[4] = size-8;
				*(unsigned long*)&riff[40] = size-44;
				size -= 44;
			}
			else if(argv[i][0]=='+') mode = 'p';
			else if(argv[i][0]=='-') mode = 'n';
			else if(!strcmp(argv[i],"s")) {
				savetrack = 1;
				i++;
				path = argv[i];
			}
		}
		if(size <= 100000000) {
			fcontents = (unsigned char*)malloc(size + 40000);
			fseek(f, start - 20000, SEEK_SET);
			//fread(fcontents, 1, size, f);
			for(unsigned long i = 0; i < size + 40000; i++) {
				if(start + i <= 20000) fseek(f, start + i - 20000, SEEK_SET);
				if(!fread(&fcontents[i], 1, 1, f)) fcontents[i] = 0x00;
			}
			smallfile = 1;
		}
	}
	int trackmain(void) {
		if(mode == 'p') {
			if(current > 20000) {
				mode = 0;
				return 1;
			}
			offset = current;
			current += 4;
			return calculate();
		}
		else if(mode == 'n') {
			if(current > 20000) {
				mode = 0;
				return 1;
			}
			offset = - current;
			current += 4;
			return calculate();
		}
		else {
			if(current > 20000) {
				mode = 0;
				return 1;
			}
			offset = current;
			if(calculate()) return 1;
			else {
				offset = - current;
				current += 4;
				return calculate();
			}
		}
	}
	u32 calculate(void) {
		crc32 calc;
		if(smallfile) {
			if(isriff) calc.ProcessCRC(riff, 44);
			calc.ProcessCRC(&fcontents[20000 + offset], size);
		}
		else {
			unsigned char buffer;
			unsigned long i;
			fseek(f, start+offset, SEEK_SET);
			if(isriff) calc.ProcessCRC(riff, 44);
			for(i = 0; i < size; i++) {
				if(!fread(&buffer, 1, 1, f)) {
					buffer = 0x00;
					fseek(f, start + offset + i + 1, SEEK_SET);
				}
				calc.ProcessCRC(&buffer, 1);
			}
		}
		printf ("Offset correction %d bytes, %d samples, CRC-32 %08x\n", offset, offset/4, calc.m_crc32);
		return (calc.m_crc32 == crc);
	}
	void done() {
		if(smallfile) free(fcontents);
		if(!mode) printf ("\nCan't find offset!\n");
		else {
			if(savetrack) {
				FILE *f2;
				unsigned char buffer;
				fopen_s ( &f2, path, "w+b" );
				if(isriff) fwrite(riff, 1, 44, f2);
				fseek(f, start + offset, SEEK_SET);
				for(unsigned long i = 0; i < size; i++) {
					if(!fread(&buffer, 1, 1, f)) {
						buffer = 0x00;
						fseek(f, start + offset + i + 1, SEEK_SET);
					}
					fwrite(&buffer, 1, 1, f2);
				}
			}
			printf ("\nDONE!\n\nOffset correction: %d bytes / %d samples\n", offset, offset/4);
		}
	}
};

class filetools {
private:
	FILE *f;
	char exename[20];
	char datevalue[11];
public:
	filetools(FILE * file) {
		f = file;
	}
	u32 size() {
		fseek(f, 0, SEEK_END);
		return ftell(f);
	}
	char *exe() {
		char buffer[20];
		u32 lba;
		fseek(f, 51744, SEEK_SET);
		while(strcmp(buffer,"SYSTEM.CNF") != 0)
		{
			fread(buffer, 1, 10, f);
			buffer[10] = 0;
			fseek(f, -9, SEEK_CUR);
		}
		fseek(f, -32, SEEK_CUR);
		fread(&lba, 4, 1, f);
		fseek(f, (2352 * lba) + 29, SEEK_SET);
		fread(&buffer, 1, 6, f);
		buffer[6] = 0;
		while(strcmp(buffer,"cdrom:") != 0) {
			fseek(f, -5, SEEK_CUR);
			fread(&buffer, 1, 6, f);
		}
		fread(&buffer, 1, 1, f);
		if(buffer[0] != '\\') fseek(f, -1, SEEK_CUR);
		long i = -1;
		do fread(&buffer[++i], 1, 1, f); while(buffer[i] != ';');
		for(long a = 0; a < i; a++ ) exename[a] = (char)toupper(buffer[a]);
		exename[i] = 0;
		return exename;
	}
	char *date() {
		char buffer[12],datenofrmt[3];
		fseek( f, 51744, SEEK_SET );
		do {
			fread( buffer, 1, 11, f );
			buffer[11]=NULL;
			fseek( f, -10, SEEK_CUR);
		} while(strcmp(exename,buffer)!=0);
		fseek(f, -16, SEEK_CUR);
		fread(datenofrmt, 1, 3, f);
		if (datenofrmt[0] < 50) itoa(datenofrmt[0]+2000, datevalue, 10);
		else itoa(datenofrmt[0]+1900, datevalue, 10);
		*(datevalue+4) = '-';
		if(datenofrmt[1] < 10) {
			datevalue[5] = '0';
			itoa (datenofrmt[1],&datevalue[6],10);
		} else itoa (datenofrmt[1],&datevalue[5],10);
		*(datevalue+7) = '-';
		if(datenofrmt[2] < 10) {
			datevalue[8] = '0';
			itoa (datenofrmt[2],&datevalue[9],10);
		} else itoa (datenofrmt[2],&datevalue[8],10);
		*(datevalue+10) = 0;
		return datevalue;
	}
	int resize(const char *filename, u32 newsize) {
		u32 oldsize = size();
		if (oldsize == newsize) return 0;
		else if (oldsize < newsize) {
			fseek(f, newsize-1, SEEK_SET);
			fputc(0, f);
			return 1;
		}
		else if (oldsize > newsize) {
			fclose(f);
			wchar_t newfilename[1024];
			for(u8 i = 0; i < strlen(filename); i++) {
				newfilename[i] = filename[i];
				*((char *)newfilename +i*2 +1) = 0;
			}
			newfilename[strlen(filename)] = 0;
			HANDLE hfile = CreateFileW(newfilename, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
			SetFilePointer(hfile, newsize, 0, FILE_BEGIN);
			SetEndOfFile(hfile);
			CloseHandle(hfile);
			fopen_s(&f, filename, "rb");
			return 2;
		}
	}
	u32 imagesize() {
		u32 sectors;
		fseek(f, 0x9368, SEEK_SET);
		fread(&sectors, 4, 1, f);
		return sectors;
	}
};

void info(const char * filename) {
	FILE *image;
	crc32 crc;
	u8 buffer[2352];

	if(fopen_s(&image, filename, "rb") != 0) {
		printf("Error opening file \"%s\"!\n",filename);
		return;
	}

	filetools file(image);
	u32 imagesize = getfilesize(image);
	printf("File: %s\n", filename);
	if (imagesize % 2352 != 0) {
		printf("File \"%s\" is not Mode2/2352 image!\n", filename);
		fclose(image);
		return;
	}
	u32 realsectors = file.imagesize();
	u32 imagesectors = imagesize / 2352;
	u32 realsize = realsectors * 2352;
	if (imagesize == realsize) {
		printf("Size (bytes):   %d (OK)\n", imagesize);
		printf("Size (sectors): %d (OK)\n", imagesectors);
	} else {
		printf("Size (bytes):   %d\n", imagesize);
		printf("From image:     %d\n", realsize);
		printf("Size (sectors): %d\n", imagesectors);
		printf("From image:     %d\n", realsectors);
	}


	printf("EDC in Form 2 sectors: ");
	if (getedc(image)) printf("YES\n");
	else printf("NO\n");
	char *exe = file.exe();
	printf("ID: %c%c%c%c-%c%c%c%c%c\n",exe[0],exe[1],exe[2],exe[3],exe[5],exe[6],exe[7],exe[9],exe[10]);
	printf("Date: %s\n",file.date());
	printf("System area: ");
	fseek(image, 0, SEEK_SET);
	for (u32 i = 0; i < 16; i++) {
		fread(buffer, 1, 2352, image);
		crc.ProcessCRC(buffer, 2352);
	}
	u32 imagecrc = crc.m_crc32;
	switch (imagecrc) {
		case 0x11e3052d: printf("Eu EDC"); break;
		case 0x808c19f6: printf("Eu NoEDC"); break;
		case 0x70ffa73e: printf("Eu Alt NoEDC"); break;
		case 0x7f9a25b1: printf("Eu Alt 2 EDC"); break;
		case 0x783aca30: printf("Jap EDC"); break;
		case 0xe955d6eb: printf("Jap NoEDC"); break;
		case 0x9b519a2e: printf("US EDC"); break;
		case 0x0a3e86f5: printf("US NoEDC"); break;
		case 0x6773d4db: printf("US Alt NoEDC"); break;
		default: printf("Unknown, crc %x", imagecrc);
	}
	printf("\n\n");
	fclose(image);
	return;
}
void help() {
	printf("Usage:\n");
	printf("======\n");
	printf("psxt001z.exe image.bin\n");
	printf("  Display image's info.\n\n");
	printf("psxt001z.exe --scan image.bin\n");
	printf("  Scan image.bin postgap for errors.\n\n");
	printf("psxt001z.exe --fix image.bin\n");
	printf("  Scan image.bin postgap for errors and fix them.\n\n");
	printf("psxt001z.exe --libcryptdrvfast <drive letter>\n");
	printf("  Check subchannels for LibCrypt protection using new detection\n  method (disc).\n\n");
	printf("psxt001z.exe --checksums file [start [end]]\n");
	printf("  Calculate file's checksums (CRC-32, MD5, SHA-1).\n");
	printf("  [in] file   Specifies the file, which checksums will be calculated.\n");
	printf("       start  Specifies start position for checksums calculation.\n");
	printf("       size   Specifies size of block for checksums calculation.\n\n");
	printf("psxt001z.exe --zektor image.bin\n");
	printf("  Zektor. Replace EDC in Form 2 Mode 2 sectors with zeroes.\n\n");
	printf("psxt001z.exe --antizektor image.bin\n");
	printf("  Antizektor. Restore EDC in Form 2 Mode 2 sectors.\n\n");
	printf("psxt001z.exe --resize image.bin size\n");
	printf("  Resize file to requested size.\n\n");
	printf("psxt001z.exe --patch image.bin patch.bin offset\n");
	printf("  Insert patch.bin into image.bin, skipping given number of bytes from the\n  offset.\n\n");
	printf("psxt001z.exe --track image.bin bytes_to_skip size crc-32 [r] [+/-/f] [s filename]\n");
	printf("  Try to guess an offset correction of the image dump by searching a track with\n  given size and CRC-32.\n  r - Calculate crc with RIFF header.\n  +/- - Search only for positive or negative offset correction.\n  s - Save track with given filename.\n\n");
	printf("psxt001z.exe --gen file.bin filesize [-r]\n");
	printf("  Generate a file of the requested size.\n  -r - add RIFF header.\n\n");
	printf("psxt001z.exe --str file.str video.str audio.xa\n");
	printf("  Deinterleave file.str to video.str and audio.xa.\n\n");
	printf("psxt001z.exe --str2bs file.str\n");
	printf("  Convert file.str to .bs-files.\n\n");
	printf("psxt001z.exe --sub subchannel.sub size\n");
	printf("  Generate RAW subchannel with given size (in sectors).\n\n");
	printf("psxt001z.exe --m3s subchannel.m3s\n");
	printf("  Generate M3S subchannel.\n\n");
	printf("psxt001z.exe --libcrypt <sub> [<sbi>]\n");
	printf("Usage: psxt001z.exe --libcrypt <sub> [<sbi>]\n");
	printf("  Check subchannels for LibCrypt protection. (file)\n");
	printf("  [in]  <sub>   Specifies the subchannel file to be scanned.\n");
	printf("  [out] <sbi>   Specifies the subchannel file in SBI format where protected\n  sectors will be written.\n\n");
	printf("psxt001z.exe --libcryptdrv <drive letter>\n");
	printf("  Check subchannels for LibCrypt protection (disc).\n\n");

	printf("Press any key to continue...");
	_getch();
}
int patch(char * argv[]) {
	FILE * f1, * f2;
	u8 buffer;
	u32 i = 0;
	if (fopen_s(&f1, argv[2], "r+b") != 0) {
		printf("Error opening file \"%s\"!\n", argv[2]);
		return 0;
	}
	if (fopen_s(&f2, argv[3], "rb") != 0) {
		printf("Error opening file \"%s\"!\n", argv[3]);
		return 0;
	}
	u32 skip = atol(argv[4]);
	fseek(f1, skip, SEEK_SET);
	printf("Patching \"%s\" with \"%s\", skipping %d bytes...\n", argv[2], argv[3], skip);
	for (; fread(&buffer, 1, 1, f2); i++) fwrite(&buffer, 1, 1, f1);
	printf("Done!\n\n%d bytes were replaced\n",i);
	printf("File was successully patched!\n");
	return 1;
}
int resize(char * argv[]) {
	FILE *f;
	if(fopen_s(&f, argv[2], "r+b") != 0) {
		printf("Error opening file \"%s\"!\n",argv[2]);
		return 0;
	}
	filetools image(f);
	unsigned long newsize = atol(argv[3]);
	switch(image.resize(argv[2],newsize)) {
		case 0: printf("File's \"%s\" size is already %d bytes!\n",argv[2],newsize);break;
		case 1: printf("File \"%s\" was successfully resized to %d bytes!\n",argv[2],newsize);break;
		case 2: printf("File \"%s\" was successfully truncated to %d bytes!\n",argv[2],newsize);break;
	}
	return 1;
}
int copy(char * argv[]) {
	//argv[1] - infile
	//argv[2] - outfile
	//argv[3] - startbyte
	//argv[4] - length
	char *errorstring;
	FILE * infile;
	__int64 startbyte = _strtoi64(argv[3], &errorstring, 10);
	if (fopen_s(&infile, argv[1], "rb") != 0) {
		printf("File %s can't be found.", argv[1]);
		return 1;
	}
	return 1;
	//HANDLE hfile = CreateFileW(argv[1], GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
	//SetFilePointer(hfile, newsize, 0, FILE_BEGIN);
	//SetEndOfFile(hfile);
}
int main(int argc, char *argv[]) {
	printf("psxt001z by Dremora, %s\n\n", VERSION);
	     if (argc > 1 && (!strcmp(argv[1], "--checksums") || !strcmp(argv[1], "-c"))) checksums(argc, argv);
	else if (argc > 1 && (!strcmp(argv[1], "--libcrypt") || !strcmp(argv[1], "-l"))) libcrypt(argc - 2, argv + 2);
	else if (argc > 1 && !strcmp(argv[1], "--libcryptdrv")) libcryptdrv(argc - 2, argv + 2);
	else if (argc > 1 && !strcmp(argv[1], "--libcryptdrvfast")) libcryptdrvfast(argc - 2, argv + 2);
	else if (argc > 1 && !strcmp(argv[1], "--xorlibcrypt")) xorlibcrypt();
	
	else if (argc == 2) info(argv[1]);

	else if (argc == 3 && !strcmp(argv[1], "--zektor")) zektor(argv[2]);
	else if (argc == 3 && !strcmp(argv[1], "--antizektor")) antizektor(argv[2]);
	else if (argc == 5 && !strcmp(argv[1], "--patch")) patch(argv);
	else if (argc == 4 && !strcmp(argv[1], "--resize")) resize(argv);
	else if (argc >= 6 && argc <= 10 && !strcmp(argv[1], "--track")) {track trackfix(argv, argc); while(!trackfix.trackmain()){;} trackfix.done();}
	else if (argc == 5 && !strcmp(argv[1], "--str")) str(argv);
	else if (argc == 3 && !strcmp(argv[1], "--str2bs")) str2bs(argv);
	else if ((argc == 4 || argc == 5) && !strcmp(argv[1], "--gen")) generate(argc, argv);
	else if (argc == 3 && !strcmp(argv[1], "--scan")) info(argv[2], 0);
	else if (argc == 3 && !strcmp(argv[1], "--fix")) info(argv[2], 1);
	else if (argc == 4 && !strcmp(argv[1], "--sub")) sub(argv[2], argv[3]);
	else if (argc == 3 && !strcmp(argv[1], "--m3s")) m3s(argv[2]);
	else if (argc == 6 && !strcmp(argv[1], "--matrix")) matrix(argv);
	else help();
	return 1;
}

void antizektor(s8 * filename) {
	FILE * image;
	u32 filesize, sectors, sector;
	u8 z, buffer[2332];
	if (fopen_s(&image, filename, "r+b") != 0) {
		perror(filename);
		return;
	}

	static u8  ecc_f_lut[256];
	static u8  ecc_b_lut[256];
	static u32 edc_lut[256];
	u32 a, b, edc_init;
	for(a = 0; a < 256; a++) {
		b = (a << 1) ^ (a & 0x80 ? 0x11D : 0);
		ecc_f_lut[a] = b;
		ecc_b_lut[a ^ b] = a;
		edc_init = a;
		for (b = 0; b < 8; b++) edc_init = (edc_init >> 1) ^ (edc_init & 1 ? 0xD8018001 : 0);
		edc_lut[a] = edc_init;
	}

	filesize = getfilesize(image);
	if (filesize % 2352 != 0) {
		printf("File \"%s\" is not Mode2/2352 image!\n", filename);
		fclose(image);
		return;
	} 
	sectors = filesize / 2352;
	printf("Converting image...\n");
	for (sector = 0; sector < sectors; sector++) {
		fseek(image, sector * 2352 + 18, SEEK_SET);
		fread(&z, 1, 1, image);
		if (z >> 5 & 0x1) {
			fseek(image, -3, SEEK_CUR);
			fread(buffer, 1, 2332, image);
			fseek(image, 0, SEEK_CUR);
			putw(calculate_edc(buffer, 2332, edc_lut), image);
		}
	}
	fclose(image);
	printf("Done!\n");
	return;
}
u8 checksums(int argc, char *argv[]) {
	if (argc < 3 || argc > 5) {
		printf("psxt001z.exe --checksums file [start [end]]\n");
		printf("  Calculate file's checksums (CRC-32, MD5, SHA-1).\n");
		printf("  [in] file   Specifies the file, which checksums will be calculated.\n");
		printf("       start  Specifies start position for checksums calculation.\n");
		printf("       size   Specifies size of block for checksums calculation.\n\n");
		return 0;
	}

	// opening file
	FILE * file;
	if (fopen_s(&file, argv[2], "rb") != 0) {
		perror(argv[2]);
		return 0;
	}
	printf("File:   %s\n", argv[2]);

	s8 *byte;
	u8 percents = 0;
	u32 start, block, filesize, total;

	filesize = getfilesize(file);
	if (argc > 3) {
		start = strtoul(argv[3], &byte, 10);
		if (start >= filesize) {
			printf("Error:  start position can't be larger than filesize!\n");
			return 0;
		}
		printf("Start:  %u\n", start);
	} else
		start = 0;

	if (argc > 4) {
		block = strtoul(argv[4], &byte, 10);
		if (block > filesize) {
			printf("Error:  block size can't be larger than filesize!\n");
			return 0;
		}
		if (block == 0) {
			printf("Error:  block size can't equal with zero!\n");
			return 0;
		}
	} else
		block = filesize - start;
	if (block + start > filesize) {
		printf("Error:  block size and start position can't be larger than file size!\n");
		return 0;
	}
	printf("Size:   %u\n", block);
	total = ceil(double(double(block) / 1024));

	// checksums
	uint8_t Message_Digest[20];
	crc32 crc;
	u8 buffer[1024], digest[16];
	u32 len;
	SHA1Context sha;
	MD5_CTX context;
	MD5Init(&context);
	SHA1Reset(&sha);
	fseek(file, start, SEEK_SET);
	for (u32 i = 0; i < total; i++) {
		if (i * 100 / total > percents) {
			percents = i * 100 / total;
			printf("\rCalculating checksums: %02u%%", percents);
		}
		len = fread(buffer, 1, 1024, file);
		if (block <= len) {
			len = block;
			block = 0;
		} else
			block -= 1024;
		MD5Update(&context, buffer, len);
		SHA1Input(&sha, buffer, len);
		crc.ProcessCRC(buffer, len);
	}
	MD5Final (digest, &context);
	SHA1Result(&sha, Message_Digest);
	printf ("\rCRC-32: %08x                      \n", crc.m_crc32);
	printf ("MD5:    ");
	for (int i = 0; i < 16; i++ )
		printf ( "%02x", digest[i] );
	printf ("\nSHA-1:  ");
	for (int i = 0; i < 20 ; i++ )
		printf ( "%02x", Message_Digest[i] );
	printf ("\n\n");
	return 1;
}

void generate(u32 argc, s8 * argv[]) {
	FILE *f;
	u8 riff[44];
	u32 size = atol(argv[3]);
	if (fopen_s(&f, argv[2], "wb") != 0) {
		printf("Error creating file \"%s\"!\n", argv[2]);
		return;
	}
	if (argc == 5) {
		if (!strcmp(argv[4], "-r")) {
			riff[0] = 0x52;
			riff[1] = 0x49;
			riff[2] = 0x46;
			riff[3] = 0x46;
			riff[8] = 0x57;
			riff[9] = 0x41;
			riff[10] = 0x56;
			riff[11] = 0x45;
			riff[12] = 0x66;
			riff[13] = 0x6D;
			riff[14] = 0x74;
			riff[15] = 0x20;
			riff[16] = 0x10;
			riff[17] = 0x00;
			riff[18] = 0x00;
			riff[19] = 0x00;
			riff[20] = 0x01;
			riff[21] = 0x00;
			riff[22] = 0x02;
			riff[23] = 0x00;
			riff[24] = 0x44;
			riff[25] = 0xAC;
			riff[26] = 0x00;
			riff[27] = 0x00;
			riff[28] = 0x10;
			riff[29] = 0xB1;
			riff[30] = 0x02;
			riff[31] = 0x00;
			riff[32] = 0x04;
			riff[33] = 0x00;
			riff[34] = 0x10;
			riff[35] = 0x00;
			riff[36] = 0x64;
			riff[37] = 0x61;
			riff[38] = 0x74;
			riff[39] = 0x61;
			*(unsigned long*)&riff[4] = size-8;
			*(unsigned long*)&riff[40] = size-44;
			fwrite(riff, 1, 44, f);
		}
	}
	fseek(f, size - 1, SEEK_SET);
	putc(0x00, f);
	fclose(f);
	printf("File \"%s\" with size %d bytes was successfully generated!\n", argv[2], size);
	return;
}
void m3s(s8 * filename) {
	FILE * subchannel;
	u8 buffer[10] = {0x41, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		min, sec, frame;
	u16 crc;
	u32 sectors, sector, sector2, i;
	double mindbl, secdbl;

	if (fopen_s(&subchannel, filename, "w+b") != 0) {
		perror(filename);
		return;
	}

	printf("File: %s\n", filename);

	for (sector = 13350, sector2 = sector + 150; sector < 17850; sector++, sector2++) {
		mindbl = sector/60/75;
		min = floor(mindbl);
		secdbl = (sector - (min * 60 * 75)) / 75;
		sec = floor(secdbl);
		frame = sector - (min * 60 * 75) - (sec * 75);
		buffer[3] = itob(min);
		buffer[4] = itob(sec);
		buffer[5] = itob(frame);

		mindbl = sector2/60/75;
		min = floor(mindbl);
		secdbl = (sector2 - (min * 60 * 75)) / 75;
		sec = floor(secdbl);
		frame = sector2 - (min * 60 * 75) - (sec * 75);
		buffer[7] = itob(min);
		buffer[8] = itob(sec);
		buffer[9] = itob(frame);

		crc = crc16(buffer, 10);
		fwrite(buffer, 1, 10, subchannel);
		putc((u8)*(((u8 *)&(crc))+1), subchannel);
		putc((u8)crc, subchannel);

		for (i = 0; i < 4; i++) putc(0x00, subchannel);
		printf("Creating M3S: %02u%%\r", (100*sector) / sectors);
	}
	printf("Creating M3S: 100%%\n");
	fclose(subchannel);
	printf("Done!\n");
	return;
}


void matrix(s8 * argv[]) {
	FILE *f1, *f2, *f3, *f4;
	fopen_s(&f1, argv[2], "r+b");
	fopen_s(&f2, argv[3], "r+b");
	fopen_s(&f3, argv[4], "r+b");
	fopen_s(&f4, argv[5], "w+b");

	u8 r1, r2, r3;
	u32 subsize, i = 0;
	subsize = getfilesize(f1);
	for (; i < subsize; i++)
	{
		fread(&r1, 1, 1, f1);
		fread(&r2, 1, 1, f2);
		fread(&r3, 1, 1, f3);
		if (r1 == r2) fwrite(&r1, 1, 1, f4);
		else if (r1 == r3) fwrite(&r1, 1, 1, f4);
		else if (r2 == r3)fwrite(&r2, 1, 1, f4);
		else {
			printf("Byte 0x%x (%d) is different!\n%s: %02x\n%s: %02x\n%s: %02x\n\n", i, i, argv[2], r1, argv[3], r2, argv[4], r3);
			return;
		}
	}
	printf("Done!\n");
	return;
}
void str(s8 * argv[]) {
	FILE * str, * audio, * video;
	u8 ctrlbyte, buffer[2336];
	u32 filesize, sectors;

	if (fopen_s(&str, argv[2], "rb") != 0) {
		printf("Error opening file \"%s\" for reading!\n", argv[2]);
		return;
	}
	filesize = getfilesize(str);
	if (filesize % 2336 != 0) {
		printf("File \"%s\" is not in STR format!\n", argv[2]);
		fclose(str);
		return;
	}
	sectors = filesize / 2336;
	if (fopen_s(&video, argv[3], "wb") != 0) {
		printf("Error creating file \"%s\"!\n", argv[3]);
		return;
	}
	sectors = filesize / 2336;
	if (fopen_s(&audio, argv[4], "wb") != 0) {
		printf("Error creating file \"%s\"!\n", argv[4]);
		return;
	}
	for (u32 i = 0; i < sectors; i++)  {
		fseek(str, 2, SEEK_CUR);
		fread(&ctrlbyte, 1, 1, str);
		if (ctrlbyte >> 5 & 0x1) {
			fseek(str, -3, SEEK_CUR);
			fread(&buffer, 1, 2336, str);
			fwrite(&buffer, 1, 2336, audio);
		}
		else {
			fseek(str, 5, SEEK_CUR);
			fread(&buffer, 1, 2048, str);
			fwrite(&buffer, 1, 2048, video);
			fseek(str, 280, SEEK_CUR);
		}
	}
	fclose(str);
	fclose(audio);
	fclose(video);
	printf("Done!\n");
	return;
}
void str2bs(s8 * argv[]) {
	FILE * str, * bs;					// инициализация файловых потоков
	u8 buffer[2016];						// инициализация буффера
	u8 byte;							// инициализация проверяемого байта
	u16 filenamesize = strlen(argv[2]);		// инициализация длины пути
	s8 * filename = (s8*)calloc(1024, 1);	// инициализация filename
	strcpy(filename, argv[2]);				// записываем путь + имя str-файла в filename
	strcat(filename, "-bs");				// записываем "-bs" в filename
	if (fopen_s(&str, argv[2], "rb") != 0) {
		perror(argv[2]);
		return;
	}
	u32 filesize = getfilesize(str);		// инициализация размера файла
	if (filesize % 2048 != 0) {
		printf("File \"%s\" is not in STR format!\n", argv[2]);
		fclose(str);
		return;
	}

	/*wchar_t newfilename[2048];
	for (u8 f = 0; f < strlen(filename); f++) {
		newfilename[f] = filename[f];
		*((char *)newfilename +f*2 +1) = 0;
	}*/
	CreateDirectoryA((const char *)filename, 0);
	//CreateDirectoryW((const wchar_t *)char2wchar_t((u8 *)filename), 0);	// создаем папку с именем filename
	printf("%u\n", GetLastError());
	strcpy(&*(filename + filenamesize + 3), "\\000001.bs");				// записываем "\000001.bs" в filename
	u32 numblocks = filesize / 2048;									// инициализация кол-ва блоков
	fopen_s(&bs, (const char *)filename, "wb");							// открываем .bs-файл
	u16 a = 1;															//
	u16 b = 0;															// счетчики 
	u16 c = 0;															//
	u8 ax[2] = {0, 0};
	u8 bx[2] = {0, 0};
	u8 cx[2] = {0, 0};
	fseek(str, 32, SEEK_CUR);
	fread(&buffer, 1, 2016, str);
	printf("%s\n",filename);
	fwrite(&buffer, 1, 2016, bs);
	printf("2\n");
	printf("Creating: %s\n", filename);	

	for (u32 i = 1; i < numblocks; i++) {
		fseek(str, 1, SEEK_CUR);
		fread(&byte, 1, 1, str);
		if (!byte) {
			if (a < 99) a++;
			else if (b < 99) {b++; a = 0;}
			else {c++; b = 0; a = 0;}
			fclose(bs);
			itoa(a, (char *)ax, 10);
			itoa(b, (char *)bx, 10);
			itoa(c, (char *)cx, 10);
			if (c < 10) {
				*(filename + filenamesize + 4) = '0';
				strcpy(&*(filename + filenamesize + 5), (const char *)cx);
			} else strcpy(&*(filename + filenamesize + 4), (const char *)cx);

			if (b < 10) {
				*(filename + filenamesize + 6) = '0';
				strcpy(&*(filename + filenamesize + 7), (const char *)bx);
			} else strcpy(&*(filename + filenamesize + 6), (const char *)bx);

			if (a < 10) {
				*(filename + filenamesize + 8) = '0';
				strcpy(&*(filename + filenamesize + 9), (const char *)ax);
			} else strcpy(&*(filename + filenamesize + 8), (const char *)ax);

			*(filename + filenamesize + 10) = '.';

			if (fopen_s(&bs, (const char *)filename, "wb") != 0) printf("Can't open file!\n");
			printf("Creating: %s\n", filename);	
		}
		fseek(str, 30, SEEK_CUR);
		fread(&buffer, 1, 2016, str);
		fwrite(&buffer, 1, 2016, bs);
	}
	fclose(bs);
	fclose(str);
	printf("\nDone!\n");
	free(filename);
	return;
}
void sub(s8 *filename, s8 *strsectors) {
	FILE * subchannel;
	u8 buffer[10], min, sec, frame;
	s8 *end;
	u16 crc;
	u32 sector, sector2, sectors, i;
	double mindbl, secdbl;

	sectors = strtoul(strsectors, &end, 10);
	if (sectors == 0 || sectors == -1) {
		printf("Wrong size!\n");
		return;
	}

	if (fopen_s(&subchannel, filename, "w+b") != 0) {
		perror(filename);
		return;
	}

	printf("File: %s\n", filename);
	printf("Size (bytes): %d\n", sectors * 96);
	printf("Size (sectors): %d\n", sectors);

	buffer[0] = 0x41;
	buffer[1] = 0x01;
	buffer[2] = 0x01;
	buffer[6] = 0x00;

	for (sector = 0, sector2 = 150; sector < sectors; sector++, sector2++) {
		/*if (sector2 == 4350) {
			buffer[1] = 0x02;
			sector = 0;
		}*/
		mindbl = sector/60/75;
		min = floor(mindbl);
		secdbl = (sector - (min * 60 * 75)) / 75;
		sec = floor(secdbl);
		frame = sector - (min * 60 * 75) - (sec * 75);
		buffer[3] = itob(min);
		buffer[4] = itob(sec);
		buffer[5] = itob(frame);

		mindbl = sector2/60/75;
		min = floor(mindbl);
		secdbl = (sector2 - (min * 60 * 75)) / 75;
		sec = floor(secdbl);
		frame = sector2 - (min * 60 * 75) - (sec * 75);
		buffer[7] = itob(min);
		buffer[8] = itob(sec);
		buffer[9] = itob(frame);

		crc = crc16(buffer, 10);

		for (i = 0; i < 12; i++) putc(0x00, subchannel);
		fwrite(buffer, 1, 10, subchannel);
		putc((u8)*(((u8 *)&(crc))+1), subchannel);
		putc((u8)crc, subchannel);
		for (i = 0; i < 72; i++) putc(0x00, subchannel);

		printf("Creating: %02u%%\r", (100*sector) / sectors);
	}
	fseek(subchannel, 0, SEEK_SET);
	for (i = 0; i < 12; i++) putc(0xFF, subchannel);
	printf("Creating: 100%%\n");
	fclose(subchannel);
	printf("Done!\n");
	return;
}
void zektor(s8 * filename) {
	FILE * image;
	u32 filesize, sectors, sector;
	u8 z, zero[4] = {0x00, 0x00, 0x00, 0x00};
	if (fopen_s(&image, filename, "r+b") != 0) {
		perror(filename);
		return;
	}
	filesize = getfilesize(image);
	if (filesize % 2352 != 0) {
		printf("File \"%s\" is not Mode2/2352 image!\n", filename);
		fclose(image);
		return;
	} 
	sectors = filesize / 2352;
	printf("Converting image...\n");
	for (sector = 0; sector < sectors; sector++) {
		fseek(image, sector * 2352 + 18, SEEK_SET);
		fread(&z, 1, 1, image);
		if (z >> 5 & 0x1) {
			fseek(image, 2329, SEEK_CUR);
			fwrite(zero, 1, 4, image);
		}
	}
	fclose(image);
	printf("Done!\n");
	return;
}